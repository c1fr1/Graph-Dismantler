LIBS=-lm -lglfw -lglad
DFLAGS=-DDGL_DEBUG -g
OBJS=out/shader.o out/dgl.o out/texture.o out/vao.o out/dglKeyboardStates.o

out/main: out src/main.c $(OBJS)
	gcc $(DFLAGS) $(OBJS) src/main.c -o $@ $(LIBS)

out/shader.o: src/shader.c src/shader.h
	gcc $(DFLAGS) -c src/shader.c -o $@

out/dgl.o: src/dgl.c src/dgl.h
	gcc $(DFLAGS) -c src/dgl.c -o $@

out/texture.o: src/texture.c src/texture.h
	gcc $(DFLAGS) -c src/texture.c -o $@

out/vao.o: src/vao.c src/vao.h
	gcc $(DFLAGS) -c src/vao.c -o $@

out/dglKeyboardStates.o: src/dglKeyboardStates.c src/dglKeyboardStates.h
	gcc $(DFLAGS) -c src/dglKeyboardStates.c -o $@

.PHONY: run clean release out

out:
	mkdir -p out

release: out
	gcc -c src/shader.c -o out/shader.o
	gcc -c src/dgl.c -o out/dgl.o
	gcc -c src/texture.c -o out/texture.o
	gcc -c src/vao.c -o out/vao.o
	gcc -c src/dglKeyboardStates.c -o out/dglKeyboardStates.o
	gcc $(OBJS) src/main.c -o out/main $(LIBS)

run: out/main
	out/main

clean: 
	rm out/ -r
