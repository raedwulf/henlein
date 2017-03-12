# henlein version
VERSION = 0.0.1

PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

CC = cc
AR = ar
RANLIB = ranlib

CPPFLAGS = -D_XOPEN_SOURCE=700 -D_DEFAULT_SOURCE
CFLAGS   = -O2 -std=gnu99 -Wall -pedantic
LDFLAGS  = -s

ifneq ($(OS),Windows_NT)
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        LDFLAGS += -lrt
    endif
endif
