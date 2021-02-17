#ifndef _PAYLOAD_H
#define _PAYLOAD_H

/*
 * Entrypoint
 */
int __start(unsigned long ip_address, unsigned short port) __attribute__((section(".start")));

/*
 * Network thread
 */
int payload(struct sockaddr_in* host);

#endif /* _PAYLOAD_H */
