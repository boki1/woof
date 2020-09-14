SHM_OBJ=sharedmem.o
SHM_ELF=readshm.elf writeshm.elf destroyshm.elf

CC=gcc
CFLAGS=-Wall -g -pthread

sharedmem.o: sharedmem.c sharedmem.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.elf: %.o $(SHM_OBJ)
	$(CC) $(CFLAGS) $< $(SHM_OBJ) -o $@

all: $(SHM_ELF) 
	
clean:
	rm *.o *.elf

PHONY: clean all
