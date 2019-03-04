#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void fifo(int nframes, int debug, FILE* file)
    {
        char filePage[50];
        char page[nframes][50];
        int i = 0;
        int j = 0;
        int pos = 0;
        int exists = 0;
        int pageFault = 0;
        int writes = 0;
        while(fgets(filePage, 50, file) != NULL)
        {
            if(i < nframes)
            {
                for(j = 0; j < nframes; j++)
                {
                    if(strncmp(page[j], filePage, 8) == 0)
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
                    strcpy(page[i], filePage);
                    i++;
                    pageFault++;
                }
            }
            else
            {
                for(j = 0; j < nframes; j++)
                {
                    if(strncmp(page[j], filePage, 8) == 0)
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
                    //If page being replaced had a write instruction increment the writes counter
                    if(page[nframes-1][9] == 'W')
                    {
                        writes++;
                    }
                    for(j = 0; j < nframes-1; j++)
                    {
                        strcpy(page[j], page[j+1]);
                    }
                    strcpy(page[nframes-1], filePage);
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
            printf("%s", page[i]);
        }
        printf("\nNumber of Reads: %d\n", pageFault);
        printf("Number of Writes: %d\n", writes);

    }

    void lru(int nframes, int debug, FILE* file)
    {
        char page[nframes][50];
        char filePage[50];
        int i = 0;
        int j = 0;
        int pos = 0;
        int exists = 0;
        int pageFault = 0;
        int writes = 0;
        while(fgets(filePage, 50, file) != NULL)
        {
            if(i < nframes)
            {
                for(j = 0; j < nframes; j++)
                {
                    if(strncmp(page[j], filePage, 8) == 0)
                    {
                        exists = 1;
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
                    strcpy(page[i], filePage);
                    i++;
                    pageFault++;
                }
            }
            else
            {
                for(j = 0; j < nframes; j++)
                {
                    if(strncmp(page[j], filePage, 8) == 0)
                    {
                        exists = 1;
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
                    if(page[nframes-1][9] == 'W')
                    {
                        writes++;
                    }
                    for(j = nframes-1; j > 0; j--)
                    {
                        strcpy(page[j], page[j-1]);
                    }
                    strcpy(page[0], filePage);
                    pageFault++;
                }
                else
                {
                    for(j = pos; j > 0; j--)
                    {
                        strcpy(page[j], page[j-1]);
                    }
                    strcpy(page[0], filePage);
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
            printf("%s", page[i]);
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

    printf("This is the string: %s\n", argv[1]);
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

