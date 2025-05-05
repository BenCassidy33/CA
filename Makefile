all:
	gcc ./main.c -L ca.h -I ./ -o ca
	./ca
