#include "storage.h"
#include "comm.h"
#include "storage_common.h"
#include <string.h>

#define BUFSIZE 200

int main(int argc, char** argv)
{
    unsigned char buffer[BUFSIZE];
    HEADER header;
    HEADER header_out;
    int ret;
    STORAGE *storage;
    
    int fd_out;
    int fd_in;
    
    // Loop forever (break out with SHUTDOWN)
    while(1) {
        fprintf(stderr, "Waiting for connection with client...\n");
        
        // Open to_storage pipe
        
        
        
        
        
        
        // We broke out because of a disconnection: clean up
        fprintf(stderr, "Closing connection\n");
        close(fd_in);
        close(fd_out);
        close_storage(storage);
    }
    
    // Should never reach here
    return(0);
}
