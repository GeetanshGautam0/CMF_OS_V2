set WSLPATH="/usr/local/x86_64elfgcc/bin"

nasm src\sector1\bootloader.asm -f bin -o bin\bootloader.bin
nasm src\sector2+\MainProg.asm -f elf64 -o bin\main_prog.o

wsl %WSLPATH%/x86_64-elf-gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "src/kernel/Kernel.cpp" -o "bin/kernel.o"
wsl %WSLPATH%/x86_64-elf-ld -T"link.ld"

copy /b bin\bootloader.bin+bin\kernel.bin dist\boot.flp

pause