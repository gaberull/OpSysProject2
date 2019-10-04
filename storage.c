#include "storage.h"

/* opens the file for reading and writing, creating it if it doesn't exist. If
 * successful, returns dynamically created STORAGE obj. Else returns NULL
 */
STORAGE * init_storage(char * name)
{
    STORAGE *st = malloc(sizeof(STORAGE)); //  allocate storage on heap
    st->fd = open(name, O_RDWR | O_CREAT, S_IRWXU);//TODO check these flags, mode
    if (st->fd == -1)  // open file was unsuccessful
    {
        perror("failed to open");
        return NULL;
    }
    else // open file successful
    {
        return st;
    }
}

/* closes the previously opened file. Frees dynamically allocated structure
 * Returns 0 on success and -1 on failure
 */
int close_storage(STORAGE *storage)
{
    int ret = close(storage->fd);
    free(storage);
    return ret;
}
/*
 * reads len num of bytes into buf starting with offset location. Returns
 * number of bytes read. 0 if at EOF. or a negative num on error.
 */
int get_bytes(STORAGE *storage, unsigned char *buf, int location, int len)      // FIXME: broken
{
    //
    off_t off = lseek(storage->fd, location, SEEK_SET);
    if (off == -1) perror("error has occured during lseek()");
    ssize_t ret = read(storage->fd, buf, len); //TA said it is fine to use len directly - the size of a char is one byte he said.
    if (ret < 0) perror ("Read error");
    if (ret == 0) printf("EOF\n");
    // TODO: consider using int (with cast?) like his examples
    return ret;
}

/*
 * Writes the first len bytes in buf to the storage file, starting at location
 * Returns the number of bytes written, or a negative number on error
 */
int put_bytes(STORAGE *storage, unsigned char *buf, int location, int len)
{
    off_t off = lseek(storage->fd, location, SEEK_SET);  // set offset to location from parameter
    if (off == -1) perror("error has occurred during lseek() operation");
    int ret = write(storage->fd, buf, len); //TA said to use len directly - the size of a char is 1 byte
    if (ret == -1)
    {
        perror("error has occured during write() operation");
        return -1;
    }
    if (ret == 0) printf("EOF\n");
    return ret;
}
