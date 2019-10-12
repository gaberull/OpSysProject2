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
        
        // Open pipes
        if ((fd_in = open("pipe_in", O_RDONLY) < 0))
        {
            perror("the following error occured while opening pipe_in: ");
            exit(-1);       // instructions say okay to terminate on failure
        }
        if ((fd_out = open("pipe_out", O_WRONLY) < 0))
        {
            perror("error while opening pipe_out: ");
            exit(-1);
        }
        // read() from pipe
        if (read(fd_in, &header, sizeof(HEADER) != sizeof(HEADER) ))
        {
            perror("error during read of fd_in: ");
            exit(-1);
        }
        // conditionals for all possible header types
        if (header.type == INIT_CONNECTION) // check that header type is INIT 
        {
            // prepare to Send back aknowledge
            header_out.type = ACKNOWLEDGE;
            header_out.len_message = 0;
            header_out.location = -1;
            header_out.len_buffer = -1;
            // write to fd_out to send header_out back across pipe
            if (write(fd_out, &header_out, sizeof(HEADER)) != sizeof(HEADER))
            {
                perror("error during write to fd_out: ");
                exit (-1);
            }
            
            // read in again to get name of file
            int size = header.len_message;      // size of message to follow
            char filename[size];    // create buffer to hold filename
            if (read(fd_in, filename, size) != size)
            {
                perror("error at read of fd_in: ");
                exit(-1);
            }
            // open (initialize) file using storage.c function
            storage = init_storage(filename); 
            
                // send back aknowledge after opening file
            if(write(fd_out, &header_out, sizeof(HEADER)) != sizeof(HEADER))
            {
                perror("error at write of fd_out: ");
                exit(-1);
            }
        }
        else if (header.type == READ_REQUEST)
        {
            
        }
        else if (header.type == WRITE_REQUEST)
        {
            
        }
        else    // header.type == SHUTDOWN
        {
            sleep(1);           // sleep for 1 second
            close(fd_in);       // close the named pipes
            close(fd_out);
            close_storage(storage);     // close the storage

        }

        // We broke out because of a disconnection: clean up
        fprintf(stderr, "Closing connection\n");
        close(fd_in);
        close(fd_out);
        close_storage(storage);
    }
    
    // Should never reach here
    return(0);
}
