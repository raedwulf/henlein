.SUFFIXES:
.SUFFIXES: .o .c

HDR = henlein.h

LIBHENLEIN = libhenlein.a
LIBHENLEINSRC = henlein.c

LIBHENLEINOBJ = $(LIBHENLEINSRC:.c=.o)
OBJ = $(BIN:=.o) $(LIBHENLEINOBJ)
SRC = $(BIN:=.c)
BIN = example

all: $(BIN)

$(BIN): $(LIBHENLEIN) $(@:=.o)

$(OBJ): $(HDR) config.mk

$(LIBHENLEIN): $(LIBHENLEINOBJ)
	$(AR) rc $@ $?
	$(RANLIB) $@

check: example
	./example

.PHONY: clean

clean:
	rm -f *.o $(BIN) $(LIBHENLEIN)
