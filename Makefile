CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g
TARGET  = calendar
SRCS    = main.c gregorian.c bengali.c convert.c jdn.c events.c storage.c ui.c
OBJS    = $(SRCS:.c=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)