# for testing purposes
run:
	build/ca

all:
	gcc ./main.c -L ca.h -I ./ -o build/ca
