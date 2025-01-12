# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2

# target ονόματα
TARGET = main
OBJS = main.o

# Build το εκτελέσιμο
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile το main.c
main.o: main.c headers.h
	$(CC) $(CFLAGS) -c main.c

# Φωνάζουμε την κυρία Λένια για γενική καθαριότητα (clean τα artifacts)
clean:
	rm -f $(TARGET) $(OBJS)

# PHONY target to avoid conflicts with files named clean ( ?? )
.PHONY: clean
