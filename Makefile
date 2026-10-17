EE_BIN = SNAKE.ELF
EE_OBJS = main.o
EE_LIBS = -lpad

all: $(EE_BIN)

clean:
	rm -f $(EE_BIN) $(EE_OBJS)

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/common/Rules.make
