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

    struct Page{//Struct for the linked list to represent the page table.
      char pg[5];
      int dirty;
      struct Page *prev;
      struct Page *next;
    };

    void add2end(char* addr, char rw, struct Page* head) {//Function to add pages to the end exclusively for FIFO
      struct Page* temptr = head;
      struct Page* newptr;
      newptr = (struct Page*)malloc(sizeof(struct Page));

      if (rw == 'W') { //Sets the dirty status.
        newptr->dirty = 1;
      }else {
        newptr->dirty = 0;
      }

      //Cycles to the end.
      while (temptr->next != NULL) {
        temptr = temptr->next;
      }

      //Checks if the page is the only page.
      if (strncmp(temptr->pg, "\0", 1) == 0) {
        strncpy(temptr->pg, addr, 5);
        temptr->dirty = newptr->dirty;
        temptr->prev = NULL;
        temptr->next = NULL;
      }else {
        strncpy(newptr->pg, addr, 5);
        newptr->prev = temptr;
        newptr->next = NULL;
        temptr->next = newptr;
      }
    }

    void vms(int nframes, int debug, FILE* file){

      //Instantiate used integers.
      int memory = 0;
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
      char rw;

      char *addr;

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
      struct Page* ptr2;

      int countA = 0;
      int countB = 0;
      int countClean = 0;
      int countDirty = 0;
      int procA = 0;
      int procB = 0;
      int pageFault = 0;
      int writes = 0;

      //Memory allocation for pages.
      headA = (struct Page*)malloc(sizeof(struct Page));
      strncpy(headA->pg, "\0", 5);
      headA->dirty = 0;
      headA->prev = NULL;
      headA->next = NULL;
      headB = (struct Page*)malloc(sizeof(struct Page));
      strncpy(headB->pg, "\0", 5);
      headB->dirty = 0;
      headB->prev = NULL;
      headB->next = NULL;
      headClean = (struct Page*)malloc(sizeof(struct Page));
      strncpy(headClean->pg, "\0", 5);
      headClean->dirty = 0;
      headClean->prev = NULL;
      headClean->next = NULL;
      headDirty = (struct Page*)malloc(sizeof(struct Page));
      strncpy(headDirty->pg, "\0", 5);
      headDirty->dirty = 1;
      headDirty->prev = NULL;
      headDirty->next = NULL;

      while (fscanf(file, "%s %c", &scanaddr, &rw) != EOF) {
        count++; //Stores the number of traces.
    //      printf("\nCount: %d\n", count);

        addr = strdup(scanaddr);

        if (strncmp(addr, "3", 1) == 0) {
    //        printf("Process Initialized\n");
    //        printf("%s\n", addr);
          if (i < rss) {
            tempA = headA;
            for (j = 0; j < rss && tempA->next != NULL; j++) { //Loops through the page table.
    //            printf("Equality Check\n");
              if (strncmp(addr, tempA->pg, 5) == 0) { //If the page exists.
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
    //            printf("Set page, if it did not exist in FIFOA not full\n");
              if (strncmp(headA->pg, "\0", 1) == 0) {
                strncpy(headA->pg, addr, 5); //Sets the new page as the head.
                if (rw == 'W') { //Checks if the page needs to be made dirty.
                  headA->dirty = 1;
                }
              }else {
                add2end(addr, rw, headA); //Adds the new page to the end.
                memory++;
              }
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
    //            printf("Set page, if it did not exist in FIFOA full\n");
              //Checks clean and dirty for the page to be taken.
              tempDirty = headDirty;
              while (tempDirty->next != NULL) {//Checks if page exists in dirty.
                if (strncmp(tempDirty->pg, addr, 5) == 0) { //If the page is found in dirty.
                  existsd = 1;
                  break;
                }else {
                  existsd = 0;
                }
                tempDirty = tempDirty->next;
    //              printf("Dirty Check.\n");
              }
              if (existsd == 1) { //If the page is found in dirty
    //              printf("Dirty Found\n");
                if (tempDirty->prev != NULL && tempDirty->next != NULL) {//Change the Dirty list.
                  tempDirty->prev->next = tempDirty->next;
                  tempDirty->prev->next->prev = tempDirty->prev;
    //                printf("Dirty Page Removed\n");
                }else if (tempDirty->prev == NULL){//Removes the head dirty page
                  if (headDirty->next != NULL) {
                    ptr = headDirty->next;  //Removes head Dirty.
                    ptr->prev = NULL;
                    headDirty = ptr;
                  }else {
                    strncpy(headDirty->pg, "\0", 5);
                    headDirty->dirty = 1;
                    headDirty->prev = NULL;
                    headDirty->next = NULL;
                  }
                  memory--;
                }else if (tempDirty->next == NULL) { //Removes the last dirty page
                  tempDirty->prev->next = NULL;
                }
                //Runs FIFO to add the new page.
                memory--;
                if (headA->dirty == 1) {
                  if (memory >= nframes) {
                    ptr2 = headDirty->next;
                    headDirty = ptr2;
                    memory--;
                  }
                  add2end(headA->pg, rw, headDirty);
                  memory++;
                }else if (headA->dirty == 0) {
                  if (memory >= nframes) {
                    ptr2 = headClean->next;
                    headClean = ptr2;
                    memory--;
                  }
                  add2end(headA->pg, rw, headClean);
                  memory++;
                }
                if (headA->next != NULL) {
                  ptr = headA->next;  //Removes head A.
                  ptr->prev = NULL;
                  headA = ptr;
                }else {
                  strncpy(headA->pg, "\0", 5);
                  headA->dirty = 0;
                  headA->prev = NULL;
                  headA->next = NULL;
                }
                memory++;
              }else { //If the page is not found in dirty.
                tempClean = headClean;
                while (tempClean != NULL) { //Checks if page exists in clean.
                  if (strncmp(tempClean->pg, addr, 5) == 0) { //If the page is found in clean.
    //                  printf("Clean found in page\n");
                    existsc = 1;
                    break;
                  }else {
    //                  printf("Clean not found in page\n");
                    existsc = 0;
                  }
                  tempClean = tempClean->next;
                }
                if (existsc == 1) //If the page is found in clean
                {
    //                printf("Clean Found\n");

                  if (tempClean->prev != NULL && tempClean->next != NULL) {//Change the Clean list.
                    tempClean->prev->next = tempClean->next;
                    tempClean->prev->next->prev = tempClean->prev;
    //                  printf("Clean Page Removed\n");
                  }else if (tempClean->prev == NULL){//Removes the head clean page
                    if (headClean->next != NULL) {
                      ptr = headClean->next;  //Removes head Clean.
                      ptr->prev = NULL;
                      headClean = ptr;
                    }else {
                      strncpy(headClean->pg, "\0", 5);
                      headClean->dirty = 0;
                      headClean->prev = NULL;
                      headClean->next = NULL;
                    }
                    memory--;
                  }else if (tempClean->next == NULL) { //Removes the last clean page
                    tempClean->prev->next = NULL;
                  }
                  //Runs FIFO to add the new page.
                  memory--;
                  if (headA->dirty == 1) {
                    if (memory >= nframes) {
                      if (headDirty->next != NULL) {
                        ptr = headDirty->next;  //Removes head Dirty.
                        ptr->prev = NULL;
                        headDirty = ptr;
                      }else {
                        strncpy(headDirty->pg, "\0", 5);
                        headDirty->dirty = 1;
                        headDirty->prev = NULL;
                        headDirty->next = NULL;
                      }
                      memory--;
                    }
                    add2end(headA->pg, rw, headDirty);
                    memory++;
                  }else if (headA->dirty == 0) {
                    if (memory >= nframes) {
                      ptr2 = headClean->next;
                      headClean = ptr2;
                      memory--;
                    }
                    add2end(headA->pg, rw, headClean);
                    memory++;
                  }
                  if (headA->next != NULL) {
                    ptr = headA->next;  //Removes head A.
                    ptr->prev = NULL;
                    headA = ptr;
                  }else {
                    strncpy(headA->pg, "\0", 5);
                    headA->dirty = 0;
                    headA->prev = NULL;
                    headA->next = NULL;
                  }
                  add2end(addr, rw, headA);
                  memory++;

                }else {//If page is not found in clean or dirty, take first clean if none, first dirty, if none, add.
    //                printf("Not Found in Clean or Dirty\n");
    //                printf("%d\n", memory);
                  memory--;
                  if (strncmp(headClean->pg, "\0", 5) != 0) {
    //                  printf("Added to clean\n");
                      if (headClean->next != NULL) {
                        ptr = headClean->next;  //Removes head Clean.
                        ptr->prev = NULL;
                        headClean = ptr;
                      }else {
                        strncpy(headClean->pg, "\0", 5);
                        headClean->dirty = 0;
                        headClean->prev = NULL;
                        headClean->next = NULL;
                      }
                    memory--;
                    pageFault++;
                  }else if (strncmp(headDirty->pg, "\0", 5) != 0) {
    //                  printf("Added to dirty\n");
                      if (headDirty->next != NULL) {
                        ptr = headDirty->next;  //Removes head Dirty.
                        ptr->prev = NULL;
                        headDirty = ptr;
                      }else {
                        strncpy(headDirty->pg, "\0", 5);
                        headDirty->dirty = 1;
                        headDirty->prev = NULL;
                        headDirty->next = NULL;
                      }
                    memory--;
                    pageFault++;
                    writes++;
                  }else {
                    if (headB->dirty == 1) {
                      if (memory >= nframes) {
                        if (headDirty->next != NULL) {
                          ptr = headDirty->next;  //Removes head Dirty.
                          ptr->prev = NULL;
                          headDirty = ptr;
                        }else {
                          strncpy(headDirty->pg, "\0", 5);
                          headDirty->dirty = 1;
                          headDirty->prev = NULL;
                          headDirty->next = NULL;
                        }
                        memory--;
                        pageFault++;
                        writes++;
    //                      printf("Dirty sent to disk\n");
                      }
    //                    printf("%s\n", headB->pg);
                      add2end(headB->pg, rw, headDirty);
                    }else if (headB->dirty == 0) {
                      if (memory >= nframes) {
                        if (headClean->next != NULL) {
                          ptr = headClean->next;  //Removes head Clean.
                          ptr->prev = NULL;
                          headClean = ptr;
                        }else {
                          strncpy(headClean->pg, "\0", 5);
                          headClean->dirty = 0;
                          headClean->prev = NULL;
                          headClean->next = NULL;
                        }
                        memory--;
    //                      printf("Clean sent to disk\n");
                      }
    //                    printf("%s\n", headB->pg);
                      add2end(headB->pg, rw, headClean);
                    }
                  }
                  if (headB->next != NULL) {
                    ptr = headB->next;  //Removes head B.
                    ptr->prev = NULL;
                    headB = ptr;
                  }else {
                    strncpy(headB->pg, "\0", 5);
                    headB->dirty = 0;
                    headB->prev = NULL;
                    headB->next = NULL;
                  }
                  add2end(addr, rw, headB);
                  memory++;
                }
              }
            }
          }
        }else { //For the other process.
          //printf("Other Process Initialized.\n");
    //        printf("%s\n", addr);
          //For the initial filling of the FIFO.
          if (e < rss) {
            tempB = headB;
            for (j = 0; j < rss && tempB->next != NULL; j++) { //Loops through the page table.
    //            printf("Equality Check\n");
              if (strncmp(addr, tempB->pg, 5) == 0) { //If the page exists.
                exists = 1;
                if (rw == 'W') { //Checks if the page needs to be made dirty.
                  tempB->dirty = 1;
                }
                break;
              }else {
                exists = 0;
              }
              tempB = tempB->next;
            }
            //If the page does not exist and the page table is not full
            if (exists == 0) {
    //            printf("Set page, if it did not exist in FIFOB not full\n");
              if (strncmp(headB->pg, "\0", 1) == 0) {
                strncpy(headB->pg, addr, 5); //Sets the new page as the head.
                if (rw == 'W') { //Checks if the page needs to be made dirty.
                  headB->dirty = 1;
                }
              }else {
                add2end(addr, rw, headB); //Adds the new page to the end.
                memory++;
              }
              e++;
              pageFault++;
            }
          }else { //For when the FIFO is filled.
            tempB = headB;
            for (j = 0; j < rss && tempB != NULL; j++) {
              if (strncmp(tempB->pg, addr, 5) == 0) {//If page exists.
                exists = 1;
                if (rw == 'W') { //Checks if the page needs to be made dirty.
                  tempB->dirty = 1;
                }
                break;
              }else { //If page does not exist.
                exists = 0;
              }
              tempB = tempB->next;
            }
            if (exists == 0) {//If page does not exist in the FIFO.
    //            printf("Set page, if it did not exist in FIFOB full\n");
              //Checks clean and dirty for the page to be taken.
              tempDirty = headDirty;
              while (tempDirty->next != NULL) {//Checks if page exists in dirty.
                if (strncmp(tempDirty->pg, addr, 5) == 0) { //If the page is found in dirty.
                  existsd = 1;
                  break;
                }else {
                  existsd = 0;
                }
                tempDirty = tempDirty->next;
    //              printf("Dirty Check.\n");
              }
              if (existsd == 1) { //If the page is found in dirty
    //              printf("Dirty Found\n");
                if (tempDirty->prev != NULL && tempDirty->next != NULL) {//Change the Dirty list.
                  tempDirty->prev->next = tempDirty->next;
                  tempDirty->prev->next->prev = tempDirty->prev;
    //                printf("Dirty Page Removed\n");
                }else if (tempDirty->prev == NULL){//Removes the head dirty page
                  if (headDirty->next != NULL) {
                    ptr = headDirty->next;  //Removes head Dirty.
                    ptr->prev = NULL;
                    headDirty = ptr;
                  }else {
                    strncpy(headDirty->pg, "\0", 5);
                    headDirty->dirty = 1;
                    headDirty->prev = NULL;
                    headDirty->next = NULL;
                  }
                  memory--;
                }else if (tempDirty->next == NULL) { //Removes the last dirty page
                  tempDirty->prev->next = NULL;
                }
                //Runs FIFO to add the new page.
                memory--;
                if (headB->dirty == 1) {
                  if (memory >= nframes) {
                    ptr2 = headDirty->next;
                    headDirty = ptr2;
                    memory--;
                  }
                  add2end(headB->pg, rw, headDirty);
                  memory++;
                }else if (headB->dirty == 0) {
                  if (memory >= nframes) {
                    ptr2 = headClean->next;
                    headClean = ptr2;
                    memory--;
                  }
                  add2end(headB->pg, rw, headClean);
                  memory++;
                }
                if (headB->next != NULL) {
                  ptr = headB->next;  //Removes head B.
                  ptr->prev = NULL;
                  headB = ptr;
                }else {
                  strncpy(headB->pg, "\0", 5);
                  headB->dirty = 0;
                  headB->prev = NULL;
                  headB->next = NULL;
                }
                add2end(addr, rw, headB);
                memory++;
              }else { //If the page is not found in dirty.
                tempClean = headClean;
                while (tempClean != NULL) { //Checks if page exists in clean.
                  if (strncmp(tempClean->pg, addr, 5) == 0) { //If the page is found in clean.
    //                  printf("Clean found in page\n");
                    existsc = 1;
                    break;
                  }else {
    //                  printf("Clean not found in page\n");
                    existsc = 0;
                  }
                  tempClean = tempClean->next;
                }
                if (existsc == 1) //If the page is found in clean
                {
    //                printf("Clean Found\n");

                  if (tempClean->prev != NULL && tempClean->next != NULL) {//Change the Clean list.
                    tempClean->prev->next = tempClean->next;
                    tempClean->prev->next->prev = tempClean->prev;
    //                  printf("Clean Page Removed\n");
                  }else if (tempClean->prev == NULL){//Removes the head clean page
                    if (headClean->next != NULL) {
                      ptr = headClean->next;  //Removes head Clean.
                      ptr->prev = NULL;
                      headClean = ptr;
                    }else {
                      strncpy(headClean->pg, "\0", 5);
                      headClean->dirty = 0;
                      headClean->prev = NULL;
                      headClean->next = NULL;
                    }
                    memory--;
                  }else if (tempClean->next == NULL) { //Removes the last clean page
                    tempClean->prev->next = NULL;
                  }
                  //Runs FIFO to add the new page.
                  if (headB->dirty == 1) {
                    if (memory >= nframes) {
                      ptr2 = headDirty->next;
                      headDirty = ptr2;
                      memory--;
                    }
                    add2end(headB->pg, rw, headDirty);
                    memory++;
                  }else if (headB->dirty == 0) {
                    if (memory >= nframes) {
                      ptr2 = headClean->next;
                      headClean = ptr2;
                      memory--;
                    }
                    add2end(headB->pg, rw, headClean);
                    memory++;
                  }
                  if (headB->next != NULL) {
                    ptr = headB->next;  //Removes head B.
                    ptr->prev = NULL;
                    headB = ptr;
                  }else {
                    strncpy(headB->pg, "\0", 5);
                    headB->dirty = 0;
                    headB->prev = NULL;
                    headB->next = NULL;
                  }
                  memory--;
                  add2end(addr, rw, headB);
                  memory++;

                }else {//If page is not found in clean or dirty, take first clean if none, first dirty, if none, add.
    //                printf("Not Found in Clean or Dirty\n");
    //                printf("%d\n", memory);
                  memory--;
                  if (strncmp(headClean->pg, "\0", 5) != 0) {
    //                  printf("Added to clean\n");
                    if (headClean->next != NULL) {
                      ptr = headClean->next;  //Removes head Clean.
                      ptr->prev = NULL;
                      headClean = ptr;
                    }else {
                      strncpy(headClean->pg, "\0", 5);
                      headClean->dirty = 0;
                      headClean->prev = NULL;
                      headClean->next = NULL;
                    }
                    memory--;
                    pageFault++;
                  }else if (strncmp(headDirty->pg, "\0", 5) != 0) {
    //                  printf("Added to dirty\n");
                    if (headDirty->next != NULL) {
                      ptr = headDirty->next;  //Removes head B.
                      ptr->prev = NULL;
                      headDirty = ptr;
                    }else {
                      strncpy(headDirty->pg, "\0", 5);
                      headDirty->dirty = 0;
                      headDirty->prev = NULL;
                      headDirty->next = NULL;
                    }
                    memory--;
                    pageFault++;
                    writes++;
                  }else {
                    if (headB->dirty == 1) {
                      if (memory >= nframes) {
                        ptr2 = headDirty->next;
                        headDirty = ptr2;
                        memory--;
    //                      printf("Dirty sent to disk\n");
                      }
    //                    printf("%s\n", headB->pg);
                      add2end(headB->pg, rw, headDirty);
                    }else if (headB->dirty == 0) {
                      if (memory >= nframes) {
                        ptr2 = headClean->next;
                        headClean = ptr2;
                        memory--;
    //                      printf("Clean sent to disk\n");
                      }
    //                    printf("%s\n", headB->pg);
                      add2end(headB->pg, rw, headClean);
                    }
                  }
                  if (headB->next != NULL) {
                    ptr = headB->next;  //Removes head B.
                    ptr->prev = NULL;
                    headB = ptr;
                  }else {
                    strncpy(headB->pg, "\0", 5);
                    headB->dirty = 0;
                    headB->prev = NULL;
                    headB->next = NULL;
                  }
                  add2end(addr, rw, headB);
                  memory++;
                }
              }
            }
          }
        }
        if (debug == 1) { //If debug is true, prints the page.
          printf("\nFIFO(A):  [ ");
          tempA = headA;
          while (tempA != NULL) {
            printf("%s-%d ", tempA->pg, tempA->dirty);
            tempA = tempA->next;
          }
          printf("]\nFIFO(B):  [ ");
          tempB = headB;
          while (tempB != NULL) {
            printf("%s-%d ", tempB->pg, tempB->dirty);
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

      printf("\nFIFO(A):  [ ");
      tempA = headA;
      while (tempA != NULL) {
        printf("%s-%d ", tempA->pg, tempA->dirty);
        tempA = tempA->next;
      }
      printf("]\nFIFO(B):  [ ");
      tempB = headB;
      while (tempB != NULL) {
        printf("%s-%d ", tempB->pg, tempB->dirty);
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

      printf("Total memory frames: %d\nEvents in trace: %d\nTotal disk read: %d\nTotal disk writes: %d\n\n", nframes, count, pageFault, writes);
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
