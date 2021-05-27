# the compiler: gcc
CC = gcc

# compiler flags:
#  -Wall turns on most, but not all, compiler warnings
#  -ansi enforce c89 c -standard
CFLAGS  = -Wall -ansi

# the build target executable:
TARGET = program

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)
