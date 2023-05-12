OBJ_dir = obj
OBJECTS = main.o memory.o process.o client.o intermediary.o enterprise.o synchronization.o configuration.o apsignal.o
main.o = main.h main-private.h process.h configuration.h
memory.o = memory.h
process.o = process.h client.h intermediary.h enterprise.h synchronization.h
client.o = client.h synchronization.h
intermediary.o = intermediary.h synchronization.h
enterprise.o = enterprise.h synchronization.h
synchronization.o = synchronization.h
configuration.o = configuration.h
apsignal.o = apsignal.h
CFLAGS = -Wall -Wextra
CC = gcc

admpor: $(OBJECTS)
	$(CC) $(addprefix $(OBJ_dir)/,$(OBJECTS)) -o bin/admpor
%.o: src/%.c $($@)
	$(CC) $(CFLAGS) -I include -o $(OBJ_dir)/$@ -c $<
clean:
	rm -rf $(OBJ_dir)/*.o bin/*