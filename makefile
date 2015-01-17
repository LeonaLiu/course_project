all:	server client

server:
	gcc server.c transmit.c sock.c strtool.c dbtime.c -lrt -lpthread -o server
  
client:
	gcc client.c transmit.c sock.c strtool.c dbtime.c -lrt -lpthread -o client 

clean:  
	rm -rf server client *bak dbtime.time out.md5	 
