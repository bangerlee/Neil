#include <arpa/inet.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	int                i, j, sockfd[2000];
	struct sockaddr_in servaddr;

	for(i = 1; i < 2001; i++)
	{
		sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);

		bzero(&servaddr, sizeof(servaddr));

		servaddr.sin_family = AF_INET;
		servaddr.sin_port   = htons(5555);
		inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

		connect(sockfd[i], (struct sockaddr*)&servaddr, sizeof(servaddr));
		if(i % 20 == 0)
		{
			for(j = i; j > i - 20; j--)
			{
				close(sockfd[j]);
			}
		}
	}

	sleep(2);

	return 0;
}
