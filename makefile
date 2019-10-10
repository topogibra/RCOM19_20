CC = gcc

SRCS := $(wildcard *.c)
BINS := $(SRCS:%.c=%)

dep := packet_factory.c state_machine.c


all: sender receiver

sender:
			@echo "Building sender..."
			$(CC) sender.c $(dep) -o sender

receiver: 
			@echo "Building receiver..."
			$(CC) receiver.c $(dep) -o receiver


clean:
				@echo "Cleaning up..."
				rm -rvf *.o ${BINS}