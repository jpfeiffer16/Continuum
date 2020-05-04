make:
	mkdir -p ./bin
	gcc ./continuum.c -o ./bin/cont -lX11 -lXext -ggdb
	gcc ./client.c -o ./bin/client -lX11 -lXext -ggdb
	gcc ./temp.c ./record.c -o ./bin/temp -lXtst -lX11 -lXext -lXi -ggdb
	gcc ./keyboardtemp.c -o ./bin/keytemp -lX11 -lXext -lXi -ggdb
