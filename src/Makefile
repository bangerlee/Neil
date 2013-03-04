all:
	g++ game_start.cpp loginMgr.cpp CMySQLMan.cpp Global_Var.h process_map.cpp process.cpp -lpthread -I/usr/include/mysql/ -L/usr/lib/mysql -lmysqlclient -L net/ -lepollserver -o game_start
	gcc client.c -o client
clean:
	rm game_start client
