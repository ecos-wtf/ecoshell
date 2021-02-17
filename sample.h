#ifndef _SAMPLE_H
#define _SAMPLE_H

/*
 * Entrypoint
 */
int __start(void) __attribute__((section(".start")));

/*
 * Network thread
 */
int payload(void);

#endif /* _SAMPLE_H */
