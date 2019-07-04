#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // for close
#define MAX 80
#define PORT 8888
#define SA struct sockaddr

// Функция общения клиента с сесвером.
void func(int sockfd)
{
	char buff[MAX];
	int i;
	char *end;
	for (;;) {
		bzero(buff, MAX);
		// Копирование сообщения клиента в буфер
		read(sockfd, buff, sizeof(buff));
		// Конвертация принятых данных в Int
		const char *p=buff;
	  i = strtol(p, &end, 10);
		printf("'%.*s' -> ", (int)(end-p), p);
		bzero(buff, MAX);
		// Операции на числом
		i=i*2;
		printf("%i\n", i);
		// Int to string
		sprintf(buff,"%i",i);
		// Отправка результата клиенту
		write(sockfd, buff, sizeof(buff));
		break;
	}
}
// Driver function
int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server acccept failed...\n");
		exit(0);
	}
	else
		printf("server acccept the client...\n");

	// Вызов функции обмена сообщениями
	func(connfd);

	// Закрытие сокета
	shutdown(sockfd,SHUT_RDWR);
}
