CC = gcc
CFLAGS = -Wall
TARGET = HackAssembler

SRCS = main.c symboltable.c
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c config.h symboltable.h
symboltable.o: symboltable.c config.h symboltable.h

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJS)