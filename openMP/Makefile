# Universidade Federal de São Carlos - Campus Sorocaba
# 	Computação Paralela - 2015/2

#	Erico N. Matthiesen
#	Gabriel Stankevix Soares
#	Maurício Spinardi

# The compiler: gcc for C programs
CC = gcc

# Compiler flags:
#  -g    Adds debugging information to the executable file
#  -Wall 	Turns on most, but not all, compiler warnings
#  -fopenmp		Necessary for OpenMP implementations
CFLAGS  = -g -Wall -fopenmp

# The build target executable:
TARGET = primes

all: $(TARGET)
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)
