
#include "SharedMemory.h"

int SharedCreate(key_t key){

    if(key<0){
      printf("Error in key! /n");
      return -1;
    }

    return shmget(key, sizeof(Part), IPC_CREAT | 0666);
}

Part* SharedAttach(int SharedID){
    return shmat(SharedID, (void *)0, 0);
}

int SharedDetach(Part* ShmPtr){
    return shmdt(ShmPtr);
}

int SharedDelete(int SharedID){
    return shmctl(SharedID, IPC_RMID, 0);
}
