CC = gcc
CFLAGS = -I. -g -w
LDFLAGS = -lm

SRCS = hashset.c spellcheck.c spell_checker.c

OBJS = $(SRCS:.c=.o)

EXEC = spell_checker

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC) $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
bucketsort: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJS) $(EXEC)
