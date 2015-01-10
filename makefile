all:	server client

server:
	gcc server.c dbtime.c -lrt -o server
  
client:
	gcc -o client client.c

clean:  
	rm -rf server client

