CC = gcc
CFLAGS = -w
LDFLAGS = -lm

SRCS = hashset.c spellcheck.c spell_checker.c

OBJS = $(SRCS:.c=.o)

EXEC = spell_checker

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(EXEC)
