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
    namestr[length -1] = '0';           // append null character to string
    strncpy(namestr, name, length-1);   // copy all but the null character b/c its already there
    //sscanf(name, "%s", namestr);    // should store a null character in addition to the string
    
    // create and initialize HEADER instance to send INIT_CONNECTION message to server
    HEADER h;
    //STORAGE st;
    h.type = INIT_CONNECTION;
    h.len_message = length;
    h.location = -1;    // not applicable to INIT
    h.len_buffer = -1;  // not applicable to INIT
    
    // write() to send the HEADER if return value > 0 it was succesful
    if (write(s->fd_to_storage, &h, sizeof(h)) != sizeof(h))
    {
        perror("write (send to pipe_in) STORAGE failed");
        return NULL;
    }
    // send filename
    if (write(s->fd_to_storage, &namestr, length) != length)
    {
        perror("write (send to pipe_in) filename failed");
        return NULL;
    }
    
    // Do two reads() to get responses for both writes
    if (read(s->fd_from_storage, &h, sizeof(HEADER) != sizeof(HEADER)))
    {
        perror("read() from pipe_out has failed");
        return NULL;
    }
    if (read(s->fd_from_storage, &h, sizeof(HEADER) != sizeof(HEADER)))
    {
        perror("read() from pipe_out has failed");
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

