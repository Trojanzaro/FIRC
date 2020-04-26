# FIRC
A Fake IRC program

Make sure to compile both programs with the `-pthread` header included
```
$ gcc client.c -o client -pthread 
$ gcc server.c -o server -pthread
```

To run the client you only need to include the arguments:

1) Server's IP (No hostname resolution implemented yet sorry :-p )
	
2) Username (50 chars only) to appear in the chatroom
	
```
$ ./client 127.0.0.1 UserName
```

To run the server you only need to run it

```
$ ./server
```

Server runs on port 1234
