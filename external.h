#ifndef _EXTERNAL_H
#define _EXTERNAL_H

#include "includes/cyg_type.h"

#define SOCK_STREAM	1		/* stream (connection) socket	*/
#define SOCK_DGRAM	2		/* datagram (conn.less) socket	*/
#define SOCK_RAW	3		/* raw socket			*/

#define AF_INET		0x2	/* Internet IP Protocol 	*/
#define AF_INET6	0x1c	/* IP version 6			*/

#define SOL_SOCKET  0xffff      /* options for socket level */

/*
 * Option flags per-socket.
*/
#define SO_DEBUG    0x0001      /* turn on debugging info recording */
#define SO_ACCEPTCONN   0x0002      /* socket has had listen() */
#define SO_REUSEADDR    0x0004      /* allow local address reuse */
#define SO_KEEPALIVE    0x0008      /* keep connections alive */
#define SO_DONTROUTE    0x0010      /* just use interface addresses */
#define SO_BROADCAST    0x0020      /* permit sending of broadcast msgs */
#define SO_USELOOPBACK  0x0040      /* bypass hardware when possible */
#define SO_LINGER   0x0080      /* linger on close if data present */
#define SO_OOBINLINE    0x0100      /* leave received OOB data in line */
#define SO_REUSEPORT    0x0200      /* allow local address & port reuse */


struct cyg_thread {
    int sched_info;
    int entry;
    int entry_data;
    char*   name;
    int stack_base;
    int stack_size;
};
typedef struct cyg_thread cyg_thread;

struct sockaddr 
{
	unsigned short	sa_family;	/* address family, AF_xxx	*/
	char		sa_data[14];	/* 14 bytes of protocol address	*/
};


typedef struct sock_addr_s {
	unsigned int _pad1;
	unsigned int port;
	unsigned int addr;
	unsigned int _pad4;
	unsigned int _pad5;
	unsigned int _pad6;
	unsigned int _pad7;
	unsigned int _pad8;
} sock_addr_t;

// Socket/connection information

typedef char    int8_t;
typedef unsigned char        u_int8_t;
typedef unsigned char         uint8_t;
typedef short             int16_t;
typedef unsigned short      u_int16_t;
typedef unsigned short       uint16_t;
typedef int           int32_t;
typedef unsigned int        u_int32_t;
typedef unsigned int         uint32_t;
typedef long long         int64_t;
typedef unsigned long long  u_int64_t;
typedef unsigned long long   uint64_t;

typedef u_int8_t        sa_family_t;
typedef u_int16_t       in_port_t;

//LINUX

/**struct in_addr {
     in_addr_t s_addr;
};*/

/** LINUX
 * struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};*/

typedef struct in_addr {
    unsigned long  s_addr;  // IPv4 address
} in_addr_t;

struct sockaddr_in {
    short sin_family; 
    unsigned short sin_port;
    struct in_addr sin_addr;
    char sin_zero[8];
};

/* stdlib functions */

void* memset(void* buf, int c, unsigned int n);
void* memcpy(void* dst, const void* src, int n);
void* malloc(unsigned int size);
int printf(char* str, ...);
void bzero(void *s, int n);

/* network functions */

int socket(int domain, int type, int protocol);

/*
 * Create a new socket with IP protocol ip_proto on IP stack number ip_stack
 * _a and _b are unknown, use 1 and 6 respectively for IPv4 TCP.
 * Use 2 and 0x11 for UDP.
 * Returns NULL on failure and pointer to socket handle on success.
 */
//void* socket(int ip_proto, int _a, int _b, int ip_stack);
//int socket(int domain, int type, int protocol);

//int setsockopt(int sockfd, int level, int optname, const void *optval, int optlen);

/* 
 * Close the give socket.
 */
void close(int handle);


/* 
 * Bind the valid socket handle to the given address and port. 
 * Returns 0 on success. 
 */
int bind(int sockfd, const struct sockaddr_in *addr, int addrlen);


int connect(int sockfd, const struct sockaddr_in *addr, int addrlen);
//int connect(void* handle, void* addr, int addrlen);
/* 
 * Listen for a TCP connection on the bound socket. 
 * Arg _a is unknown, probably backlog like posix listen(). Use 1.
 * Returns 0 on success.
 */
int listen(int sockfd, int backlog);
//int listen(void* handle, int _a);

/* 
 * Accept a connection on a listening socket. 
 * Args _a and _b are unknown, use 0 and 0 respectively.
 * Returns a pointer to a new socket handle on success or NULL on failure.
 */
int accept(int sockfd, struct sockaddr *addr, int *addrlen);


/* 
 * Send len bytes of data at the buffer buf on the connection handle.
 * Arg _a is unknown, use 0.
 * Returns the number of bytes send on success.
 */
int send(void* handle, char* buf, int len, int _a);

/* 
 * Read up to len bytes of data from the connection handle into buffer buf.
 * Arg _a is unknown, use 0.
 * Returns the number of bytes read on success.
 */
int recv(int sockfd, void *buf, int len, int flags);

/*
 * For UDP sockets
 */
int recvfrom(void* handle, char* buf, int len, int flags, int* addr, int* addrlen);
int sendto(void* handle, char* buf, int len, int flags, int* addr, int addrlen);


/*
 * Sleep for t milliseconds.
 */
void sleep(unsigned int t);

int* cyg_fp_get(int fd);
void cyg_fp_free(void *fp);
void cyg_fd_free(int fd);
int cyg_fd_assign(int fd, void *fp);

unsigned int inet_addr(char *param_1);

void* BcmConsoleGetSingletonInstance(void);
int BcmConsoleExecuteCurrentCommand(void* console);


typedef struct
 {
     cyg_handle_t        handle;
     cyg_uint16          id;
     cyg_uint32          state;
     char                *name;
     cyg_priority_t      set_pri;
     cyg_priority_t      cur_pri;
     cyg_addrword_t      stack_base;
     cyg_uint32          stack_size;
     cyg_uint32          stack_used;
 } cyg_thread_info;

void cyg_thread_create(int sched_info, void *entry, void* entry_data, char *name, void *stack_base, unsigned int stack_size,  int *handle,  cyg_thread *thread);

void cyg_thread_resume(int thread);

void cyg_thread_exit(void);

cyg_bool_t cyg_thread_get_next(cyg_handle_t *current, cyg_uint16 *id);

cyg_handle_t cyg_thread_find(cyg_uint16 id);

cyg_bool_t cyg_thread_get_info(cyg_handle_t threadh, cyg_uint16 id, cyg_thread_info *info);

void free(void *ptr);


int getsockname(int s, struct sockaddr *name, int *namelen);

// Internet protocols
struct protoent {
     char *p_name;
     int   p_proto;
};

struct protoent *getprotobyname(const char *);
struct protoent *getprotobynumber(const int);


struct servent {
     char    *s_name;        /* official service name */
     char    **s_aliases;    /* alias list */
     int     s_port;         /* port number */
     char    *s_proto;       /* protocol to use */
};

struct servent *getservbyname(const char *name, const char *proto);
struct servent *getservbyport(int port, const char *proto);


int* cyg_error_get_errno_p(void);


int strlen(const char *s);

int write(int fd, const void *buf, int count);

char *inet_ntoa(struct in_addr in);

char *strcpy(char *dest, const char *src);

char *strncpy(char *dest, const char *src, int n);

int strncmp(const char *s1, const char *s2, int n);

int getpeername(int sockfd, struct sockaddr *addr, int *addrlen);


int inet_ntop4(void* src,char* dst, int size);

char * strerror( int error_code );

#define errno (*cyg_error_get_errno_p())
#endif /* _EXTERNAL_H */
