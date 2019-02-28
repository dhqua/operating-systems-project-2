#include <stdio.h>
#include "linkedList.c"


// Data structure to represent mem accesses read from file
    struct memReference 
    {
        unsigned memAddress;
        char accessType;
    };

int main(int argc, char const *argv[])
{

     int loopCount = 1005001;
    // int loopCount = 1005000;
    //int loopCount = 20;
    int frameNumber = atoi(argv[2]);
    int traceCount = 0;
    
    // Simulated Structures
    // Trace File
    struct memReference traceRead[loopCount];
    // RAM
    struct memReference simRAM[frameNumber];
    // Page Table
    // 32 bit address / 4kb page size = 2^20 pages = 1048576
    unsigned pageTable[1048576];

    printf("Size of struct: %lx\n", sizeof(struct memReference));

    // Capture policy and mode from command arguments
    const char* policy = argv[3];
    const char* mode = argv[4];

    // Reads the contents of the memtrace file into our memtrace structure
    FILE *fptr = fopen(argv[1], "r");
    unsigned addr;
    char rw;
    while(fscanf(fptr, "%x %c", &addr, &rw) != EOF )
    {
        traceRead[traceCount].memAddress = addr;
        traceRead[traceCount].accessType = rw;
        if( mode == "debug")
            printf("%d: Address: %x Access Type: %c\n",traceCount,traceRead[traceCount].memAddress,traceRead[traceCount].accessType);
        traceCount++;
    }
        

    printf("Address: %08x Access Type: %c\n" ,traceRead[4].memAddress,traceRead[4].accessType );
    // printf("Address: %08x Access Type: %c\n" ,traceRead[100].memAddress,traceRead[100].accessType );
    // printf("Address: %08x Access Type: %c\n" ,traceRead[223].memAddress,traceRead[223].accessType );
    // printf("Address: %08x Access Type: %c\n" ,traceRead[78846].memAddress,traceRead[78846].accessType );

    fclose(fptr);
    return 0;
}

// Function that returns the page number that should be replaced in RAM
// checkAddress - cna
unsigned fifoPolicy(LinkedList &visitedQueue, struct memReference &simulatedRam)
{
    unsigned addressToReplace = dequeue(visitedQueue); 
    // Right shifts the adress by 12, in order to get the first 5 chars of the length 8 hex adress
    // In other words this gets the page number by getting the first 20 bits of the 32 bit address
    unsigned pageNumber = addressToReplace >> 12;    
    
    // Iterate through the RAM and find the position of the pageNumber within RAM
    int ramPageNum = 0;
    int ramLength = sizeof(simulatedRam) / sizeof(struct memReference);
    for( : ramPageNum < ramLength, ramPageNum++)
    {
        // If the current contents of the this location in the ram matches the pagenumber that needs to be replaced
        // then return the current location within the RAM
        if( simulatedRam[ramPageNum] == pageNumber)
            return ramPageNum;
    }
}
