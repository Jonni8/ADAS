#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#define DEF_PROTOCOL 0
#define SOCKET_NAME "ADAS"

char* oraCorrente();
void redigiLog(int fd);