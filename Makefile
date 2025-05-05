# for testing purposes
run: all
	build/ca

all:
	gcc ./main.c -L ca.h -I ./ -o build/ca -lm
