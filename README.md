# FIRC
A Fake IRC program

Make sure to compile both programs with the `-pthread` header included
```
$ gcc client.c -o client -pthread 
$ gcc server.c -o server -pthread
```

To run the client you only need to include the arguments:

1) Server's IP or hostname (Yay! Implemented :-p )

3) Server's port (1234 By default)
	
2) Username (50 chars only) to appear in the chatroom
	
```
$ ./client localhost 1234 UserName
```

To run the server you only need to run it

```
$ ./server
```
