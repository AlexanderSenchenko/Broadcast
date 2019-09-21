#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BROADCAST "192.168.1.255"

#define PORT 7777

int fd;

void init_socket();
int hton(const char*);

int main()
{
    int ret;
    char buf[64];
    struct sockaddr_in svaddr;
    socklen_t len;

    init_socket();

    ret = recvfrom(fd, buf, 64, 0, (struct sockaddr*) &svaddr, &len);
	if (ret == -1) {
		perror("recvfrom");
		close(fd);
		exit(EXIT_FAILURE);
	}

    buf[ret++] = '\n';
	write(STDOUT_FILENO, buf, ret);

    shutdown(fd, SHUT_RDWR);
    close(fd);

    return 0;
}

void init_socket()
{
    int ret;
    struct sockaddr_in self_addr;
    socklen_t len;

    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd == -1) {
        perror("socket");
		exit(EXIT_FAILURE);
    }

    self_addr.sin_family = AF_INET;
	self_addr.sin_port = htons(PORT);
	self_addr.sin_addr.s_addr = hton(BROADCAST);

    ret = bind(fd, (struct sockaddr*) &self_addr, sizeof(struct sockaddr_in));
	if (ret == -1) {
		perror("bind");
        shutdown(fd, SHUT_RDWR);
		close(fd);
		exit(EXIT_FAILURE);
	}
}

int hton(const char* addr)
{
    int s_addr, ret;

    ret = inet_pton(AF_INET, addr, &s_addr);
	if (ret == 0) {
		perror("inet_pton(0)");
		close(fd);
		exit(EXIT_FAILURE);
	} else if (ret == -1) {
		perror("inet_pton(-1)");
		close(fd);
		exit(EXIT_FAILURE);
	}

    return s_addr;
}