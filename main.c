// Qua'on Thomas and Cedric Nicolas 
//
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

    int traceCount;
    int diskReadCount ;
    int diskWriteCount;
    int lruTicket;;
    LinkedList* RSSOne;
    LinkedList* RSSTwo;

    LinkedList * cleanList;
    LinkedList * dirtyList;

int main(int argc, char const *argv[])
{

     int loopCount = 1005001;
//     int loopCount = 200;;
    // int loopCount = 1005000;
    //int loopCount = 20;
    int pageNum = 1048576;
    int frameNumber = atoi(argv[2]);
     traceCount = 0;
     diskReadCount = 0;
     diskWriteCount =0;
     lruTicket = 0;
    int RSS = frameNumber / 2 - 1;
    cleanList = (LinkedList*)malloc(sizeof(memRef) * (frameNumber / 2) + 1);  
    dirtyList = (LinkedList*)malloc(sizeof(memRef) * (frameNumber / 2) + 1);
    // Simulated Structures
    // Trace File
    memRef traceRead[loopCount];
    // RAM
    memRef simRAM[frameNumber];
     
    // Page Table
    // 32 bit address / 4kb page size = 2^20 pages = 1048576
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
        
    if( strcmp(policy, "fifo") == 0)
    {
        LinkedList * queue = create();

        int maxRamSize = sizeof(memRef) * frameNumber;
        // Loop through the memtrace
        int i =0;
        unsigned tempAddy;
        for( ; i < loopCount; i++)
        {
            tempAddy = traceRead[i].memAddress;

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

                    //if(traceRead[i].accessType == 'W')
                      //  diskWriteCount++;
                    
                }
            // Check if it is in RAM
            int j =0;
            for( ; j < frameNumber; j++)
            {
                if( simRAM[j].memAddress == traceRead[i].memAddress)
                    inRamFlag = 1;
            }

            // Continues trace read because the current address is alread in RAM
            if(inRamFlag)
                continue; 

            // RAM is full
            if(queue->size >= frameNumber)
            //if( ramState == maxRamSize)
            {
                
               // Add current Addy to the queue
               enqueue(queue, traceRead[i].memAddress);
               // Replace something in ram with current address using the FIFO function
               // Fifo returns the page number of the page to be replace in RAM
               simRAM[fifoPolicy(queue, &simRAM,frameNumber)] = traceRead[i];
               if(strcmp(mode,"debug") == 0 )
                printf("FIFO was called!!!!!\n");
            }
            else
            {
                enqueue(queue, traceRead[i].memAddress);
                // Check if already in page table
                // Right shifting by 12 to get a value between 1 and 2^20 which represents the page number

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

    if(strcmp(policy,"lru") == 0)
    {
                

        int maxRamSize = sizeof(memRef) * frameNumber;
        // Loop through the memtrace
        int i =0;
        unsigned tempAddy;
        for( ; i < loopCount; i++)
        {
            tempAddy = traceRead[i].memAddress;

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

                    //if(traceRead[i].accessType == 'W')
                      //  diskWriteCount++;
                    
                }
 
            // Check if it is in RAM
            int j =0;
            for( ; j < frameNumber; j++)
            {
                if( simRAM[j].memAddress == traceRead[i].memAddress)
                    inRamFlag = 1;
            }

            // Continues trace read because the current address is alread in RAM
            if(inRamFlag)
                continue; 


            // RAM is full
            if(i >= frameNumber)
            //if( ramState == maxRamSize)
            {
                
               // Replace something in ram with current address using the FIFO function
               // Fifo returns the page number of the page to be replace in RAM
               traceRead[i].lruRank = lruTicket++;
               simRAM[lru( &simRAM,frameNumber)] = traceRead[i];
               if(strcmp(mode,"debug") == 0 )
                printf("LRU was called!!!!!\n");
            }
            else
            {
                // Right shifting by 12 to get a value between 1 and 2^20 which represents the page number
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

    if(strcmp(mode, "vms") == 0)
    {
        int i = 0;
        for(; i < traceCount; i++){
    
            
           int processID = (int)(traceRead[i].memAddress >> 28 ) ;
           // rssSelection 0 = all other process, 1 = process that handles addresses that begin with 3
           int rssSelection = processID == 3;
            //rssSelection 1 = 0 -> RSS, rssSelection 1 = RSS -> frameNumber
            //We split the RAM into two and gave one half to each process
            int endOfArray;
            int isRAMFull = 0;
           if(rssSelection)
            {
              // Checks is the RAM for process two is full
              if(RSSTwo->size >= RSS)
                isRAMFull = 1;
              endOfArray = frameNumber;
            } else {
              // Checks is the RAM for process two is full
              if(RSSOne->size >= RSS)
                isRAMFull = 1;
                endOfArray = RSS;
            }

            
           // determines startpointh
           int j = rssSelection * RSS;
           int inRam = 0;
           // Check if in RAM
           for( ; j < endOfArray; j++)
            {
                if(simRAM[j].memAddress == traceRead[i].memAddress)
                {
                    inRam = 1;
                }   
            }
            if(inRam)
            {
                continue;   
            } 
          

            if(!isRAMFull)
            {
                
               if( traceRead[i].accessType == 'R')
                    diskReadCount++;

               LinkedList * currentRam = RSSTwo;
               int emptySpot = 0;
               if(rssSelection == 1)
               {
                currentRam = RSSTwo;
                emptySpot = RSS;
               }
                else
               {
               currentRam = RSSOne;
               }
                
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
               // Continues to the next iteration because it was added to 
               continue;
                
            }
          
           int currentPageNum = (int)(traceRead[i].memAddress >> 12 ) ;
           if( pageTable[currentPageNum] == 0 )
            {
                // Checks if page is in clean list of dirty list
                int inRSSList = 0;
               Node * cursorNode = cleanList->head;
                while(cursorNode != NULL)
                {
                    if(cursorNode->memAddress == traceRead[i].memAddress)
                        inRSSList = 1;
                    cursorNode = cursorNode->next;
                } 

                cursorNode = dirtyList->head;
                while(cursorNode != NULL)
                {
                    if(cursorNode->memAddress == traceRead[i].memAddress)
                        inRSSList = 1;
                    cursorNode = cursorNode->next;
                } 


               
                
                if(inRSSList)
                {
                    //TODO: FifoPolicy               
                }
                else
                {
                    diskReadCount++;
                }
            
                // 0 = page not in table, 1 = page exists 
                pageTable[currentPageNum] = 1; 
                if(traceRead[i].accessType == 'R')
                    diskReadCount++;

                //if(traceRead[i].accessType == 'W')
                  //  diskWriteCount++;
                
            }            
            
        }
    }

    if(strcmp(mode,"quiet") == 0 )
        {
            printf("total memory frames: %d\n", frameNumber);
            printf("events in trace: %d\n", traceCount);
            printf("total disk reads: %d\n", diskReadCount);
            printf("total disk writes: %d\n", diskWriteCount);
        }

    fclose(fptr);
    return 0;
}

// Function that returns the page number that should be replaced in RAM
// checkAddress - cna
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

int lru(memRef * simulatedRam, int frameNum){
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


