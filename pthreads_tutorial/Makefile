CC = gcc
CFLAGS = -Wall -Wextra -g -lpthread
LDFLAGS = -lpthread

# Source files and object files
SRCS = 01_serial.c 02_serial_time.c 03_parallel1.c 04_parallel2.c 05_parallel3.c 06_parallel4.c
OBJS = $(SRCS:.c=.o)
EXEC = $(SRCS:%.c=%)

all: $(EXEC)

clean:
	rm -f $(EXEC)
	rm -rf $(EXEC:%=%.dSYM)
	rm -f serial.txt
	rm -f parallel.txt