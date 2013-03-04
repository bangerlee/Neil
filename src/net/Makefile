all:
	g++ -c epoll_server.cpp pubSource.cpp acceptThread.cpp sendThread.cpp queue.c thread.cpp lock.c
	ar -cr libepollserver.a epoll_server.o pubSource.o acceptThread.o sendThread.o queue.o thread.o lock.o
#	gcc -DQUEUE_TEST queue.c lock.c -o queue_test
#	gcc -DLOCK_TEST lock.c -o lock_test
clean:
	rm *.o *.a
