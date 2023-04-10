OBJ_dir = obj
OBJECTS = main.o memory.o process.o client.o intermediary.o enterprise.o
main.o = main.h main-private.h process.h
memory.o = memory.h
process.o = process.h client.h intermediary.h enterprise.h
client.o = client.h
intermediary.o = intermediary.h
enterprise.o = enterprise.h
CFLAGS = -Wall
CC = gcc

admpor: $(OBJECTS)
	$(CC) $(addprefix $(OBJ_dir)/,$(OBJECTS)) -o bin/admpor
%.o: src/%.c $($@)
	$(CC) $(CFLAGS) -I include -o $(OBJ_dir)/$@ -c $<
clean:
	rm -rf $(OBJ_dir)/*.o bin/*