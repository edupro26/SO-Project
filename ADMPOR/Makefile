OBJ_dir = obj
OBJECTOS = main.o process.o memory.o intermidiary.o enterprise.o client.o
main.o = funcoes.h
par.o =
CC = gcc

out: $(OBJECTOS)
	$(CC) $(addprefix $(OBJ_dir)/,$(OBJECTOS)) -o bin/out
%.o: src/%.c $($@)
	$(CC) -I include -o $(OBJ_dir)/$@ -c $<

cleanall:
	rm -rf $(OBJ_dir)/* bin/*