# Universidade Federal de São Carlos - Campus Sorocaba
# 	Computação Paralela - 2015/2

#	Erico N. Matthiesen
#	Gabriel Stankevix Soares
#	Maurício Spinardi

# The compiler: gcc for C programs
CC = mpicc

# Compiler flags:
#  -g    Adds debugging information to the executable file
#  -Wall 	Turns on most, but not all, compiler warnings
#  --std=c99	Enables C99
CFLAGS  = -g -Wall --std=c99

# The build target executable:
TARGET = mpi_primes

all: $(TARGET)
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)
