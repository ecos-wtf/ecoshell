socket = 0x80d95970;
bind = 0x80d95c78;
listen = 0x80d96114;
accept = 0x80d95ae8;
connect = 0x80d95d5c;
send = 0x80d96684;
recv = 0x80d9642c;
close = 0x80d94898;
inet_ntop4 = 0x80d450ec;
inet_addr = 0x80d44cd0;
inet_aton = 0x80d44cfc;
malloc =  0x8002a554;
free = 0x8002a744;
bzero = 0x80d3fce8;
memset = 0x80d32ee8;
printf = 0x80bc9b38;
strncmp = 0x8064f7a0;
strcpy = 0x80d3f414;
strncpy = 0x80d3f6c8;
strerror = 0x80d419c4;
sleep = 0x80d92ccc;
cyg_error_get_errno_p = 0x80d41ce0;
cyg_thread_create = 0x80d34428;
cyg_thread_resume = 0x80d34520;
cyg_thread_exit = 0x80d34490;
cyg_fp_get = 0x80d93cf8;
cyg_fp_free = 0x80d93d60;
cyg_fd_assign = 0x80d93c20;
cyg_fd_free = 0x80d93c90;
BcmConsoleExecuteCurrentCommand = 0x8024e2ec;
BcmConsoleGetSingletonInstance = 0x8024e298;

SECTIONS
{
  . = 0x80810000;
  .start : { *(.start) }
  .text : { *(.text) }
  .data : { *(.data) }
  .rodata : { *(.rodata) }
}
