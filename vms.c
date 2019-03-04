#include <stdio.h>
#include <string.h>

int main()
{

  int nframes = 12;
  File* file = fopen("gcc.trace", "r");
  int debug = 0;
  //0 = False, 1 = True
  int pageFault = 0;
  int writes =  0;

  void vms(int nframes, debug)
  {
    //Instantiates nframes storage for FIFO
    //Also instantiates counters.
    int rssA = 0;
    int rssB = 0;
    int diskread = 0;
    int diskwrite = 0;
    int exists = 0;
    int i = 0;
    int j = 0;
    int k = 0;

    char addr[8];
    char rw;

    char pageTemp[5];
    char pageTemp2[5];

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
    char fifoA[rssA][5];
    char fifoB[rssB][5];
    int fifoAdirt[rssA] = 0;
    int fifoBdirt[rssB] = 0;
    char clean[nframes/2 + 1][5];
    char dirty[nframes/2 + 1][5];
    char page[nframes][5];

    while (fscanf(file, "%s %c", &addr, &rw) == 1)
    {
      if (addr[0] == '3') //If the first number of the trace is 3...
      {
        //For initially filling the page tables.
        if (i < rssA)
        {
          for (j = 0; j < rssA; j++) //Loops through fifoA
          {
            if (strncmp(fifoA[j], addr, 5) == 0)
            {
              exists = 1;
              if (rw == 'W') { //Checks if the page needs to be marked dirty.
                fifoAdirt[j] = 1;
              }
              break; //consider removal
            }else
            {
              exists = 0;
            }
          }
          if (exists == 0)
          {
            strncpy(fifoA[i], addr, 5);
            if (rw = 'W')
            {
              fifoAdirt[i] = 1;
            }
            i++;
            pageFault++;
          }
        }else
        {
          for (j = 0; j < rssA; j++)
          {
            if(strncmp(fifoA[j], addr, 5) == 0)
            {
                exists = 1;
                if (rw == 'W') { //Checks if the page needs to be marked dirty.
                  fifoAdirt[j] = 1;
                }
                break;
            }
            else
            {
                exists = 0;
            }
            if (exists == 0) {
              //Removes page and adds to dirty or clean.
              if (fifoAdirt[0] == 1) {
                for (k = 0; dirty[k] != NULL; k++);
                strcpy(dirty[k], fifoA[0]);
              }else {
                for (int k = 0; clean[k] != NULL; k++);
                strcpy(clean[k], fifoA[0]);
              }

            }

          }
        }
      }else //For the other process.
      {

      }
    }
  }

}
