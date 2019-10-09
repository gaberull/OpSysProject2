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
        if ((fd_in = open("pipe_in", O_RDONLY) < 0))
        {
            printf("exited on opening pipe_in");
            exit(-1);       // TODO: instructions say okay to terminate on failure
        }
        if ((fd_out = open("pipe_out", O_WRONLY) < 0))
        {
            printf("exited on opening pipe_out");
            exit(-1);
        }
        if (read(fd_in, &header, sizeof(HEADER) != sizeof(HEADER) ))
        {
            printf("exited on read of fd_in");
            exit(-1);
        }
        if (header.type == INIT_CONNECTION) // TODO: check all this possible scrap it
        {
            // Send back aknowledge
            header_out.type = ACKNOWLEDGE;
            header_out.len_message = 0;
            header_out.location = -1;
            header_out.len_buffer = -1;
            if (write(fd_out, &header_out, sizeof(HEADER)) != sizeof(HEADER))
            {
                printf("exited on write to fd_out");
                exit (-1);
            }
            
            // read in again to get name of fiole
            int size = header.len_message;      // size of message to follow
            char filename[30];
            if (read(fd_in, filename, size) != size)        // get filename from another read
            {
                printf("exited on read of fd_in");
                exit(-1);
            }
            // open file using storage.c function
            //storage = init_storage(filename);
            
            // send aknowledge again after opening file
            if(write(fd_out, &header_out, sizeof(HEADER)) != sizeof(HEADER))
            {
                printf("exited on write of fd_out");
                exit(-1);
            }
            
        }
        
        // We broke out because of a disconnection: clean up
        fprintf(stderr, "Closing connection\n");
        close(fd_in);
        close(fd_out);
        //close_storage(storage);
    }
    
    // Should never reach here
    return(0);
}
