CC = gcc
CFLAGS = -Wall -MMD -MP
TARGET = JackAnalyzer

# SRCS = main.c jackTokenizer.c compilationEngine.c
SRCS = main.c jackTokenizer.c
OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

-include $(DEPS)

clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)