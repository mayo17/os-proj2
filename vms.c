#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

  int nframes = 12;
  FILE* file = fopen("gcc.trace", "r");
  int debug = 1;
  //0 = False, 1 = True
  int pageFault = 0;
  int writes =  0;

  struct Page{//Struct for the linked list to represent the page table.
    char pg[5];
    int dirty = 0;
    struct Page *prev;
    struct Page *next;
  }

  void vms(int nframes, int debug){

    //Instantiate used integers.
    int rss = 0;
    int diskread = 0;
    int diskwrite = 0;
    int exists = 0;
    int existsc = 0;
    int existsd = 0;
    int temp = 0;
    int count = 0;
    int i = 0;
    int j = 0;
    int k = 0;

    //Instantiate for file-reading.
    char addr[8];
    char rw;

    //Instantiates working arrays.
    char fifoA[rss][5];
    char fifoB[rss][5];
    char tempstr[5];
    int fifoAdirt[rss];
    int fifoBdirt[rss];
    char clean[nframes/2 + 1][5];
    char dirty[nframes/2 + 1][5];

    while (fscanf(file, "%s %c", &addr, &rw) == 1) {
      count++;
      if (addr[0] == '3')
      {
        //For the initial filling of the FIFO.
        if (i < rss) {
          for (j = 0; j < rss; j++) { //Loops through the page table.
            if (strncmp(fifoA[j], addr, 5) == 0) { //If the page exists.
              exists = 1;
              if (rw == 'W') { //Checks if the page needs to be made dirty.
                fifoAdirt[j] = 1;
              }
              break;
            }else {
              exists = 0;
            }
          }
          //If the page does not exist and the page table is not full
          if (exists == 0) {
            strncpy(fifoA[i], addr, 5);
            if (rw = 'W')
            {
              fifoAdirt[i] = 1;
            }
            i++;
            pageFault++;
          }
        }else { //For when the FIFO is filled.
          for (j = 0; j < rss; j++) {
            if (strncmp(fifoA[j], addr, 5) == 0) {//If page exists.
              if (rw == 'W') { //Checks if the page needs to be made dirty.
                fifoAdirt[j] = 1;
              }
              break;
            }else { //If page does not exist.
              exists = 0;
            }
          }
          if (exists == 0) {//If page does not exist.
            if (fifoAdirt[0] == 1) { //If the page being replaced is dirty.
              for (k = 0; dirty[k] != NULL; k++); //Iterates to the end of dirty. MIGHT NOT WORK, TRY STORE THE SIZE OF DIRTY
              strncpy(dirty[k], fifoA[0], 5);
            }else { //If the page being replaced is clean.
              for (k = 0; clean[k] != NULL; k++); //Iterates to the end of clean.
              strncpy(clean[k], fifoA[0], 5);
            }
            //Checks clean and dirty for the page.
            for (k = 0; dirty[k] != NULL; k++) { //Checks if page exists in dirty.
              if (strncmp(dirty[k], addr, 5) == 0) { //If the page is found in dirty.
                existsd = 1;
                break;
              }else {
                existsd = 0;
              }
            }
            if (existsd == 1) { //If the page is found in dirty
              for (k = 0; k < nframes/2 - 1; k++) {
                strncpy(fifoA[k], fifoA[k+1], 5);
                //fifoA[k] = fifoA[k+1];
                //temp = &fifoAdirt[k+1]; //STOPPED HERE
                fifoAdirt[k] = fifoAdirt[k+1]; //FINISH NOT COMPLETE
                writes++;
              }
              strncpy(fifoA[k], addr, 5); //Adds the page to the FIFO
              if (rw == 'W') { //If the trace is a Write, set it dirty.
                fifoAdirt[k] = 1;
              }else {
                fifoAdirt[k] = 0;
              }
              // for (k = 0; k < nframes/2 - 1; k++) {
              //   strncpy(clean[k], clean[k+1], 5);
              // }
            } else { //If the page is not found in dirty.
              for (k = 0; clean[k] != NULL; k++) { //Checks if page exists in clean.
                if (strncmp(clean[k], addr, 5) == 0) { //If the page is found in clean.
                  existsc = 1;
                  break;
                }else {
                  existsc = 0;
                }
              }
              if (existsc == 1) //If the page is found in clean
              {
                for (k = 0; k < nframes/2; k++) {
                  strncpy(fifoA[k], fifoA[k+1], 5);
                  fifoAdirt[k] = fifoAdirt[k+1];
                }
                strncpy(fifoA[k], addr, 5);
                if (rw == 'W') { //If the trace is a Write, set it dirty.
                  fifoAdirt[k] = 1;
                }else {
                  fifoAdirt[k] = 0;
                }
              } else {//If page is not found in clean or dirty, take first clean.
                for (k = 0; k < nframes/2 - 1; k++) {
                  //strncpy(temp, fifoA[k]);
                  strncpy(fifoA[k], fifoA[k+1], 5);
                  fifoAdirt[k] = fifoAdirt[k+1];
                  pageFault++;
                }
                strncpy(fifoA[k], addr, 5);
                if (rw == 'W') { //If the trace is a Write, set it dirty.
                  fifoAdirt[k] = 1;
                }else {
                  fifoAdirt[k] = 0;
                }
              }
            }
            //rest of the text here.
          }
        }
      }else { //For the other process.
        //For the initial filling of the FIFO.
        if (i < rss) {
          for (j = 0; j < rss; j++) { //Loops through the page table.
            if (strncmp(fifoB[j], addr, 5) == 0) { //If the page exists.
              exists = 1;
              if (rw == 'W') { //Checks if the page needs to be made dirty.
                fifoBdirt[j] = 1;
              }
              break;
            }else {
              exists = 0;
            }
          }
          //If the page does not exist and the page table is not full
          if (exists == 0) {
            strncpy(fifoB[i], addr, 5);
            if (rw = 'W')
            {
              fifoBdirt[i] = 1;
            }
            i++;
            pageFault++;
          }
        }else { //For when the FIFO is filled.
          for (j = 0; j < rss; j++) {
            if (strncmp(fifoB[j], addr, 5) == 0) {//If page exists.
              if (rw == 'W') { //Checks if the page needs to be made dirty.
                fifoBdirt[j] = 1;
              }
              break;
            }else { //If page does not exist.
              exists = 0;
            }
          }
          if (exists == 0) {//If page does not exist.
            if (fifoBdirt[0] == 1) { //If the page being replaced is dirty.
              for (k = 0; dirty[k] != NULL; k++); //Iterates to the end of dirty. MIGHT NOT WORK, TRY STORE THE SIZE OF DIRTY
              strncpy(dirty[k], fifoB[0], 5);
            }else { //If the page being replaced is clean.
              for (k = 0; clean[k] != NULL; k++); //Iterates to the end of clean.
              strncpy(clean[k], fifoB[0], 5);
            }
            //Checks clean and dirty for the page.
            for (k = 0; dirty[k] != NULL; k++) { //Checks if page exists in dirty.
              if (strncmp(dirty[k], addr, 5) == 0) { //If the page is found in dirty.
                existsd = 1;
                break;
              }else {
                existsd = 0;
              }
            }
            if (existsd == 1) { //If the page is found in dirty
              for (k = 0; k < nframes/2 - 1; k++) {
                strncpy(fifoB[k], fifoB[k+1], 5);
                //fifoB[k] = fifoB[k+1];
                //temp = &fifoBdirt[k+1]; //STOPPED HERE
                fifoBdirt[k] = fifoBdirt[k+1]; //FINISH NOT COMPLETE
                writes++;
              }
              strncpy(fifoB[k], addr, 5); //Adds the page to the FIFO
              if (rw == 'W') { //If the trace is a Write, set it dirty.
                fifoBdirt[k] = 1;
              }else {
                fifoBdirt[k] = 0;
              }
              // for (k = 0; k < nframes/2 - 1; k++) {
              //   strncpy(clean[k], clean[k+1], 5);
              // }
            } else { //If the page is not found in dirty.
              for (k = 0; clean[k] != NULL; k++) { //Checks if page exists in clean.
                if (strncmp(clean[k], addr, 5) == 0) { //If the page is found in clean.
                  existsc = 1;
                  break;
                }else {
                  existsc = 0;
                }
              }
              if (existsc == 1) //If the page is found in clean
              {
                for (k = 0; k < nframes/2; k++) {
                  strncpy(fifoB[k], fifoB[k+1], 5);
                  fifoBdirt[k] = fifoBdirt[k+1];
                }
                strncpy(fifoB[k], addr, 5);
                if (rw == 'W') { //If the trace is a Write, set it dirty.
                  fifoBdirt[k] = 1;
                }else {
                  fifoBdirt[k] = 0;
                }
              } else {//If page is not found in clean or dirty, take first clean.
                for (k = 0; k < nframes/2 - 1; k++) {
                  //strncpy(temp, fifoB[k]);
                  strncpy(fifoB[k], fifoB[k+1], 5);
                  fifoBdirt[k] = fifoBdirt[k+1];
                  pageFault++;
                }
                strncpy(fifoB[k], addr, 5);
                if (rw == 'W') { //If the trace is a Write, set it dirty.
                  fifoBdirt[k] = 1;
                }else {
                  fifoBdirt[k] = 0;
                }
              }
            }
            //rest of the text here.
          }
        }
      }

      if (debug == 1) { //If debug is true, prints the page.
        printf("\nFIFO(A):\t[ ");
        for (i = 0; i < nframes/2; i++) {
          printf("%s ", fifoA[i]);
        }
        printf("]\nFIFO(B):\t[ ");
        for (i = 0; i < nframes/2; i++) {
          printf("%s ", fifoB[i]);
        }
        printf("]\nClean:\t[ ");
        for (i = 0; i < nframes/2+1; i++) {
          printf("%s ", clean[i]);
        }
        printf("]\nDirty:\t[ ");
        for (i = 0; i < nframes/2+1; i++) {
          printf("%s ", dirty[i]);
        }
        printf("]\n\n");

        printf("Total memory frames: %d\nEvents in trace: %d\nTotal disk read:%d\nTotal disk writes: %d\n\n", nframes, count, pageFault, writes);
      }
    }
    printf("Total memory frames: %d\nEvents in trace: %d\nTotal disk read: %d\nTotal disk writes: %d\n\n", nframes, count, pageFault, writes);
  }

  vms(nframes, debug);

  return 0;

}
