make:
	mkdir -p ./bin
	gcc ./continuum.c -o ./bin/cont -lX11 -lXext -ggdb
	gcc ./client.c -o ./bin/client -lX11 -lXext -ggdb
	gcc ./temp.c -o ./bin/temp -lX11 -lXext -lXi -ggdb
