#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void fifo(int nframes, int debug, FILE* file)
    {
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
        printf("\nNumber of Reads: %d\n", pageFault);
        printf("Number of Writes: %d\n", writes);

    }

    void lru(int nframes, int debug, FILE* file)
    {
        char addr[8];
        char rw;
        char page[nframes][8];
        char temp[8];
        int tempD = 0;
        int dirty[nframes];
        int i = nframes-1;
        int j = 0;
        int k = 0;
        int pos = 0;
        int exists = 0;
        int pageFault = 0;
        int writes = 0;
        while(fscanf(file, "%s %c", &addr, &rw) != EOF)
        {
            if(i >= 0)
            {
                exists = 0;
                for(j = 0; j < nframes; j++)
                {
                    if(strncmp(page[j], addr, 5) == 0)
                    {
                        exists = 1;
                        if(rw == 'W')
                        {
                            dirty[j] = 1;
                        }
                        tempD = dirty[j];
                        for(k = j; k >=0; k--)
                        {
                            int temp2 = dirty[k - 1]
                            strcpy(page[k], page[k-1]);
                            dirty[k] = temp2;
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
                if(exists == 0)
                {
                    strncpy(page[i], addr, 5);
                    if(rw == 'W')
                    {
                        dirty[i] = 1;
                    }
                    i--;
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
                        pos = j;
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
                    if(dirty[nframes-1] == 1)
                    {
                        writes++;
                    }
                    for(j = nframes-1; j >= 0; j--)
                    {
                        int temp2 = dirty[j-1];
                        strcpy(page[j], page[j-1]);
                        dirty[j] = temp2;
                    }
                    strncpy(page[0], addr, 5);
                    if(rw == 'W')
                    {
                        dirty[0] = 1;
                    }
                    else
                    {
                        dirty[0] = 0;
                    }
                    pageFault++;
                }
                else
                {
                    tempD = dirty[pos];
                    for(j = pos; j >= 0; j--)
                    {
                        int temp2 = dirty[j-1];
                        strcpy(page[j], page[j-1]);
                        dirty[j] = temp2;
                    }
                    strncpy(page[0], addr, 5);
                    dirty[0] = tempD;
                }
                
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
        printf("\nNumber of Reads: %d\n", pageFault);
        printf("Number of Writes: %d\n", writes);

    }

int main(int argc, char* argv[]) {
    printf("This is america\n");
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

    printf("This is the string: %s\n", argv[3]);
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
        //work in progress
    }
    else
    {
        printf("Invalid algorithm entered.\n");
    }

    //lru(nframes, debug);

    //fifo(nframes, debug);

    return 0;
}

