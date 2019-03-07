if (strncmp(addr, "3", 1) == 0) {
  //printf("Process A Executed\n");
  procA++;

}else {
  //printf("Process B Executed\n");
  procB++;
}

if (strncmp(addr, tempstr, 1) == 0) {
  printf("Process Check Initialized.\n");
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
      e++;
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
}
if (debug == 1) { //If debug is true, prints the page.
  printf("\nFIFO(A):\t[ ");
  tempA = headA;
  while (tempA != NULL) {
    printf("%s ", tempA->pg);
    tempA = tempA->next;
  }
  printf("]\nFIFO(B):\t[ ");
  tempB = headB;
  while (tempB != NULL) {
    printf("%s ", tempB->pg);
    tempB = tempB->next;
  }
  printf("]\nClean:\t[ ");
  tempClean = headClean;
  while (tempClean != NULL) {
    printf("%s ", tempClean->pg);
    tempClean = tempClean->next;
  }
  printf("]\nDirty:\t[ ");
  tempDirty = headDirty;
  while (tempDirty != NULL) {
    printf("%s ", tempDirty->pg);
    tempDirty = tempDirty->next;
  }
  printf("]\n\n");

  printf("Total memory frames: %d\nEvents in trace: %d\nTotal disk read:%d\nTotal disk writes: %d\n\n", nframes, count, pageFault, writes);
}

if (strncmp(addr, "3", 1) == 0) {
  printf("Process Check Initialized.\n");
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
    tempA = headA;
    for (j = 0; j < rss && tempA != NULL; j++) { //Loops through the page table.
      //strncpy(temp, addr, 5); //Stores the page temporarily.
      //strncpy(tempstr, tempA->pg, 5);
      printf("%s\n", tempA->pg);
      //printf("%s\n", tempstr);
      printf("Equality Check\n");
      if (strncmp(addr, tempA->pg, 5) == 0) { //If the page exists.
        printf("Equality Check Success T\n");
        exists = 1;
        if (rw == 'W') { //Checks if the page needs to be made dirty.
          tempA->dirty = 1;
        }
        break;
      }else {
        printf("Equality Check Success F\n");
        exists = 0;
      }
      tempA = tempA->next;
    }
    //If the page does not exist and the page table is not full
    if (exists == 0) {
      //strncpy(fifoA[i], addr, 5);
      add2end(addr, rw, headA); //Adds the new page to the end.
      e++;
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
}

if (debug == 1) { //If debug is true, prints the page.
  //printf("%s Initialized.\n", &addr);
  count++;
  //strcpy(tempstr, "3");
  //printf("%s Initialized.\n", tempstr);

  printf("]\n\n");
  printf("Total memory frames: %d\nEvents in trace: %d\nTotal disk read:%d\nTotal disk writes: %d\n\n", nframes, count, pageFault, writes);
}
