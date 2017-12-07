CC=gcc
CFLAGS=-std=gnu99 -W -Wall -I/usr/local/include/pbc
LDFLAGS=-lpbc -lgmp
EXEC=abe
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)
all: $(EXEC)
abe: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
.PHONY: clean mrproper run
clean:
	rm -rf *.o
mrproper: clean
	rm -rf $(EXEC)	
	


