#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
  //printf("Linked Pages Initialized.\n");

  int nframes = 12;
  FILE* file = fopen("gcc.trace", "r");
  int debug = 1;
  //0 = False, 1 = True
  int pageFault = 0;
  int writes =  0;

  //printf("Main Variables Initialized.\n");

  struct Page{//Struct for the linked list to represent the page table.
    char pg[5];
    int dirty;
    struct Page *prev;
    struct Page *next;
  };

  //printf("Struct Initialized.\n");

  void rmhead(struct Page* head) {//To remove the preset heads.
    struct Page* temptr = head->next;
    //free(head);
    temptr->prev = NULL;
    head = temptr;
  }

  void add2end(char* addr, char rw, struct Page* head) {//Function to add pages to the end exclusively for FIFO
    struct Page* temptr = head;
    struct Page* newptr;
    newptr = (struct Page*)malloc(sizeof(struct Page));

    while (temptr->next != NULL) {
      temptr = temptr->next;
    }

    //newptr->pg = (char* )malloc(5 + 1);
    strncpy(newptr->pg, addr, 5);
    printf("Page added to table\n");
    if (rw == 'W') {
      newptr->dirty = 1;
    }else {
      newptr->dirty = 0;
    }

    if (temptr->pg == NULL) {//If the new page is the only page
      temptr->next = newptr;
      rmhead(head);
    }else {
      newptr->prev = temptr;
      temptr->next = newptr;
    }
    newptr->next = NULL;

    free(newptr);
  }

  printf("Functions Initialized.\n");

  void vms(int nframes, int debug){

    //Instantiate used integers.
    int rss = nframes/2;
    int exists = 0;
    int existsc = 0;
    int existsd = 0;
    int temp = 0;
    int count = 0;
    int i = 0;
    int e = 0;
    int j = 0;
    int k = 0;

    //Instantiate for file-reading.
    char scanaddr[8];
    //addr = (char*)malloc(8 + 1);
    char rw;

    char *addr;
    //tempstr = (char*)malloc(5 + 1);

    //Instantiate Linked List Pointers
    struct Page* headA;
    struct Page* headB;
    struct Page* headClean;
    struct Page* headDirty;
    struct Page* tempA;
    struct Page* tempB;
    struct Page* tempClean;
    struct Page* tempDirty;
    struct Page* ptr;

    int countA = 0;
    int countB = 0;
    int countClean = 0;
    int countDirty = 0;
    int procA = 0;
    int procB = 0;

    headA = (struct Page*)malloc(sizeof(struct Page));
    strncpy(headA->pg, "\0", 5);
    printf("headA initialized\n");
    headA->dirty = 0;
    headA->prev = NULL;
    headA->next = NULL;
    //add2end(NULL, 'R', headA);
    headB = (struct Page*)malloc(sizeof(struct Page));
    strncpy(headA->pg, "\0", 5);
    headB->dirty = 0;
    headB->prev = NULL;
    headB->next = NULL;
    //add2end(NULL, 'R', headB);
    headClean = (struct Page*)malloc(sizeof(struct Page));
    strncpy(headA->pg, "\0", 5);
    headClean->dirty = 0;
    headClean->prev = NULL;
    headClean->next = NULL;
    //add2end(NULL, 'R', headClean);
    headDirty = (struct Page*)malloc(sizeof(struct Page));
    strncpy(headA->pg, "\0", 5);
    headDirty->dirty = 0;
    headDirty->prev = NULL;
    headDirty->next = NULL;
    //add2end(NULL, 'W', headDirty);

    printf("Instantiations Complete.\n");

    //free(temptr); to remove data.

    while (fscanf(file, "%s %c", &scanaddr, &rw) != EOF) {
      count++; //Stores the number of traces.
      printf("\nCount: %d\n", count);
      //printf("Address: %s\n", &addr);

      addr = strdup(scanaddr);

      if (strncmp(addr, "3", 1) == 0) {
        printf("Process Check Initialized. A\n");
        //For the initial filling of the FIFO.
        if (i < rss) {
          tempA = headA;
          for (j = 0; j < rss && tempA != NULL; j++) { //Loops through the page table.
            //strncpy(temp, addr, 5); //Stores the page temporarily.
            if (strncmp(tempA->pg, addr, 5) == 0) { //If the page exists.
              exists = 1;
              if (rw == 'W') { //Checks if the page needs to be made dirty.
                tempA->dirty = 1;
              }
              break;
            }else {
              exists = 0;
            }
            tempA = tempA->next;
          }
          //If the page does not exist and the page table is not full
          if (exists == 0) {
            //strncpy(fifoA[i], addr, 5);
            add2end(addr, rw, headA); //Adds the new page to the end.
            i++;
            pageFault++;
          }
        }else { //For when the FIFO is filled.
          tempA = headA;
          for (j = 0; j < rss && tempA != NULL; j++) {
            if (strncmp(tempA->pg, addr, 5) == 0) {//If page exists.
              exists = 1;
              if (rw == 'W') { //Checks if the page needs to be made dirty.
                tempA->dirty = 1;
              }
              break;
            }else { //If page does not exist.
              exists = 0;
            }
            tempA = tempA->next;
          }
          if (exists == 0) {//If page does not exist in the FIFO.

            //Checks clean and dirty for the page to be taken.

            tempDirty = headDirty;
            while (tempDirty != NULL) {//Checks if page exists in dirty.
              if (strncmp(tempDirty->pg, addr, 5) == 0) { //If the page is found in dirty.
                existsd = 1;
                break;
              }else {
                existsd = 0;
              }
              tempDirty = tempDirty->next;
            }
            if (existsd == 1) { //If the page is found in dirty
              if (tempDirty->prev != NULL) {//Change the Dirty list.
                tempDirty->prev->next = tempDirty;
                tempDirty->prev->next->prev = tempDirty->prev; //UNSURE
                //free(tempDirty);
                //writes++;
              }else {//Removes the head dirty page
                strncpy(headDirty->pg, NULL, 5);
                //add2end(headA->pg, 'W', headDirty);
              }
              //Runs FIFO to add the new page.
              //headA = headA->next;
              //headA->prev = NULL;
              if (headA->dirty == 1) {
                add2end(headA->pg, 'W', headDirty);
              } else {
                add2end(headA->pg, 'R', headClean);
              }
              rmhead(headA);
              add2end(addr, rw, headA);
            } else { //If the page is not found in dirty.
              tempClean = headClean;
              while (tempClean != NULL) { //Checks if page exists in clean.
                if (strncmp(tempClean->pg, addr, 5) == 0) { //If the page is found in clean.
                  existsc = 1;
                  break;
                }else {
                  existsc = 0;
                }
                tempDirty = tempDirty->next;
              }
              if (existsc == 1) //If the page is found in clean
              {
                if (tempClean->prev != NULL) {//Change the Clean list.
                  tempClean->prev->next = tempClean;
                  tempClean->prev->next->prev = tempClean->prev; //UNSURE
                  //free(tempDirty);
                  //pageFault++;
                }else {//Remove the head value.
                  strncpy(headClean->pg, NULL, 5);
                  //add2end(headA->pg, 'R', headClean);
                }
                //Runs FIFO to add the new page.
                //headA = headA->next;
                //headA->prev = NULL;
                if (headA->dirty == 1) {
                  add2end(headA->pg, 'W', headDirty);
                } else {
                  add2end(headA->pg, 'R', headClean);
                }
                rmhead(headA);
                add2end(addr, rw, headA);
              } else {//If page is not found in clean or dirty, take first clean.
                if (strncmp(headClean->pg, NULL, 5) != 0) {
                  rmhead(headA);
                  add2end(addr, rw, headA);
                  strncpy(headClean->pg, NULL, 5);
                  pageFault++;
                }else if (strncmp(headDirty->pg, NULL, 5) != 0) {
                  rmhead(headA);
                  add2end(addr, rw, headA);
                  strncpy(headDirty->pg, NULL, 5);
                  pageFault++;
                  writes++;
                }
              }
            }
          }
        }
      }else { //For the other process.
        printf("Other Process Initialized.\n");
        //For the initial filling of the FIFO.
        if (e < rss) {
          tempB = headB;
          for (j = 0; j < rss && tempB != NULL; j++) { //Loops through the page table.
            //strncpy(temp, addr, 5); //Stores the page temporarily.
            //strncpy(tempstr, tempB->pg, 5);
            //printf("%s\n", tempB->pg);
            //printf("%s\n", tempstr);
            printf("Equality Check\n");
            if (strncmp(addr, tempB->pg, 5) == 0) { //If the page exists.
              printf("Page exists in FIFOB not full\n");
              exists = 1;
              if (rw == 'W') { //Checks if the page needs to be made dirty.
                tempB->dirty = 1;
              }
              break;
            }else {
              printf("Page does not exists in FIFOB not full\n");
              exists = 0;
            }
            tempB = tempB->next;
          }
          //If the page does not exist and the page table is not full
          if (exists == 0) {
            //strncpy(fifoA[i], addr, 5);
            printf("Set page, if it did not exists not full\n");
            add2end(addr, rw, headA); //Adds the new page to the end.
            e++;
            pageFault++;
          }
        }else { //For when the FIFO is filled.
          tempB = headB;
          for (j = 0; j < rss && tempB != NULL; j++) {
            if (strncmp(tempB->pg, addr, 5) == 0) {//If page exists.
              printf("Page did exists in FIFOA full\n");
              exists = 1;
              if (rw == 'W') { //Checks if the page needs to be made dirty.
                tempB->dirty = 1;
              }
              break;
            }else { //If page does not exist.
              printf("Page did not exist in FIFOA not full\n");
              exists = 0;
            }
            tempB = tempB->next;
          }
          if (exists == 0) {//If page does not exist in the FIFO.

            //Checks clean and dirty for the page to be taken.

            tempDirty = headDirty;
            while (tempDirty != NULL) {//Checks if page exists in dirty.
              if (strncmp(tempDirty->pg, addr, 5) == 0) { //If the page is found in dirty.
                existsd = 1;
                break;
              }else {
                existsd = 0;
              }
              tempDirty = tempDirty->next;
            }
            if (existsd == 1) { //If the page is found in dirty
              if (tempDirty->prev != NULL) {//Change the Dirty list.
                tempDirty->prev->next = tempDirty;
                tempDirty->prev->next->prev = tempDirty->prev; //UNSURE
                //free(tempDirty);
                //writes++;
              }else {//Removes the head dirty page
                strncpy(headDirty->pg, NULL, 5);
                //add2end(headA->pg, 'W', headDirty);
              }
              //Runs FIFO to add the new page.
              //headA = headA->next;
              //headA->prev = NULL;
              if (headA->dirty == 1) {
                add2end(headA->pg, 'W', headDirty);
              } else {
                add2end(headA->pg, 'R', headClean);
              }
              rmhead(headA);
              add2end(addr, rw, headA);
            } else { //If the page is not found in dirty.
              tempClean = headClean;
              while (tempClean != NULL) { //Checks if page exists in clean.
                if (strncmp(tempClean->pg, addr, 5) == 0) { //If the page is found in clean.
                  existsc = 1;
                  break;
                }else {
                  existsc = 0;
                }
                tempDirty = tempDirty->next;
              }
              if (existsc == 1) //If the page is found in clean
              {
                if (tempClean->prev != NULL) {//Change the Clean list.
                  tempClean->prev->next = tempClean;
                  tempClean->prev->next->prev = tempClean->prev; //UNSURE
                  //free(tempDirty);
                  //pageFault++;
                }else {//Remove the head value.
                  strncpy(headClean->pg, NULL, 5);
                  //add2end(headA->pg, 'R', headClean);
                }
                //Runs FIFO to add the new page.
                //headA = headA->next;
                //headA->prev = NULL;
                if (headA->dirty == 1) {
                  add2end(headA->pg, 'W', headDirty);
                } else {
                  add2end(headA->pg, 'R', headClean);
                }
                rmhead(headA);
                add2end(addr, rw, headA);
              } else {//If page is not found in clean or dirty, take first clean.
                if (strncmp(headClean->pg, NULL, 5) != 0) {
                  rmhead(headA);
                  add2end(addr, rw, headA);
                  strncpy(headClean->pg, NULL, 5);
                  pageFault++;
                }else if (strncmp(headDirty->pg, NULL, 5) != 0) {
                  rmhead(headA);
                  add2end(addr, rw, headA);
                  strncpy(headDirty->pg, NULL, 5);
                  pageFault++;
                  writes++;
                }
              }
            }
          }
        }
      }

      if (debug == 1) { //If debug is true, prints the page.
        printf("\nFIFO(A):  [ ");
        tempA = headA;
        while (tempA != NULL) {
          printf("%s ", tempA->pg);
          tempA = tempA->next;
        }
        printf("]\nFIFO(B):  [ ");
        tempB = headB;
        while (tempB != NULL) {
          printf("%s ", tempB->pg);
          tempB = tempB->next;
        }
        printf("]\nClean:    [ ");
        tempClean = headClean;
        while (tempClean != NULL) {
          printf("%s ", tempClean->pg);
          tempClean = tempClean->next;
        }
        printf("]\nDirty:    [ ");
        tempDirty = headDirty;
        while (tempDirty != NULL) {
          printf("%s ", tempDirty->pg);
          tempDirty = tempDirty->next;
        }
        printf("]\n");
      }
    }
    printf("Total memory frames: %d\nEvents in trace: %d\nTotal disk read: %d\nTotal disk writes: %d\n\n", nframes, count, procA, procB);
  }

  vms(nframes, debug);

  return 0;
}

printf("Process Initialized.\n");
//For the initial filling of the FIFO.
if (e < rss) {
  tempA = headA;
  for (j = 0; j < rss && tempA != NULL; j++) { //Loops through the page table.
    //strncpy(temp, addr, 5); //Stores the page temporarily.
    //strncpy(tempstr, tempA->pg, 5);
    //printf("%s\n", tempA->pg);
    //printf("%s\n", tempstr);
    printf("Equality Check\n");
    if (strncmp(addr, tempA->pg, 5) == 0) { //If the page exists.
      printf("%s\n", addr);
      printf("Page exists in FIFOB not full\n");
      exists = 1;
      if (rw == 'W') { //Checks if the page needs to be made dirty.
        tempA->dirty = 1;
      }
      break;
    }else {
      exists = 0;
    }
    tempA = tempA->next;
  }
  //If the page does not exist and the page table is not full
  if (exists == 0) {
    //strncpy(fifoA[i], addr, 5);
    printf("%s\n", addr);
    printf("Set page, if it did not exists not full\n");
    add2end(addr, rw, headA); //Adds the new page to the end.
    e++;
    pageFault++;
  }
}else { //For when the FIFO is filled.
  tempA = headA;
  for (j = 0; j < rss && tempA != NULL; j++) {
    if (strncmp(tempA->pg, addr, 5) == 0) {//If page exists.
      printf("Page did exists in FIFOA full\n");
      exists = 1;
      if (rw == 'W') { //Checks if the page needs to be made dirty.
        tempA->dirty = 1;
      }
      break;
    }else { //If page does not exist.
      printf("Page did not exist in FIFOA not full\n");
      exists = 0;
    }
    tempA = tempA->next;
  }
  if (exists == 0) {//If page does not exist in the FIFO.

    //Checks clean and dirty for the page to be taken.

    tempDirty = headDirty;
    while (tempDirty != NULL) {//Checks if page exists in dirty.
      if (strncmp(tempDirty->pg, addr, 5) == 0) { //If the page is found in dirty.
        existsd = 1;
        break;
      }else {
        existsd = 0;
      }
      tempDirty = tempDirty->next;
    }
    if (existsd == 1) { //If the page is found in dirty
      if (tempDirty->prev != NULL) {//Change the Dirty list.
        tempDirty->prev->next = tempDirty;
        tempDirty->prev->next->prev = tempDirty->prev; //UNSURE
        //free(tempDirty);
        //writes++;
      }else {//Removes the head dirty page
        strncpy(headDirty->pg, "\0", 5);
        //add2end(headA->pg, 'W', headDirty);
      }
      //Runs FIFO to add the new page.
      //headA = headA->next;
      //headA->prev = NULL;
      if (headA->dirty == 1) {
        add2end(headA->pg, 'W', headDirty);
      } else {
        add2end(headA->pg, 'R', headClean);
      }
      rmhead(headA);
      add2end(addr, rw, headA);
    } else { //If the page is not found in dirty.
      tempClean = headClean;
      while (tempClean != NULL) { //Checks if page exists in clean.
        if (strncmp(tempClean->pg, addr, 5) == 0) { //If the page is found in clean.
          existsc = 1;
          break;
        }else {
          existsc = 0;
        }
        tempDirty = tempDirty->next;
      }
      if (existsc == 1) //If the page is found in clean
      {
        if (tempClean->prev != NULL) {//Change the Clean list.
          tempClean->prev->next = tempClean;
          tempClean->prev->next->prev = tempClean->prev; //UNSURE
          //free(tempDirty);
          //pageFault++;
        }else {//Remove the head value.
          strncpy(headClean->pg, "\0", 5);
          //add2end(headA->pg, 'R', headClean);
        }
        //Runs FIFO to add the new page.
        //headA = headA->next;
        //headA->prev = NULL;
        if (headA->dirty == 1) {
          add2end(headA->pg, 'W', headDirty);
        } else {
          add2end(headA->pg, 'R', headClean);
        }
        rmhead(headA);
        add2end(addr, rw, headA);
      } else {//If page is not found in clean or dirty, take first clean.
        if (strncmp(headClean->pg, "\0", 5) != 0) {
          rmhead(headA);
          add2end(addr, rw, headA);
          strncpy(headClean->pg, "\0", 5);
          pageFault++;
        }else if (strncmp(headDirty->pg, "\0", 5) != 0) {
          rmhead(headA);
          add2end(addr, rw, headA);
          strncpy(headDirty->pg, "\0", 5);
          pageFault++;
          writes++;
        }
      }
    }
  }
}
