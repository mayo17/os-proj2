#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

  int nframes = 12;
  FILE* file = fopen("gcc.trace", "r");
  int debug = 0;
  //0 = False, 1 = True
  int pageFault = 0;
  int writes =  0;

  void vms(int nframes, int debug){

    //Instantiate used integers.
    int rss = 0;
    int diskread = 0;
    int diskwrite = 0;
    int exists = 0;
    int existsc = 0;
    int existsd = 0;
    int temp = 0;
    int i = 0;
    int j = 0;
    int k = 0;

    //Instantiate for file-reading.
    char addr[8];
    char rw;

    //Instantiates working arrays.
    char fifoA[rss][5];
    char fifoB[rss][5];
    int fifoAdirt[rss];
    int fifoBdirt[rss];
    char clean[nframes/2 + 1][5];
    char dirty[nframes/2 + 1][5];
    char page[nframes][5];

    while (fscanf(file, "%s %c", &addr, &rw) == 1) {
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
              for (k = 0; dirty[k] != NULL; k++); //Iterates to the end of dirty.
              strcpy(dirty[k], fifoA[0]);
            }else { //If the page being replaced is clean.
              for (k = 0; clean[k] != NULL; k++); //Iterates to the end of clean.
              strcpy(clean[k], fifoA[0]);
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
                strcpy(fifoA[k], fifoA[k+1]);
                fifoA[k] = fifoA[k+1];
                temp = fifoAdirt[k+1]; //STOPPED HERE
                fifoAdirt[k] = temp;
              }
              for (k = 0; k < nframes/2; k++) {
                strcpy(clean[k], clean[k+1]);
              }
            } else {
              for (k = 0; clean[k] != NULL; k++) { //Checks if page exists in clean.

              }
            }
            //rest of the text here.
          }
        }
      }else { //For the other process.

      }
    }

  }

  return 0;

}
