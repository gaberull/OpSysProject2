/**
 API for remote storage.  Identical API as provided by the local storage implementation
 */
#include <string.h>
#include "storage_remote.h"
#include "storage_common.h"
#include "comm.h"

/**
 initialize the storage
 
 Open the two fds (in and out), wait for an init message, initialize the local storage
 */
STORAGE * init_storage(char * name)
{
    // Create space for the STORAGE object
    STORAGE *s = malloc(sizeof(STORAGE));
    
    // open pipe_in for writing from client to server
    if ((s->fd_to_storage = open("pipe_in", O_WRONLY) < 0)) // open pipe_in for writing?
        perror("failed to open pipe_in");
    // open pipe_out for reading from server to client
    if ((s->fd_from_storage = open("pipe_out", O_RDONLY) < 0))
        perror("failed to open pipe_out");
    
    // Append '\0' to name
    int length = (int)strlen(name) + 1;
    char namestr[length];
    namestr[length -1] = '\0';           // append null character to string
    strncpy(namestr, name, length-1);   // copy all but the null character b/c its already there
    
    // create and initialize HEADER instance to send INIT_CONNECTION message to server
    HEADER h;
    h.type = INIT_CONNECTION;
    h.len_message = length;
    h.location = -1;    // not applicable to INIT_CONNECTION
    h.len_buffer = -1;  // not applicable to INIT_CONNECTION
    
    // write() to send the HEADER if return value > 0 it was succesful
    if (write(s->fd_to_storage, &h, sizeof(h)) != sizeof(h))
    {
        perror("write (send to pipe_in) STORAGE failed: ");
        return NULL;
    }
    // send filename
    if (write(s->fd_to_storage, &namestr, length) != length)
    {
        perror("write (send to pipe_in) filename failed: ");
        return NULL;
    }
    
    // Do two reads() to get responses for both writes
    if (read(s->fd_from_storage, &h, sizeof(HEADER) != sizeof(HEADER)))
    {
        perror("read() from pipe_out has failed: ");
        return NULL;
    }
    if (read(s->fd_from_storage, &h, sizeof(HEADER) != sizeof(HEADER)))
    {
        perror("read() from pipe_out has failed: ");
        return NULL;
    }
    // All okay
    if (h.type == ACKNOWLEDGE)       // if correct response return s
    {
        return s;
    }
    else return NULL;

};

/**
 Shut down the connection
 
 Tell the server to shut down
 */
int close_storage(STORAGE *storage)
{
    // Create the shutdown message
    HEADER header;
    // initialize header values
    header.type = SHUTDOWN;
    header.len_message = 0;
    header.location = -1;
    header.len_buffer = -1;
    
    // send header to server
    if (write(storage->fd_to_storage, &header, sizeof(HEADER) != sizeof(HEADER) ))
    {
        perror("while sending shudown header: ");
        return -1;
    }
    // receive response from server
    if (read(storage->fd_from_storage, &header, sizeof(HEADER) != sizeof(HEADER) ))
    {
        perror("while receiving acknowledgement about shutdown: ");
        return -1;
    }
    // check that header type is aknowledge and close pipes
    if (header.type == ACKNOWLEDGE)
    {
        if (close(storage->fd_to_storage) < 0 )
        {
            perror("failed to close fd_to_storage: ");
            return -1;
        }
        if (close(storage->fd_from_storage) < 0 )
        {
            perror("failed to close fd_from_storage: ");
            return -1;
        }
    }
    // Free the storage struction
    free(storage);
    
    // Done
    return(0);
}

/**
 read bytes from the storage
 */
int get_bytes(STORAGE *storage, unsigned char *buf, int location, int len)
{
    
    
    
    // Success
    return(len);
};


/**
 Write bytes to the storae
 
 Send the write request message + the data
 */
int put_bytes(STORAGE *storage, unsigned char *buf, int location, int len)
{
    
    
    
    // Success
    return(len);
};

