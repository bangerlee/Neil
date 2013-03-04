#include <arpa/inet.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>

#define SERV_PORT 5555
#define SERV_ADDR "127.0.0.1"

int main(int argc, char** argv)
{
	int                sockfd;
	struct sockaddr_in serveraddr;
	char sendline[30] = "1001lx      lx      \0 ";

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&serveraddr, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port   = htons(SERV_PORT);

	inet_pton(AF_INET, SERV_ADDR, &serveraddr.sin_addr);

	connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	
	write(sockfd, sendline, strlen(sendline));

	pause();

	return 0;
}
