
CC		= gcc
CFLAGS	= -I. -O2
LDFLAGS	= -dynamiclib -Wl,-single_module
RM		= rm -f

TARGET-test	= MC6803_TEST
OBJS-test	= MC6803_TEST.o

TARGET-libMC6803E	= libMC6803E.dylib
OBJS-libMC6803E	= Image.o MC6803E_ALU.o MC6803E_Memory.o MC6803E_Core.o

all:: $(TARGET-libMC6803E) $(TARGET-test)

$(TARGET-libMC6803E): $(OBJS-libMC6803E)
	$(CC) $(OBJS-libMC6803E) $(LDFLAGS) -o $@

$(TARGET-test): $(OBJS-test)
	$(CC) $(CFLAGS) -L. -lMC6803E $< -o $@

clean::
	find . -name \*.o | xargs $(RM)
	$(RM) $(TARGET-libMC6803E) $(OBJS-libMC6803E)
	$(RM) $(OBJS-test) $(TARGET-test)
