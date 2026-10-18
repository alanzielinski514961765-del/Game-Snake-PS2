EE_BIN = SLES_555.55
EE_OBJS = main.o
EE_LIBS = -lpad -lgraph -ldraw -lkernel -lc

all: $(EE_BIN)

clean:
	rm -f *.elf *.o $(EE_BIN)

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/common/Make.rules
