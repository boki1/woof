SHM_OBJ=sharedmem.o
SHM_ELF=test/readshm.elf test/writeshm.elf test/destroyshm.elf

CC=clang
CFLAGS=-Wall -g

sharedmem.o: sharedmem.c sharedmem.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.elf: %.o $(SHM_OBJ)
	$(CC) $(CFLAGS) $< $(SHM_OBJ) -o $@

test_shm: $(SHM_ELF) 
	
clean:
	rm *.o test/*.elf
