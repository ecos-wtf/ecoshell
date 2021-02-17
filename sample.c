#include "external.h"
#include "sample.h"

#define PAYLOAD_PRIORITY     11
#define PAYLOAD_STACKSIZE  0x2000
/* Globals */

int __start(void)
{
    printf("__start called\n");
    int payload_handle;
    char* payload_stack = malloc(PAYLOAD_STACKSIZE * sizeof(char));
    static cyg_thread payload_thread;
    int id = 0;
    int res;
    printf("calling cyg_thread_create\n");
    cyg_thread_create(
        PAYLOAD_PRIORITY,
        &payload,
        0,
        "payload",
        payload_stack,
        PAYLOAD_STACKSIZE,
        &payload_handle,
        &payload_thread
    );
    printf("calling cyg_thread_resume\n");
    cyg_thread_resume(payload_handle);
    return 0;
}

int payload(void){
    
    while(1){
        printf("\n[+] I'm a sample thread, running in the background.\n");
        sleep(5);
    }
}
