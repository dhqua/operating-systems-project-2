// Project 2 - Operating Systems
// Due 4 March 2019
// Run File: memsim <tracefile> <nframes> <lru|fifo|vms> <debug|quiet>
#include <stdio.h>
#include "LinkedList.h"
#include <string.h>
#include <stdlib.h>

// Data structure to represent mem accesses read from file
typedef struct memReference{
    unsigned memAddress;
    char accessType;
    int lruRank;
} memRef;

//Global Variables
int traceCount;
int diskReadCount ;
int diskWriteCount;
int lruTicket;

//Represents the RAM in the VMS case
LinkedList* RSSOne;
LinkedList* RSSTwo;

//Extra lists for use with VMS
LinkedList * cleanList;
LinkedList * dirtyList;

int main(int argc, char const *argv[])
{

    int loopCount = 1005001;
    // 32 bit address / 4kb page size = 2^20 pages = 1048576
    int pageNum = 1048576;
    int frameNumber = atoi(argv[2]);
    //Initialize global variables
    traceCount = 0;
    diskReadCount = 0;
    diskWriteCount =0;
    lruTicket = 0;
    int RSS = frameNumber / 2;

    //Initialize Data Structures
    RSSOne = create();
    RSSTwo = create();
    cleanList = create(); 
    dirtyList = create(); 
    // Trace File
    memRef traceRead[loopCount];
    // RAM
    memRef simRAM[frameNumber];
    // Page Table
    unsigned pageTable[pageNum];

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
        if( strcmp(mode, "debug") == 0)
            printf("%d: Address: %x Access Type: %c\n",traceCount,traceRead[traceCount].memAddress,traceRead[traceCount].accessType);
        traceCount++;
    }
        
    // Handles the fifo replacement policy
    if( strcmp(policy, "fifo") == 0)
    {
        //Initializes queue
        LinkedList * queue = create();

        // Loop through the memtrace
        int i =0;
        for( ; i < loopCount; i++)
        {
            int inRamFlag = 0;
            // Calculates the page number of the current address by taking the first 5 of hex address of length 8
            int currentPageNum = (int)(traceRead[i].memAddress >> 12 ) ;

            // Adds current page number to the page table and records if it requires a disk read or write
            if( pageTable[currentPageNum] == 0 )
            {
                // 0 = page not in table, 1 = page exists 
                pageTable[currentPageNum] = 1; 
                if(traceRead[i].accessType == 'R')
                    diskReadCount++;
            }
            // Check if current page is in RAM
            int j =0;
            for( ; j < frameNumber; j++)
            {
                if( (simRAM[j].memAddress >> 12) == currentPageNum)
                    inRamFlag = 1;
            }

            // Skip to next address in trace read because the current address is alread in RAM
            if(inRamFlag)
                continue; 

            // Case where RAM is full
            if(queue->size >= frameNumber)
            {
                // Add current Addy to the queue
                enqueue(queue, traceRead[i].memAddress);
                // Replace something in ram with current address using the FIFO function

                // Fifo returns the index within ram of the page to be replaced
                simRAM[fifoPolicy(queue, &simRAM,frameNumber)] = traceRead[i];

                if(strcmp(mode,"debug") == 0 )
                    printf("FIFO was called!!!!!\n");
            }
            // if RAM is not full,place it in an empty spot in RAM
            else
            {
                enqueue(queue, traceRead[i].memAddress);
                int loc =  0;
                for( ; loc < frameNumber; loc++)
                {
                    // Current slot in RAM
                    if(simRAM[loc].memAddress == 0 )
                    {
                        simRAM[loc].memAddress = traceRead[i].memAddress;
                        simRAM[loc].accessType = traceRead[i].accessType;;
                        break;
                    }
                }
            }
        }
    }

    // Handles the lru policy
    if(strcmp(policy,"lru") == 0)
    {
        // Loop through the memtrace
        int i =0;
        for( ; i < loopCount; i++)
        {
            int inRamFlag = 0;
            // Calculates the page number of the current address by taking the first 5 of hex address of length 8
            int currentPageNum = (int)(traceRead[i].memAddress >> 12 ) ;
            // Adds address to the page table and records if it requires a disk read or write
            if( pageTable[currentPageNum] == 0 )
            {
                // 0 = page not in table, 1 = page exists 
                pageTable[currentPageNum] = 1; 
                if(traceRead[i].accessType == 'R')
                    diskReadCount++;
            }

            // Check if page is in RAM
            int j =0;
            for( ; j < frameNumber; j++)
            {
                if( (simRAM[j].memAddress >> 12) == currentPageNum)
                    inRamFlag = 1;
            }

            // Updtes the LRU ticket then continues trace read because the current address is alread in RAM
            if(inRamFlag)
            {
                int z =0;
                for( ; z < frameNumber; z++)
                {
                    if((simRAM[z].memAddress >> 12 ) == currentPageNum)
                    {
                        simRAM[z].lruRank = lruTicket++;
                    }
                }
                continue; 
            }

            // RAM is full
            if(i >= frameNumber)
            {

                // Replace something in ram with current address using the lru function
                // lru returns the page number of the page to be replace in RAM
                traceRead[i].lruRank = lruTicket++;
                simRAM[lru( &simRAM,frameNumber)] = traceRead[i];

                if(strcmp(mode,"debug") == 0 )
                    printf("LRU was called!!!!!\n");
            }
            else
            {
                int loc =  0;
                for( ; loc < frameNumber; loc++)
                {
                    // Current slot in RAM
                    if(simRAM[loc].memAddress  == 0 )
                    {
                        simRAM[loc].memAddress = traceRead[i].memAddress;
                        simRAM[loc].accessType = traceRead[i].accessType;;
                        simRAM[loc].lruRank = lruTicket++;
                        break;
                    }
                }
            }

        }
    }

    //Handles the VMS policy
    if(strcmp(policy, "vms") == 0)
    {
        // Loop through memtrace values
        int i = 0;
        for(; i < traceCount; i++)
        {

            // This line is used to capture the first value of the hex string to determne which process will handle the address
            int processID = (int)(traceRead[i].memAddress >> 28 ) ;

            // rssSelection 0 = all other process, 1 = process that handles addresses that begin with 3
            int rssSelection = processID == 3;

            //We split the RAM into two and gave one half to each process
            // RSS is the size of each queue that is designated a one half of the RAM.RSS = frameNumber / 2
            //rssSelection 1 = 0 -> RSS, rssSelection 1 = RSS -> frameNumber
            int endOfArray;
            int isRAMFull = 0;
            if(rssSelection)
            {
                // Checks if the queue for process two is full
                if(RSSTwo->size >= RSS  )
                    isRAMFull = 1;
                endOfArray = frameNumber;
            } 
            else {
                // Checks if the queue for process two is full
                if(RSSOne->size >= RSS)
                    isRAMFull = 1;
                endOfArray = RSS;
            }


            // determines startpoint of the loop depending on which replacement queue has been selected
            int j = rssSelection * RSS;
           
            int inRam = 0;
            // Checks if page is in RAM
            for( ; j < endOfArray; j++)
            {
                if((simRAM[j].memAddress >> 12) == traceRead[i].memAddress >> 12)
                {
                    inRam = 1;
                }   
            }
            
            // If the page is in ram continue to the next address in the trace file
            if(inRam)
            {

                if( strcmp(mode, "debug") == 0)
                    printf("Address is in RAM\n");
                continue;   
            } 

            // Temp variable to point to the selected RAM based on the flag
            LinkedList * currentRam = RSSTwo;
            int emptySpot = 0;
            if(rssSelection == 1)
            {
                currentRam = RSSTwo;
                //Sets start point to be the second half of the RAM
                emptySpot = RSS;
            }
            else
            {
                currentRam = RSSOne;
            }

            if(!isRAMFull)
            {

                if( strcmp(mode, "debug") == 0)
                    printf("Ram is NOT FULL\n");
                // We read from Disk becuase at this point the page does not exist in the RAM or in a Queue
                if( traceRead[i].accessType == 'R')
                    diskReadCount++;


                // Place the page in an empty spot within RAM
                for( ; emptySpot < endOfArray; emptySpot++)
                {
                    if( simRAM[emptySpot].memAddress == 0)
                    {
                        simRAM[emptySpot] = traceRead[i];
                        // Adds to the RSS queue
                        enqueue(currentRam, traceRead[i].memAddress);

                        // Adds to the page table
                        int currentPageNum = (int)(traceRead[i].memAddress >> 12 ) ;
                        pageTable[currentPageNum] = 1;
                        break;
                    }                 
                }
                // Continues to the next iteration because it was added to the RAM
                continue;

            }

            //CASE Where the RAM is Full

            int currentPageNum = (int)(traceRead[i].memAddress >> 12 ) ;
            int inPageTable = 1;
            
            //Checks if the page is in the page table
            if( pageTable[currentPageNum] == 0 )
            {
                // 0 = page not in table, 1 = page exists 
                pageTable[currentPageNum] = 1; 
                inPageTable = 0;

                if(traceRead[i].accessType == 'R')
                    diskReadCount++;

                //if(traceRead[i].accessType == 'W')
                //  diskWriteCount++;

            }            

            // Checks if page is in clean list or dirty list
            int inRSSList = 0;
            Node * cursorNode = cleanList->head;
            while(cursorNode != NULL)
            {
                if((cursorNode->memAddress >> 12) == (traceRead[i].memAddress >> 12))
                    inRSSList = 1;
                cursorNode = cursorNode->next;
            } 

            cursorNode = dirtyList->head;
            while(cursorNode != NULL)
            {
                if((cursorNode->memAddress >> 12) == (traceRead[i].memAddress >> 12))
                    inRSSList = 1;
                cursorNode = cursorNode->next;
            } 


            // Case where the page in in either RSS queue
            if(!inRSSList)
            {
                // Replace something in ram with current address using the FIFO function
                // Fifo returns the page number of the page to be replace in RAM
                int toBeReplaced = fifoPolicy(currentRam, &simRAM,RSS);
                memRef temp = simRAM[toBeReplaced];               
                int isDirty = 0; 
                if( temp.accessType == 'W')
                    isDirty = 1;
                // checks if clean list is empty, is so, then queue it
                if(!isDirty && cleanList->size < RSS)
                {
                    enqueue(cleanList, simRAM[toBeReplaced].memAddress);
                }
                // checks if dirty list is empty, is so, then queue it
                else if(isDirty && dirtyList->size < RSS)
                {
                    enqueue(dirtyList, simRAM[toBeReplaced].memAddress);
                }
                else if(!isDirty)
                {
                    enqueue(cleanList,temp.memAddress);
                    dequeue(cleanList);
                }
                else
                {
                    enqueue(dirtyList,temp.memAddress);
                    dequeue(dirtyList);   
                    diskWriteCount++;
                } 
                simRAM[toBeReplaced] = traceRead[i];
                enqueue(currentRam, simRAM[toBeReplaced].memAddress);
            }
            else if(!inPageTable && !inRam && !inRSSList && traceRead[i].accessType == 'R')
            {
                enqueue(cleanList, traceRead[i].memAddress);
                diskReadCount++;
            }
            else if(!inPageTable && !inRam && !inRSSList && traceRead[i].accessType == 'W')
            {
                enqueue(dirtyList, traceRead[i].memAddress);
                diskWriteCount++;
            }


        }
    }

    // Print project stats
    printf("total memory frames: %d\n", frameNumber);
    printf("events in trace: %d\n", traceCount);
    printf("total disk reads: %d\n", diskReadCount);
    printf("total disk writes: %d\n", diskWriteCount);

    fclose(fptr);
    return 0;
}

// Function that returns the page number that should be replaced in RAM based on the fifo policy
// visitedQueue - the queue that will determine which address is being replace
// simulatedRam - the data structure that is representing RAM in memory
// frameNum - the max size of the ram being passed in
int fifoPolicy(LinkedList * visitedQueue, memRef * simulatedRam, int frameNum)
{
    unsigned addressToReplace = dequeue(visitedQueue); 
    // Right shifts the adress by 12, in order to get the first 5 chars of the length 8 hex adress
    // In other words this gets the page number by getting the first 20 bits of the 32 bit address
    unsigned pageNumber = addressToReplace >> 12;    

    // Iterate through the RAM and find the position of the pageNumber within RAM
    int ramPageNum = 0;
    for( ; ramPageNum < frameNum; ramPageNum++)
    {
        // If the current contents of the this location in the ram matches the pagenumber that needs to be replaced
        // then return the current location within the RAM
        if( (simulatedRam[ramPageNum].memAddress >> 12) == pageNumber)
        {
            if(simulatedRam[ramPageNum].accessType == 'W' )
                diskWriteCount++;
            return ramPageNum;
        }
    }
}

// Function that returns the page number that should be replaced in RAM based on the lru policy
// simulatedRam - the data structure that is representing RAM in memory
// frameNum - the max size of the ram being passed in
int lru(memRef * simulatedRam, int frameNum){
    // Finds the minimum structure in the RAM based on its lruTicket which representes the last time they were accessed
    int minIndex = 0;
    int i = 0;
    for( ; i < frameNum; i++)
    {
        if( simulatedRam[i].lruRank < simulatedRam[minIndex].lruRank )
            minIndex = i;   
    }

    if(simulatedRam[minIndex].accessType == 'W')
        diskWriteCount++;
    return minIndex;

}

