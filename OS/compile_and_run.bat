@echo off
cls

nasm bootloader.asm -f bin -o bootloader.bin
nasm MainEntry.asm -f bin -o MainEntry.bin

copy /b bootloader.bin+MainEntry.bin bootloader.flp

bochsrc.bxrc
