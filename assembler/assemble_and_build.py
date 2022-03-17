import os, sys

# Messy code, but it gets the job done.

# Path
PATH = os.path.abspath('')

# Binaries
BOOT_BIN = "bin\\boot.bin"
MAIN_PROG_BIN = "bin\\main_prog.bin"

# Dist
BOOT_FLP = "dist\\boot.flp"

# SRC Files
BOOTLOADER = "src\\sector1\\bootloader.asm"
MAIN_PROG = 'src\\sector2+\\MainProg.asm'

def rc(command):
    sys.stdout.write("[INFO] Running %s\n" % command)
    os.system(command)

def tr(func,  __condition, int_to_bool, *args, **kwargs):
    if not __condition: return False 
    
    try: 
        sys.stdout('[INFO] Running {}({}, {})\n'.format(
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
        tr(os.remove, os.path.isfile(BOOT_BIN), True, BOOT_BIN)
        tr(os.remove, os.path.isfile(MAIN_PROG_BIN), True, MAIN_PROG_BIN)
        tr(os.remove, os.path.isfile(BOOT_FLP), True, BOOT_FLP)
        tr(os.makedirs, not os.path.isdir('.logs'), True, '.logs')
        
        rc(f'nasm {BOOTLOADER} -f bin -o {BOOT_BIN}')
        rc(f'nasm {MAIN_PROG} -f bin -o {MAIN_PROG_BIN}')
        rc(f'copy /b {BOOT_BIN}+{MAIN_PROG_BIN} {BOOT_FLP}')
            
    if '-r' in sys.argv or '--run' in sys.argv:
        rc('bochsrc.bxrc')
            
    sys.exit(0)
