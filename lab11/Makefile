# Makefile for linked-list polynomial lab (Lecture 11 style)
CC      = gcc
CFLAGS  = -O3 -Wall -Wextra -std=c11
LDFLAGS = -lm

OBJECTS = main.o \
          GetNumberOfTerms.o \
          ReadPoly.o \
          InsertTerm.o \
          PrintPoly.o \
          AddPoly.o \
          EvalPoly.o \
          DeletePoly.o

.PHONY: all clean help

all: main.exe

main.exe: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o main.exe $(LDFLAGS)

%.o: %.c term.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS) main.exe

help:
	@echo "Valid targets:"
	@echo "  make        (build main.exe)"
	@echo "  make clean  (remove objects and executable)"
	@echo "  make help"