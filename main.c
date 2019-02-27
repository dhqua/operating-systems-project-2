#include <stdio.h>

// Data structure to represent mem accesses read from file
    struct memReference 
    {
        unsigned memAddress;
        char accessType;
    };

int main(int argc, char const *argv[])
{

    // int loopCount = 1005001;
    // int loopCount = 1005000;
    int loopCount = 20;
    int frameNumber = atoi(argv[2]);
    int traceCount = 0;
    struct memReference traceRead[loopCount];
    struct memReference simRAM[frameNumber];
    printf("Size of struct: %lx\n", sizeof(struct memReference));

    // Capture policy and mode from command arguments
    const char* policy = argv[3];
    const char* mode = argv[4];

    FILE *fptr = fopen(argv[1], "r");
    unsigned addr;
    char rw;
    while(fscanf(fptr, "%x %c", &addr, &rw) != EOF )
    {
        traceRead[traceCount].memAddress = addr;
        traceRead[traceCount].accessType = rw;
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
