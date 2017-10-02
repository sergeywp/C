#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include <net/if.h>
#include <errno.h>
#include <stdio.h>
int main()
{
int sock;                                                                      // дескриптор сокета
struct sockaddr_in *in_addr;                                         // структура интернет-адреса (поля)
struct ifreq ifdata;                                                        // структура - параметр
char      *ifname = "eth1";                                              // имя интерфейса
sock = socket(AF_INET, SOCK_DGRAM, 0);                 // открываем дескриптор сокета
if (sock < 0) {
printf("Не удалось открыть сокет, ошибка: %s\n", strerror(errno));
return 1;
}
memset(&ifdata, 0, sizeof(ifdata));                                // очищаем структуру
strncpy(ifdata.ifr_name, ifname, sizeof(ifname));                       // задаем имя интерфейса
//получаем айпи адрес с помощью SIOCGIFADDR, одновременно проверяя результат
if (ioctl(sock, SIOCGIFADDR, &ifdata) < 0) {
printf("Не получить IP адрес для %s, ошибка: %s\n", ifname, strerror(errno));
close(sock);
return 1;
}
in_addr = (struct sockaddr_in *) &ifdata.ifr_addr;           // преобразовываем из массива байт
// в структуру sockaddr_in
printf("Интерфейс %s IP адрес: %s\n", ifname, inet_ntoa(in_addr->sin_addr));
close(sock);
return 0;
}
