import os, sys

# Messy code, but it gets the job done.

# Path
PATH = os.path.abspath('')
WSL_PATH = "wsl /usr/local/x86_64elfgcc/bin"

def rc(command):
    sys.stdout.write("[INFO] Running %s\n" % command)
    os.system(command)

def tr(func,  __condition, int_to_bool, *args, **kwargs):
    if not __condition: return False 
    
    try: 
        sys.stdout.write('[INFO] Running {}({}, {})\n'.format(
            str(func),
            ', '.join((str(i) for i in args)),
            ', '.join(f"{k}={v}" for k, v in kwargs.items())
        ))
        
        output = func(*args, **kwargs)
        
        if isinstance(output, int) and int_to_bool:
            output = not bool(output)
            
        return output
        
    except: return False


if __name__ == "__main__":
    if '-c' in sys.argv or '--compile' in sys.argv:      
        tr(os.remove, os.path.isfile('bin\\bootloader.bin'), True, 'bin\\bootloader.bin')
        tr(os.remove, os.path.isfile('bin\\main_prog.bin'), True, 'bin\\main_prog.bin')
        tr(os.remove, os.path.isfile('bin\\main_prog.o'), True, 'bin\\main_prog.o')
        tr(os.remove, os.path.isfile('bin\\kernel.bin'), True, 'bin\\kernel.bin')
        tr(os.remove, os.path.isfile('bin\\kernel.o'), True, 'bin\\kernel.o')
        tr(os.remove, os.path.isfile('bin\\kernel.tmp'), True, 'bin\\kernel.tmp')
        tr(os.remove, os.path.isfile('dist\\boot.flp'), True, 'dist\\boot.flp')
        
        if '--legacy' in sys.argv:
            sys.stdout.write("[INFO] Using legacy compilation")
            rc(f'nasm src\\sector1\\bootloader.asm -f bin -o bin\\bootloader.bin')
            rc(f'nasm src\\sector2+\\MainProg.asm -f elf64 -o bin\\main_prog.bin')    
            rc(f'copy /b bin\\bootloader.bin+bin\\main_prog.bin dist\\boot.flp')
            
        else:    
            rc(f'nasm src\\sector1\\bootloader.asm -f bin -o bin\\bootloader.bin')
            rc(f'nasm src\\sector2+\\MainProg.asm -f elf64 -o bin\\main_prog.o')        
            rc(f'{WSL_PATH}/x86_64-elf-gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "./src/kernel/Kernel.cpp" -o "./bin/kernel.o"')
            rc(f'{WSL_PATH}/x86_64-elf-ld -T"link.ld"')
            rc(f'copy /b bin\\bootloader.bin+bin\\kernel.bin dist\\boot.flp')
            
    if '-r' in sys.argv or '--run' in sys.argv:
        rc('start bochsrc.bxrc')
            
    sys.exit(0)
