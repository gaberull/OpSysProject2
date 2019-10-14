// Austin "Gabe" Scott  Project2
#include "storage.h"
#include "comm.h"
#include "storage_common.h"
#include <string.h>
#include "stdbool.h"

#define BUFSIZE 200

// allows us to read from the pipe. Trying to interpret what it has a full header object. Want to continue reading until get at least header bytes
ssize_t getAll(int fd, void * buffer, ssize_t size)
{
    bool keepgoing = true;
    char* temp = buffer;
    ssize_t totalBytesRead = 0;
    while(keepgoing)
    {
        ssize_t bytes = read(fd, temp, size - totalBytesRead);
        if (bytes < 0)
        {
            perror("error: ");
            exit (-1);
        }
        temp += bytes;
        totalBytesRead += bytes;
        keepgoing = (size != totalBytesRead);
    }
    return totalBytesRead;
}

int main(int argc, char** argv)
{
    
    unsigned char buffer[BUFSIZE];
    HEADER header;
    header.len_buffer = 0;
    header.len_message = 0;
    header.location = 0;
    header.type = SHUTDOWN;
    HEADER header_out;
    int ret;
    STORAGE *storage;
    
    int fd_out;
    int fd_in;
    
    // TESTING SERVER
    //storage = init_storage("storage.bin");
    
    
    // Loop forever (break out with SHUTDOWN)
    while(1) {
        fprintf(stderr, "Waiting for connection with client...\n");
        
        
        // Open pipes
        if ((fd_in = open(PIPE_NAME_TO_STORAGE, O_RDONLY)) < 0)
        {
            perror("the following error occured while opening pipe_in: ");
            exit(-1);       // instructions say okay to terminate on failure
        }
        if ((fd_out = open(PIPE_NAME_FROM_STORAGE, O_WRONLY)) < 0)
        {
            perror("error while opening pipe_out: ");
            exit(-1);
        }
        while (getAll(fd_in, &header, sizeof(HEADER)))
        {
            // read() from pipe


            // conditionals for all possible header types
            if (header.type == INIT_CONNECTION) // check that header type is INIT
            {
                //printf("received INIT_CONN\n");
                // prepare to Send back aknowledge
                header_out.type = ACKNOWLEDGE;
                header_out.len_message = 0;
                header_out.location = -1;
                header_out.len_buffer = -1;
                                                /*      //removing one write
                // write to fd_out to send header_out back across pipe
                if (write(fd_out, &header_out, sizeof(HEADER)) != sizeof(HEADER))
                {
                    perror("error during write to fd_out: ");
                    exit (-1);
                }                               */
                
                // read in again to get name of file
                int size = header.len_message;      // size of message to follow
                char filename[size];    // create buffer to hold filename
                int test = (int)read(fd_in, filename, size);
                if (test != size)
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
                //printf("received READ_REQ\n");
                // client is requesting DATA.
                // set return header message
                header_out.type = DATA;
                header_out.len_message = 0;
                header_out.location = header.location;
                //header_out.len_buffer = header.len_buffer;
                
                // get_bytes() from storage.c
                ret = get_bytes(storage, buffer, header.location, header.len_buffer);
                // TODO: if ret == 0, EOF, if -1, error I think. maybe send this back in header message
                header_out.len_buffer = header.len_buffer; //ret;
                header_out.len_message = header.len_buffer; //ret;
                // send back the bytes read to the buffer
                // ret will have the number of bytes retreived or 0 for EOF
                write(fd_out, &header_out, sizeof(HEADER));
                write(fd_out, buffer, ret);     //TODO: maybe add error check
            }
            else if (header.type == WRITE_REQUEST)
            {
                //printf("RECEIVED WRITE REQ\n");
                // set up header to be returned
                header_out.type = ACKNOWLEDGE;
                header_out.len_message = 0;
                header_out.location = -1;
                //header_out.len_buffer = -1;
                
                // get the buffer from the remote to be written
                int asize = (int)read(fd_in, buffer, header.len_buffer);
                if (asize != header.len_buffer)
                {
                    perror("error while reading in buffer: ");
                }
                
                ret = put_bytes(storage, buffer, header.location, header.len_buffer);
                // set len_buffer to ret - will be num bytes written or 0 for EOF
                header_out.len_buffer = header.len_buffer; //ret;
                
                // write to send back to remote
                int value = (int)write(fd_out, &header_out, sizeof(HEADER));
                if (value != sizeof(HEADER))
                {
                    perror("incorrect size written during WRITE_REQ: ");
                }
                // clear the buffer
                for (int i=0; i < 128; i++)
                {
                    buffer[i] = '\0';
                }
            }
            else    // header.type == SHUTDOWN
            {
                //printf("received SHUTDOWN\n");
                // send acknowledge message
                header_out.type = ACKNOWLEDGE;
                header_out.len_message = 0;
                header_out.location = -1;
                header_out.len_buffer = -1;
                write(fd_out, &header_out, sizeof(HEADER));
                
                sleep(1);           // sleep for 1 second
                        /*
                close(fd_in);       // close the named pipes
                close(fd_out);
                close_storage(storage);     // close the storage
                         */
                break;
            }
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
