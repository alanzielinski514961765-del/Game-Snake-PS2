EE_BIN = SLES_555.55
EE_OBJS = main.o
EE_LIBS = -lc

all: $(EE_BIN)
	mkdir -p iso_dir
	cp $(EE_BIN) iso_dir/SLES_555.55
	printf "BOOT2 = cdrom0:\\\\SLES_555.55;1\nVER = 1.00\nVMODE = NTSC\n" > iso_dir/SYSTEM.CNF
	xorriso -as mkisofs -iso-level 1 -full-iso9660-filenames -o SNAKE_GAME.ISO iso_dir/

clean:
	rm -f $(EE_BIN) $(EE_OBJS) SNAKE_GAME.ISO
	rm -rf iso_dir

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
