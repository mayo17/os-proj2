#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void fifo(int nframes, int debug, FILE* file)
    {
        int count = 0;
        char addr[8];
        char rw;
        char page[nframes][8];
        int dirty[nframes];
        int i = 0;
        int j = 0;
        int pos = 0;
        int exists = 0;
        int pageFault = 0;
        int writes = 0;
        while(fscanf(file, "%s %c", &addr, &rw) != EOF)
        {
            count++;
            if(i < nframes)
            {
                exists = 0;
                for(j = 0; j < nframes; j++)
                {
                    if(strncmp(page[j], addr, 5) == 0)
                    {
                        exists = 1;
                        break;
                    }
                    else
                    {
                        exists = 0;
                    }
                }
                if(exists == 0)
                {
                    strncpy(page[i], addr, 5);
                    i++;
                    pageFault++;
                }
            }
            else
            {
                for(j = 0; j < nframes; j++)
                {
                    if(strncmp(page[j], addr, 5) == 0)
                    {
                        exists = 1;
                        if(rw == 'W')
                        {
                            dirty[j] = 1;
                        }
                        break;
                    }
                    else
                    {
                        exists = 0;
                    }
                }
                if(exists == 0)
                {
                    //If page being replaced had a write instruction increment the writes counter
                    if(dirty[0] == 1)
                    {
                        writes++;
                    }
                    for(j = 0; j < nframes-1; j++)
                    {
                        strcpy(page[j], page[j+1]);
                        dirty[j] = dirty[j+1];
                    }
                    strncpy(page[nframes-1], addr, 5);
                    if(rw == 'W')
                    {
                        dirty[nframes-1] = 1;
                    }
                    else
                    {
                        dirty[nframes-1] = 0;
                    }
                    pageFault++;
                }
            }
            if(debug == 1)
            {
                for(i = 0; i < nframes; i++)
                {
                printf("%s", page[i]);
                }
                printf("\nNumber of Reads: %d\n", pageFault);
                printf("Number of Writes: %d\n", writes);
            }
            
        }

        for(i = 0; i < nframes; i++)
        {
            printf("%s\n", page[i]);
        }
	printf("Number of frames: %d\n", nframes);
        printf("Number of events in the trace: %d\n", count);
        printf("Number of disk Reads: %d\n", pageFault);
        printf("Number of disk Writes: %d\n", writes);

    }

    void lru(int nframes, int debug, FILE* file)
    {
        int count = 0;
        char addr[8];
        char rw;
        int tempD = 0;
        int dirty[nframes];
        int i = 0;
        int k = 0;
        int j = 0;
        char page[nframes][8];
        
        //int k = 0;
        int pos = 0;
        int exists = 0;
        int pageFault = 0;
        int writes = 0;
        while(fscanf(file, "%s %c", &addr, &rw) != EOF)
        {
            count++;
            for (j = 0; j < nframes; j++)
            {
                if (strncmp(page[j], addr, 5) == 0)
                {
                    exists = 1;
                    if (rw == 'W')
                    {
                        dirty[j] = 1;
                    }
                    else
                    {
                        dirty[j] = 0;
                    }

                    tempD = dirty[j];
                    for (k = j; k >= 0; k--)
                    {
                        strncpy(page[k], page[k - 1], 5);
                        dirty[k] = dirty[k - 1];
                    }
                    strncpy(page[0], addr, 5);
                    dirty[0] = tempD;
                    break;
                }
                else
                {
                    exists = 0;
                }
            }
            if (exists == 0)
            {
                if (dirty[nframes - 1] == 1)
                {
                    writes++;
                }
                for (j = nframes - 1; j >= 0; j--)
                {
                    strncpy(page[j], page[j - 1], 5);
                    dirty[j] = dirty[j - 1];
                }
                strncpy(page[0], addr, 5);
                if (rw = 'W')
                {
                    dirty[0] = 1;
                }
                else
                {
                    dirty[0] = 0;
                }

                pageFault++;
            }
            if(debug == 1)
            {
                for(i = 0; i < nframes; i++)
                {
                printf("%s\n", page[i]);
                }
                printf("\nNumber of Reads: %d\n", pageFault);
                printf("Number of Writes: %d\n", writes);
            }
            
        }

        for(i = 0; i < nframes; i++)
        {
            printf("%s\n", page[i]);
        }
	printf("Number of frames: %d\n", nframes);
        printf("Number of events in the trace: %d\n", count);
        printf("Number of disk Reads: %d\n", pageFault);
        printf("Number of disk Writes: %d\n", writes);

    }

    void vms(int nframes, int debug, FILE* file)
    {
        
    }

int main(int argc, char* argv[]) {
    //argv 1 is the number of frames
    int nframes = atoi(argv[2]);
    char page[nframes][50];
    int pos = 0;
    //argv 0 is the file name
    FILE* file = fopen(argv[1], "r");
    int i = 0;
    int j = 0;
    int debug = 0;
    //0 = False, 1 = True
    int exists = 0;
    int pageFault = 0;
    int writes = 0;

    if(strcmp(argv[4], "debug") == 0)
    {
        debug = 1;
    }
    else
    {
        debug = 0;
    }

    if(strcmp(argv[3], "lru") == 0)
    {
        lru(nframes, debug, file);
    }
    else if(strcmp(argv[3], "fifo") == 0)
    {
        fifo(nframes, debug, file);
    }
    else if(strcmp(argv[3], "vms") == 0)
    {
        vms(nframes, debug, file);
    }
    else
    {
        printf("Invalid algorithm entered.\n");
    }

    //lru(nframes, debug);

    //fifo(nframes, debug);

    return 0;
}

