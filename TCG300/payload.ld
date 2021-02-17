socket = 0x80e936d0;
bind = 0x80e939d8;
listen = 0x80e93e74;
accept = 0x80e93848;
connect = 0x80e93abc;
recv = 0x80e9418c;
malloc =  0x8001696c;
bzero = 0x80e3ae1c;
strncpy = 0x80e3a7fc;
memset = 0x80e2da20;
cyg_fp_get = 0x80e91690;
cyg_fp_free = 0x80e916f8;
cyg_fd_free = 0x80e91628;
free = 0x80016bf0;
strcpy = 0x80e3a548;
strncmp = 0x8062e8b4;
printf = 0x80e373bc;
send = 0x80e943e4;
BcmConsoleExecuteCurrentCommand = 0x802a9280;
cyg_fd_assign = 0x80e915b8;
BcmConsoleGetSingletonInstance = 0x802a922c;
cyg_thread_create = 0x80e2ef60;
cyg_thread_resume = 0x80e2f058;
cyg_thread_exit = 0x80e2efc8;
cyg_thread_get_info = 0x80e2f264;
cyg_thread_get_next = 0x80e2f19c;
inet_addr = 0x80e408b8;
inet_aton = 0x80e408e4;
cyg_error_get_errno_p = 0x80e3d844;
strerror = 0x80e3d528;
getservbyname = 0x80e9b110;
getsockname = 0x80e93c88;
close = 0x80e925f8;
sleep = 0x80e90608;
bsd_bind = 0x80e2acec;
tcp_usr_bind = 0x80e9d290;
udp_bind = 0x80e59da0;
bsd_socket = 0x80e2ac54;
setsockopt = 0x80e94404;
getpeername = 0x80e93ba0;
strlen = 0x80e3a670;
write = 0x80e925cc; 
inet_ntoa = 0x80e40b7c;
inet_ntop4 = 0x80e40cd4;

NandFlashRead = 0x803cf674;
NandFlashWrite = 0x803cf1ec;

SECTIONS
{
  . = 0x80810000;
  .start : { *(.start) }
  .text : { *(.text) }
  .data : { *(.data) }
  .rodata : { *(.rodata) }
}
