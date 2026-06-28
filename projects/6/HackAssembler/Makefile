CC = gcc
CFLAGS = -Wall -MMD -MP
TARGET = HackAssembler

SRCS = main.c symboltable.c assembler.c parser.c code.c test.c
OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

-include $(DEPS)

clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)