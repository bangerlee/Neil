#ifndef SOCKET_INCLUDE_H
#define SOCKET_INCLUDE_H

#ifndef _WIN32
//------------------------
//common unix includes / defines
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif //!_WIN32

typedef int SOCKET;
#define Errno errno
#define StrError strerror

//-------------------------
//OS specific adaptions

//-------------------------
//LINUX
typedef unsigned long ipaddr_t;
typedef unsigned short port_t;
#ifdef SOCKETS_NAMESPACE
namespace SOCKETS_NAMESPACE {
#endif
//no defs

#ifdef SOCKETS_NAMESPACE
}
#endif

#endif //_SOCKET_INCLUDE_H
