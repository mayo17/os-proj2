#include <stdio.h>
#include <string.h>

int main() {

    int nframes = 8;
    char page[nframes][50];
    char filePage[50];
    FILE* file = fopen("sixpack.trace", "r");
    int i = 0;
    int j = 0;
    int debug = 0;
    //0 = False, 1 = True
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
        if(debug == 1);
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

    return 0;
}