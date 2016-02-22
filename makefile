CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wpedantic -Wstrict-overflow -fno-strict-aliasing

ODIR = obj
SDIR = src
EDIR = exe

# Compile everything inside SDIR to objects in OBJ
$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CLAGS)

factorization: $(ODIR)/factorization.o
	$(CC) -o $(EDIR)/factorization $(ODIR)/factorization.o
