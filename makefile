OBJS	= item.o PQ.o list.o main.o
SOURCE	= item.c PQ.c list.c main.c 
HEADER	= item.h PQ.h list.h 
OUT	= trab2
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -lm

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

PQ.o: PQ.c
	$(CC) $(FLAGS) PQ.c 

item.o: item.c
	$(CC) $(FLAGS) item.c 

list.o: list.c
	$(CC) $(FLAGS) list.c 

main.o: main.c
	$(CC) $(FLAGS) main.c 

clean:
	rm -f $(OBJS) $(OUT)

run: $(OUT)
	./$(OUT)
