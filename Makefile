include config.mk

.SUFFIXES:
.SUFFIXES: .o .c

HDR = henlein.h

LIBHENLEIN = libhenlein.a
LIBHENLEINSRC = henlein.c
LIBHENLEINOBJ = $(LIBHENLEINSRC:.c=.o)

BIN = example
OBJ = $(BIN:=.o) $(LIBHENLEINOBJ)
SRC = $(BIN:=.c)

all: $(BIN)

$(LIBHENLEIN): $(LIBHENLEINOBJ)
	$(AR) rc $@ $?
	$(RANLIB) $@

$(BIN): $(LIBHENLEIN) $(@:=.o)

$(OBJ): $(HDR) config.mk

.o:
	$(CC) $(LDFLAGS) -o $@ $< $(LIBHENLEIN)

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

check: example
	./example

clean:
	rm -f *.o $(BIN) $(LIBHENLEIN)

.PHONY: all check clean
