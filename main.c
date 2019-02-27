#include <stdio.h>
int main(int argc, char const *argv[])
{
    int loopCount = 1000000;
    int frameNumber = atoi(argv[2]);
    printf("Loop count: %d\n", frameNumber);
    FILE *fptr = fopen(argv[1], "r");
    unsigned addr;
    char rw;
    for(int i = 0; i < loopCount; i++){
        fscanf(fptr, "%x %c", &addr, &rw);
    }
    fclose(fptr);
    return 0;
}