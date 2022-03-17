import os, sys, subprocess, random, time

# Messy code, but it gets the job done.

# Path
PATH = os.path.abspath('')

# Binaries
BOOT_BIN = "bin/boot.bin"
BOOT_FLP = "dist/boot.flp"

# SRC Files
BOOTLOADER = "src/bootloader.asm"

def tr(func,  __condition, int_to_bool, *args, **kwargs):
    if not __condition: return False 
    
    try: 
        output = func(*args, **kwargs)
        
        if isinstance(output, int) and int_to_bool:
            output = not bool(output)
            
        return output
        
    except: return False


if __name__ == "__main__":
    if '-c' in sys.argv or '--compile' in sys.argv:
        tr(os.remove, os.path.isfile(BOOT_BIN), True, BOOT_BIN)
        tr(os.remove, os.path.isfile(BOOT_FLP), True, BOOT_FLP)
        tr(os.makedirs, not os.path.isdir('.logs'), True, '.logs')
        
        os.system(f'nasm {BOOTLOADER} -f bin -o {BOOT_BIN}')
        os.system(f'copy {BOOT_BIN} {BOOT_FLP}')
        if not os.path.exists(BOOT_FLP):
            assert tr(os.system, os.path.exists('dist\\boot.bin'), True, f'ren "dist\\boot.bin" "boot.flp"'), \
                f"Failed to compile {BOOT_FLP}"
            
    if '-r' in sys.argv or '--run' in sys.argv:
        os.system('bochsrc.bxrc')
            
    sys.exit(0)
