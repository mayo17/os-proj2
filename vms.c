#include <stdio.h>
#include <string.h>

int main()
{

  int nframes = 12;
  char page[nframes][50];
  char filePage[50];
  int pos = 0;
  File* file = fopen("gcc.trace", "r");
  int i = 0;
  int j = 0;
  int debug = 0;
  //0 = False, 1 = True
  int exists = 0;
  int pageFault = 0;
  int writes = 0;

  void vms(int nframes, debug)
  {
    //Instantiates nframes storage for FIFO
    //Also instantiates counters.
    int rssA = 0;
    int rssB = 0;

    if (nframes%2 == 0) //if nframes is even.
    {
      rssA = nframes/2;
      rssB = nframes/2;
    }else
    {
      rssA = nframes/2 + 1;
      rssB = nframes/2;
    }

    //Instantiates working arrays.
    char fifoA[rssA][50];
    char fifoB[rssB][50];
    char clean[nframes/2 + 1][50];
    char dirty[nframes/2 + 1][50];
    char page[nframes][50];

    while (fgets(filePage, 50, file) != NULL)
    {
      if (filePage[0] == '3')
      {
        if (i < rssA)
        {
          for (j = 0; j < rssA; j++)
          {
            if (strncmp(fifoA[j], filePage, 8) == 0)
            {
              exists = 1;
              break;
            }else
            {
              exists = 0;
            }
          }
          if (exists == 0)
          {

            strcpy(fifoA[i], filePage);
            i++;
            pageFault++;
          }
        }else
        {
          for (j = 0; j < rssA; j++)
          {
            if(strncmp(fifoA[j], filePage, 8) == 0)
            {
                exists = 1;
                break;
            }
            else
            {
                exists = 0;
            }
          }
        }
      }else
      {
        if (filePage[0] == '3')
        {
          if (i < rssA)
          {
            for (j = 0; j < rssA; j++)
            {
              if (strncmp(fifoA[j], filePage, 8) == 0)
              {
                exists = 1;
                break;
              }else
              {
                exists = 0;
              }
            }
            if (exists == 0)
            {
              strcpy(fifoA[i], filePage);
              i++;
              pageFault++;
            }
          }else
          {
            for (j = 0; j < rssA; j++)
            {
              if(strncmp(fifoA[j], filePage, 8) == 0)
              {
                  exists = 1;
                  break;
              }
              else
              {
                  exists = 0;
              }
            }
          }
      }
    }
  }



}
