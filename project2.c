
// Austin "Gabe" Scott      project2
//  version 10/03/19

/* the main program maintains a 128-byte buffer in memory.
 * The user can interact with this buffer by placing integer,
 * floating point, character, string and hexadecimal values at
 * a specified location in the buffer. In addition, the user may
 * request that the values in the buffer be placed in a specified
 * location in the storage file, or that the buffer values can be read
 * from a specific location in the file.
 */
//#include <stdio.h>
#include <string.h>
//#include <ctype.h>
//#include <stdlib.h>
//#include <stdbool.h>
#include "storage_remote.h"

const char SEPARATORS[] = " \t\n";
char in_buffer[200];           // Input buffer from STDIN. command input buffer INBUFFSIZE = 200
char * args[100];               // the arguments split up
char ** arg;                 // working pointer that steps through the args
unsigned char charBuffer[128];             // the 128 byte buffer that holds all input numbers


void list()         // 'l' **   list contents of buffer
{
    for (int j=0; j<8; j++)
    {
        for (int i=0; i<16; ++i)    // print first 16 char array spots
        {
            printf("%02x ", charBuffer[j*16+i]);       // row major ordering
        }
        printf("\n");
    }
}

void zero()         // 'z'** zeroes out entire buffer
{
    for (int i=0; i<128; i++)       // iterate through whole buffer and set to zero
    {
        charBuffer[i] = 0;
    }
}

void writeByte(char** arg)   // 'b'** set the specified byte to input value (decimal)
{
    // TODO: error checking for number of args.
    int location = atoi(*arg);       // atoi() converts char buffer to int
    arg++;
    charBuffer[location] = atoi(*arg); // convert char array at *arg to int
}

void readByte(char** arg)           // 'B'** Read byte in decimal
{
    int location = atoi(*arg);
    printf("%d\n", charBuffer[location]);       // print number in decimal
}
void writeHex(char** arg)               // 'h' write hex number to buffer
{
    int location = atoi(*arg);
    arg++;                      // move arg to next number
    // call charToHex function before writing to buffer
    
    long int num = strtol(*arg, 0,16); // convert string to long, tell it the values in that string are in hexadecimal
    charBuffer[location] = num;
}

void writeChar(char** arg)          // 'c' TODO: check to make sure can't handle multiple bytes (2 chars) or if I need to handle that error. Also make sure it stores it in ascii. (currently printing ascii equivalent in hex)
{
    int location = atoi(*arg);
    arg++;
    charBuffer[location] = **arg;
}

void readHex(char** arg)            // 'H' prints off the value of the byte in hexadecimal
{
    int location = atoi(*arg);      // get location
    printf("%x\n", charBuffer[location]);   // print value off in hex
}

void readChar(char** arg)       // 'C' Read character value (glyph) TODO: See if need to copy it out like readInt
{
    int location = atoi(*arg);      // grab int location in buffer
    arg++;                  // increment counter to next argument
    printf("%c\n", charBuffer[location]); // print character value
}

void writeInt(char** arg)       // 'i'** write an integer value to buffer location. Stores over how ever many bytes are required.
{
    int location = atoi(*arg);  // get location
    arg++;
    //long int num = strtol(*arg, 0,16);
    int toStore = atoi(*arg);
    unsigned char* ptr = &charBuffer[location];     // create pointer to spot to store in
    memcpy(ptr, &toStore, sizeof(toStore));
}

void readInt(char** arg)        // 'I'**    reads in from buffer
{
    int location = atoi(*arg);      // get location
    arg++;
    
    int result=0;
    unsigned char* ptr = &charBuffer[location];
    memcpy(&result, ptr, sizeof(result));
    
    printf("%d\n", result);
}

void writeFloat(char** arg)         // 'f'  write floating point number to buffer
{
    int location = atoi(*arg);      // get location
    arg++;
    
    float num = atof(*arg);         // convert number to float
    unsigned char* ptr = &charBuffer[location];
    memcpy(ptr, &num, sizeof(num));
}

void readFloat(char** arg)          // 'F'  read floating point number from buffer
{
    int location = atoi(*arg);
    arg++;
    
    float result = 0.0;
    unsigned char* ptr = &charBuffer[location];
    memcpy(&result, ptr, sizeof(result));
    printf("%f\n", result);
}

void writeString(char** arg)            // 's' Write string to buffer TODO: test
{
    int location = atoi(*arg);
    arg++;
    
    //char str[50];
    int size = (int)strlen(*arg) + 1;       // add one to size of *arg for null terminator
    char str[size];
    sscanf(*arg, "%s", str);    // use sscanf to copy string over to a new string then copy it over
    int i = 0;
    while (1)
    {
        charBuffer[location] = str[i];
        location++;
        i++;
        if (str[i] == '\0')
        {
            charBuffer[location] = '\0';    // copy over null terminating character TODO: is this necessary? It is one longer and it is null by default
            break;      // copy over until we hit null terminating character
        }
    }
}

void readString(char** arg)         // 'S'  reads string from buffer
{
    int location = atoi(*arg);
    while (charBuffer[location] != '\0')
    {
        printf("%c", charBuffer[location]);
        location++;
    }
    printf("\n");
}

void writeToFile(char** arg, const char* filename)        // 'w'  writes to storage.bin file
{
    int offset = atoi(*arg);    // offset in the file to write to
    arg++;
    int len = atoi(*arg);       // get number of bytes to read 0 - 128
    STORAGE* st = init_storage(filename);
    if (put_bytes(st, charBuffer, offset, len) != len)
        perror("write function failed");
    close_storage(st);
}

void readFileToBuf(char** arg, const char* filename)          // 'r' Reads from the storage.bin file to charBuffer[]
{
    int offset = atoi(*arg);
    arg++;
    int len = atoi(*arg);           // number of bytes to read 0 - 128
    
    STORAGE* st = init_storage(filename);   // TODO: check to see if losing const is an issue or not
    if (get_bytes(st, charBuffer, offset, len) != len)
        perror("read to buffer froom file failed");
    close_storage(st);
}

int main(int argc, const char * argv[])
{
    // TODO: check command line arguments
    char filename[30];
    if (argc==1)// no additional arguments specify storage file as "storage.bin"
    {
        //STORAGE* st = init_storage("storage.bin");
        strcpy(filename, "storage.bin"); // TODO: do I need to add null terminator to name?
    }
    // TODO: if there is a second argument, it is the name of the file to write to
    else if (argc==2)
    {
        //STORAGE* st = init_storage(argv[1]);
        strcpy(filename, argv[1]);
    }
    while(fgets(in_buffer, 100, stdin)!= NULL) // while input string is coming in
    {
        arg = args;                 // arg is like an iterator through string of arguments (args)
        *arg++ = strtok(in_buffer,SEPARATORS);   // tokenize input
        while ((*arg++ = strtok(NULL,SEPARATORS)));
        arg = args;
        char option = **arg;
        arg++;
        
        switch(option)
        {
            case 'l' :  list();
                break;
            case 'z' :  zero();
                break;
            case 'b' : writeByte(arg);
                break;
            case 'B' : readByte(arg);
                break;
            case 'h' : writeHex(arg);
                break;
            case 'H' : readHex(arg);
                break;
            case 'c' : writeChar(arg);
                break;
            case 'C' : readChar(arg);
                break;
            case 'i' : writeInt(arg);
                break;
            case 'I' : readInt(arg);
                break;
            case 'f' : writeFloat(arg);
                break;
            case 'F' : readFloat(arg);
                break;
            case 's' : writeString(arg);
                break;
            case 'S' : readString(arg);
                break;
            case 'w' : writeToFile(arg, filename);
                break;
            case 'r' : readFileToBuf(arg, filename);
                break;
            default: fprintf(stderr, "incorrect input");
                break;
        }
    }
    return 0;
}
