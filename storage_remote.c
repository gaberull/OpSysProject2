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
    
    
    
    
    // All okay
    return s;
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

