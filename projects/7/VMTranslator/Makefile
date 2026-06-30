CC = gcc
CFLAGS = -Wall -MMD -MP
TARGET = VMTranslator

SRCS = main.c parser.c codeWriter.c
OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

-include $(DEPS)

clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)