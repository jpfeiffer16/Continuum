make:
	mkdir -p ./bin
	gcc ./continuum.c -o ./bin/cont -lX11 -lXext -g
