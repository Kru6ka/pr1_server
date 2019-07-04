#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // for close
#define MAX 80
#define PORT 8888
#define SA struct sockaddr
// RED LED2
#define RED_ON                                                                 \
  "echo 1 > /sys/devices/platform/leds1/leds/status1:red:mmc0/brightness"
#define RED_OFF                                                                \
  "echo 0 > /sys/devices/platform/leds1/leds/status1:red:mmc0/brightness"
// BLUE LED2
#define BLU_ON                                                                 \
  "echo 1 > /sys/devices/platform/leds1/leds/status1:yellow:usr/brightness"
#define BLU_OFF                                                                \
  "echo 0 > /sys/devices/platform/leds1/leds/status1:yellow:usr/brightness"
// GREEN LED1
#define GRN_ON                                                                 \
  "echo 1 > /sys/devices/platform/leds1/leds/status0:green:usr/brightness"
#define GRN_OFF                                                                \
  "echo 0 > /sys/devices/platform/leds1/leds/status0:green:usr/brightness"
int system(const char *command);
// Функция общения клиента с сесвером.
float convert(char *mass) {
  // Конвертация принятых данных в число
  char *end;
  const char *p = mass;
  float x = strtof(p, &end);
  return x;
}
float math_ops(float nmbr) {
  nmbr = nmbr * 2;
  return nmbr;
}
void func(int sockfd) {
  char buff[MAX];
  float i;
  for (;;) {
    bzero(buff, MAX);
    // Копирование сообщения клиента в буфер
    read(sockfd, buff, sizeof(buff));
    i = convert(buff);
    bzero(buff, MAX);
    // Операции на числом
    float result = math_ops(i);
    if (result == 0) {
      printf("Client disconnected. Shutting down...\n");
      usleep(100000);
      system(GRN_OFF);
      system(RED_ON);
      usleep(300000);
      system(RED_OFF);
      usleep(300000);
      system(RED_ON);
      usleep(300000);
      system(RED_OFF);
      close(sockfd);
      break;
    }
    printf("%f -> ", i);
    printf("%f\n", result);
    // Int to string
    sprintf(buff, "%f", result);
    // Отправка результата клиенту
    write(sockfd, buff, sizeof(buff));
    system(BLU_ON);
    usleep(100000);
    system(BLU_OFF);
  }
}
// Driver function
int main() {
  system(BLU_OFF);
  system(RED_OFF);
  system(GRN_OFF);
  int sockfd, connfd, len;
  struct sockaddr_in servaddr, cli;

  // socket create and verification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    printf("socket creation failed...\n");
    exit(0);
  } else
    printf("Socket successfully created..\n");
  bzero(&servaddr, sizeof(servaddr));

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  // Binding newly created socket to given IP and verification
  if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0) {
    printf("socket bind failed...\n");
    system(RED_ON);
    sleep(3);
    system(RED_OFF);
    exit(0);
  } else
    printf("Socket successfully binded..\n");

  // Now server is ready to listen and verification
  if ((listen(sockfd, 5)) != 0) {
    printf("Listen failed...\n");
    exit(0);
  } else
    printf("Server listening..\n");
  len = sizeof(cli);

  // Accept the data packet from client and verification
  connfd = accept(sockfd, (SA *)&cli, &len);
  if (connfd < 0) {
    printf("server acccept failed...\n");
    exit(0);
  } else {
    printf("server acccept the client...\n");
    system(GRN_ON);
  }

  // Вызов функции обмена сообщениями
  func(connfd);

  // Закрытие сокета
  close(sockfd);
  system(BLU_OFF);
  system(GRN_OFF);
  system(RED_OFF);
}
