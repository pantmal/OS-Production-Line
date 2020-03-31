//Header file for Shared Memory

#pragma once

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


typedef struct part{ //The definition of the Part that will be used in the main program
    int id;
    int type;
    int created;
    int painted;
    int checked;
    long double timea;
    long double start1;
    long double start2;
    long double start3;
}Part;


int SharedCreate(key_t); //Creating shared memory 

Part *SharedAttach(int); //Getting a pointer to the shared memory segment

int SharedDetach(Part*); //Detaching the shared segment

int SharedDelete(int); //Deleting the shared memory segment
