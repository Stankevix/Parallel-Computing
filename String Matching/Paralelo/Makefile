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
#  -fopenmp		Enable OpenMP implementations
#  -fopenmp		Enable PThread implementations
CFLAGS  = -g -Wall -fopenmp -lpthread

# The build target executable:
TARGET = substring

all: $(TARGET)
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)
