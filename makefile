# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wstrict-overflow -fno-strict-aliasing

# Required directories
EDIR = exe
ODIR = obj
SDIR = src

# Alias commands
mkdir = mkdir -p
rm = rm -f

################################################################################
# Build commands
################################################################################

all: directories factorization.o factorization

# Make the required directories
directories:
	$(mkdir) $(EDIR)
	$(mkdir) $(ODIR)

# Compile the object file
factorization.o:
	$(CC) $(CFLAGS) -c $(SDIR)/factorization.c -o $(ODIR)/factorization.o

# Compile the executable
factorization: $(ODIR)/factorization.o
	$(CC) $(CFLAGS) -o $(EDIR)/factorization $(ODIR)/factorization.o

# Clean the EDIR and ODIR directories
clean:
	$(rm) $(EDIR)/*
	$(rm) $(ODIR)/*.o
