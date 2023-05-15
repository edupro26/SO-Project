OBJ_dir = obj
OBJECTS = main.o memory.o process.o client.o intermediary.o enterprise.o synchronization.o configuration.o apsignal.o aptime.o log.o stats.o
main.o = main.h main-private.h process.h configuration.h apsignal.h log.h stats.h
memory.o = memory.h
process.o = process.h client.h intermediary.h enterprise.h synchronization.h
client.o = client.h synchronization.h aptime.h
intermediary.o = intermediary.h synchronization.h aptime.h
enterprise.o = enterprise.h synchronization.h aptime.h
synchronization.o = synchronization.h
configuration.o = configuration.h
apsignal.o = apsignal.h
aptime.o = aptime.h
log.o = log.h
stats.o = stats.h
CFLAGS = -g -lrt -Wall -Wextra
CC = gcc

admpor: $(OBJECTS)
	$(CC) $(addprefix $(OBJ_dir)/,$(OBJECTS)) -o bin/admpor
%.o: src/%.c $($@)
	$(CC) $(CFLAGS) -I include -o $(OBJ_dir)/$@ -c $<
clean:
	rm -rf $(OBJ_dir)/*.o bin/* log.txt stats.txt