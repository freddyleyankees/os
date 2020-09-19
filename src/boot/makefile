
NASMBINPARAM = -f bin -o
O_BOOT = booter.bin inibtos.bin

booter.bin: bs.asm
		nasm $(NASMBINPARAM) $@ $< 
		
inibtos.bin: startup.asm
		nasm $(NASMBINPARAM) $@ $< 

all : $(O_BOOT)
		sudo mv *.bin ../;
