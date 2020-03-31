#include <sys/wait.h>
#include <unistd.h>
#include <sys/timeb.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "Semaphores.h"
#include "SharedMemory.h"

long double SmallestStartTime(float a , float b , float c){ //this function will return the smallest creation time of three parts 
	  if(a<b && a<c)										//it will be used to calculate the second average time
            return a;
      else if(b<a && b<c)
            return b;
      else
            return c;
}

int main (int argc, char* argv[]){

   	srand(time(0));  		//rand is initialized here for the 3 keys.
   
    int Y = 0;
   
	if(argc < 1){  				
        printf("Error of argc!");
    }
    else{
        Y = atoi(argv[1]);		//getting how many products we want
    }

   	int i,y,flag1,flag2,flag3,final_id1,final_id2,final_id3,finale;	//declaring variables. i will explain what each one does when they will be used
   	int n = 8;
   	pid_t pid;


   	key_t key1;		//the keys for the shared memories
   	key_t key2;
   	key_t key3;
		
	long double timea1=0.0;		//variables for calculating times
	long double fsum=0.0;	
	struct timespec c1start, c1end;
	struct timespec c2start, c2end;
	struct timespec c3start, c3end;
	struct timespec cfinal;
	
	
	int Shm1ID;		//arguments for shared memory creation
    Part *Shm1Ptr;

    int Shm2ID;
    Part *Shm2Ptr;
    
    int Shm3ID;
    Part *Shm3Ptr;
    
   	int Full1_ID, Empty1_ID, Mutex1_ID;	//the semaphores that will be used
   	int Full2_ID, Empty2_ID, Mutex2_ID;
   	int Full3_ID, Empty3_ID, Mutex3_ID;
   	int Complete1, Complete2, Complete3;
   	int Checker1, Checker2, Checker3;
   
   
   
   	key1=1000+(rand()%9000);		//creating the shared memories
   	if (key1 == -1) {                
        printf(" ftok error! \n");
            exit(1);
    }

    Shm1ID = SharedCreate(key1);                                 
    if (Shm1ID < 0) {
        printf("shmget error! \n");
        exit(1);
    }
    Shm1Ptr = SharedAttach(Shm1ID);
    if (Shm1Ptr == (Part *)(-1)){
        printf("shmat error! \n");
        exit(1);
    }
    
   	key2=1000+(rand()%9000);
   	if (key2 == -1) {                
        printf("ftok error! \n");
            exit(1);
    }

    Shm2ID = SharedCreate(key2);                                 
    if (Shm2ID < 0) {
        printf("shmget error!\n");
        exit(1);
    }
    Shm2Ptr = SharedAttach(Shm2ID);
    if (Shm2Ptr == (Part *)(-1)){
        printf("shmat error!\n");
        exit(1);
    }
    
    key3=1000+(rand()%9000);
   	if (key3 == -1) {                
        printf("ftok error! \n");
            exit(1);
    }

    Shm3ID = SharedCreate(key3);                                 
    if (Shm3ID < 0) {
        printf("shmget error! \n");
        exit(1);
    }
    Shm3Ptr = SharedAttach(Shm3ID);
    if (Shm3Ptr == (Part *)(-1)){
        printf("shmat error! \n");
        exit(1);
    }
	    
    Empty1_ID = SemCreate((key_t)123456,1,1);     //creating and initializing the semaphores                   
    if (Empty1_ID < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    Full1_ID = SemCreate((key_t)45656,1,0);                        
    if (Full1_ID < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    Mutex1_ID = SemCreate((key_t)78956,1,1);                        
    if (Mutex1_ID < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    Empty2_ID = SemCreate((key_t)1234789,1,1);                       
    if (Empty2_ID < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    Full2_ID = SemCreate((key_t)7489789,1,0);                        
    if (Full2_ID < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    
    Mutex2_ID = SemCreate((key_t)4586789,1,1);                        
    if (Mutex2_ID < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    Empty3_ID = SemCreate((key_t)7891234,1,1);                        
    if (Empty3_ID < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    Full3_ID = SemCreate((key_t)7489666,1,0);                       
    if (Full3_ID < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    
    Mutex3_ID = SemCreate((key_t)4866789,1,1);                        
    if (Mutex3_ID < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    Complete1 = SemCreate((key_t)9066666,1,1);                        
    if (Complete1 < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    Complete2 = SemCreate((key_t)90007777,1,1);                       
    if (Complete2 < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    Complete3 = SemCreate((key_t)90069999,1,1);                        
    if (Complete3 < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    Checker1 = SemCreate((key_t)9899999,1,0);                        
    if (Checker1 < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    Checker2 = SemCreate((key_t)6867777,1,0);                        
    if (Checker2 < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    Checker3 = SemCreate((key_t)5869999,1,0);                        
    if (Checker3 < 0){
        printf("semget error! \n");
        exit(0);
    }
    
	
    		
   	for (i=0; i<n; i++){				//we will use a loop that will create 8 processes. each time it calls fork to create a process. 
       pid=fork();						
       if(pid == -1){
           printf("Error in fork() \n");
           return 1;
       }
       if(pid == 0){					//if we are on a child, then we will check the value of the counter. 
       	    if(i == 0){					//if it's 0 then we will do stuff for the first process. if it's 1 it will do stuff for the second process, and so on.
       	    	//Constructor 1 Process
				
				srand(getpid());	//we initialize rand on each constructor process to generate a random 4-digit number.

				y=0;
				while(y<Y){			//each process runs for Y times since we want Y products.  
									//since the painter and assembler will do stuff for every type of part they will run for 3Y
									
				Part part1;			//Part of type 1 is created and its members are initialized. the defintion of type Part is in the SharedMemory.h file. 
				part1.id=rand()%9000 + 1000;	
				part1.created=1;
				part1.painted=0;
				part1.checked=0;
				part1.type=1;
	
				struct timespec timecr1 = {0,1000000L}; //Every constructor process runs for 1 ms.
				nanosleep(&timecr1,NULL);
				
				clock_gettime(CLOCK_MONOTONIC, &c1start); //here we get its creation time
				
				SemDown(Empty1_ID,0);	//the Full, Empty and Mutex semaphores that I used are based on the bounded buffer problem.  
        		SemDown(Mutex1_ID,0);  //since we need 3 shared memories, these 3 semaphores are used for each shared mem segment.
				//when a process has to write stuff to the shared memory it has to be empty. So we decrement an Empty Semaphore. 
        		//Mutex ensures that other processes that may be ready will not intervene 
				
				clock_gettime(CLOCK_MONOTONIC, &c1end); //the end clock indicates when a part has reached the painter
        		long double create_start=c1start.tv_sec*1000.0f + c1start.tv_nsec/1000000.0f; //getting start and end time from timespec
        		long double create_end=c1end.tv_sec*1000.0f + c1end.tv_nsec/1000000.0f; //and making the necessary conversions
        		long double temp1=create_end - create_start;//temp1 variable keeps how much time passed from a part's creation to the time it reached the painter
        		timea1=timea1+temp1; //timea1 is the sum that will be used to calculate the average time. it will be passed in the timea argument of type Part.
        		
    
        		Shm1Ptr->id=part1.id;			//Writing to the shared memory segment
        		Shm1Ptr->created=part1.created;
 			    Shm1Ptr->painted=part1.painted;
				Shm1Ptr->checked=part1.checked;       		
        		Shm1Ptr->type=part1.type;
        		Shm1Ptr->timea=timea1;
        		Shm1Ptr->start1=create_start;	//the creation start time is passed because it will be used for the second average time 
        		
        		
        		SemUp(Mutex1_ID,0);		//Critical section is over, you may unblock all the processes that participate.
        		SemUp(Full1_ID,0);		//A process may read from the shared memory only when another process has written something. 
        								//Since this process has written succesfully the Full semaphore is incremented.
				y++;
        	    }
        	    
    		}
    		else if(i==1){
    			//Constructor 2 Process
				
				srand(getpid());

				y=0;
				while(y<Y){
				
				Part part2;
				part2.id=rand()%9000 + 1000;
				part2.created=1;
				part2.painted=0;
				part2.checked=0;
				part2.type=2;
				
				struct timespec timecr2 = {0,1000000L};
				nanosleep(&timecr2,NULL);
				
				clock_gettime(CLOCK_MONOTONIC, &c2start);
				
				SemDown(Empty1_ID,0);
        		SemDown(Mutex1_ID,0);
        		
        		
        	
				clock_gettime(CLOCK_MONOTONIC, &c2end);
        		long double create_start=c2start.tv_sec*1000.0f + c2start.tv_nsec/1000000.0f;
        		long double create_end=c2end.tv_sec*1000.0f + c2end.tv_nsec/1000000.0f;
        		long double temp2=create_end - create_start;
        		timea1=timea1+temp2;
        		
        		Shm1Ptr->id=part2.id;
        		Shm1Ptr->created=part2.created;
 			    Shm1Ptr->painted=part2.painted;
				Shm1Ptr->checked=part2.checked;       		
        		Shm1Ptr->type=part2.type;
        		Shm1Ptr->timea=timea1;
        		Shm1Ptr->start2=create_start;
        		
        		
        		SemUp(Mutex1_ID,0);
        		SemUp(Full1_ID,0);
        		
        		
				y++;
        	    }
        	    
			}
			else if(i==2){
				//Constructor 3 Process
				
				srand(getpid());
    			
				y=0;
				while(y<Y){
					
				Part part3;
				part3.id=rand()%9000 + 1000;
				part3.created=1;
				part3.painted=0;
				part3.checked=0;
				part3.type=3;
				
				struct timespec timecr3 = {0,1000000L};
				nanosleep(&timecr3,NULL);
				
				clock_gettime(CLOCK_MONOTONIC, &c3start);
				
				SemDown(Empty1_ID,0);
        		SemDown(Mutex1_ID,0);
        		
				
				clock_gettime(CLOCK_MONOTONIC, &c3end);
        		long double create_start=c3start.tv_sec*1000.0f + c3start.tv_nsec/1000000.0f;
        		long double create_end=c3end.tv_sec*1000.0f + c3end.tv_nsec/1000000.0f;
        		long double temp3=create_end - create_start;
        		timea1=timea1+temp3;
        		
        		Shm1Ptr->id=part3.id;
        		Shm1Ptr->created=part3.created;
 			    Shm1Ptr->painted=part3.painted;
				Shm1Ptr->checked=part3.checked;       		
        		Shm1Ptr->type=part3.type;
        		Shm1Ptr->timea=timea1;
        		Shm1Ptr->start3=create_start;
        		
        		
        		SemUp(Mutex1_ID,0);
        		SemUp(Full1_ID,0);
        		
				y++;
        	    }
        	    

			}
    		else if(i == 3){
        		//Painter Process
        		
        		y=0;
        		while(y<3*Y){
        			Part constr_paint; //this variable of type Part will keep the stuff we"ll read from the shared memory
        			SemDown(Full1_ID,0); //The painter will read only when the shared memory is full
        		    SemDown(Mutex1_ID,0);
        			if(Shm1Ptr->type==1){	//if the part is type 1 read the shared memory segment and indicate that it has been painted. 
        					
						constr_paint.id=Shm1Ptr->id;
        				constr_paint.created=Shm1Ptr->created;
        				constr_paint.painted=Shm1Ptr->painted;
        				constr_paint.checked=Shm1Ptr->checked;
        				constr_paint.type=Shm1Ptr->type;
        				constr_paint.timea=Shm1Ptr->timea;
        				constr_paint.start1=Shm1Ptr->start1;
        				constr_paint.start2=Shm1Ptr->start2;
        				constr_paint.start3=Shm1Ptr->start3;
        				
        				constr_paint.painted=1;
        				struct timespec k1 = {0,5000000L}; //type 1 parts will be painted for 5 ms
						nanosleep(&k1,NULL);
        			
        			}
					else if(Shm1Ptr->type==2){	//same thing for type 2. these ones will be painted for 6 ms
					
						constr_paint.id=Shm1Ptr->id;
        				constr_paint.created=Shm1Ptr->created;
        				constr_paint.painted=Shm1Ptr->painted;
        				constr_paint.checked=Shm1Ptr->checked;
        				constr_paint.type=Shm1Ptr->type;
        				constr_paint.timea=Shm1Ptr->timea;
        				constr_paint.start1=Shm1Ptr->start1;
        				constr_paint.start2=Shm1Ptr->start2;
        				constr_paint.start3=Shm1Ptr->start3;
        				
        				constr_paint.painted=1;
        				struct timespec k2 = {0,6000000L};
						nanosleep(&k2,NULL);
        				      				
					}
					else if(Shm1Ptr->type==3){ //same thing for type 3. these ones will be painted for 7 ms
						
						constr_paint.id=Shm1Ptr->id;
        				constr_paint.created=Shm1Ptr->created;
        				constr_paint.painted=Shm1Ptr->painted;
        				constr_paint.checked=Shm1Ptr->checked;
        				constr_paint.type=Shm1Ptr->type;
        				constr_paint.timea=Shm1Ptr->timea;
        				constr_paint.start1=Shm1Ptr->start1;
        				constr_paint.start2=Shm1Ptr->start2;
        				constr_paint.start3=Shm1Ptr->start3;
        				
        				constr_paint.painted=1;
        				struct timespec k3 = {0,7000000L};
						nanosleep(&k3,NULL);
						       				
					}
					SemUp(Mutex1_ID,0);  
        			SemUp(Empty1_ID,0); //The Empty semaphore is incremented so another process can write. 
        									
        			SemDown(Empty2_ID,0);	//Now the painter must write to second shared memory. The semaphores work as explained above.
        			SemDown(Mutex2_ID,0);
        			if(constr_paint.type==1){	//if what we read was type 1 write it to the second shared memory  
        				
        				Shm2Ptr->id=constr_paint.id;
        				Shm2Ptr->created=constr_paint.created;
        				Shm2Ptr->painted=constr_paint.painted;
        				Shm2Ptr->checked=constr_paint.checked;
        				Shm2Ptr->type=constr_paint.type;
        				Shm2Ptr->timea=constr_paint.timea;
        				Shm2Ptr->start1=constr_paint.start1;
        				Shm2Ptr->start2=constr_paint.start2;
        				Shm2Ptr->start3=constr_paint.start3;
        				
        			
        				SemUp(Checker1,0);	//The Checker semaphore is used because each checker has to do stuff for specific part types
					}						//if we don't use this semaphore then every checker will read random part types
					else if(constr_paint.type==2){ //same thing for types 2 and 3
						
        				Shm2Ptr->id=constr_paint.id;
        				Shm2Ptr->created=constr_paint.created;
        				Shm2Ptr->painted=constr_paint.painted;
        				Shm2Ptr->checked=constr_paint.checked;
        				Shm2Ptr->type=constr_paint.type;
        				Shm2Ptr->timea=constr_paint.timea;
        				Shm2Ptr->start1=constr_paint.start1;
        				Shm2Ptr->start2=constr_paint.start2;
        				Shm2Ptr->start3=constr_paint.start3;
        				
        				
        				SemUp(Checker2,0);
					}
					else if(constr_paint.type==3){
						
        				Shm2Ptr->id=constr_paint.id;
        				Shm2Ptr->created=constr_paint.created;
        				Shm2Ptr->painted=constr_paint.painted;
        				Shm2Ptr->checked=constr_paint.checked;
        				Shm2Ptr->type=constr_paint.type;
        				Shm2Ptr->timea=constr_paint.timea;
        				Shm2Ptr->start1=constr_paint.start1;
        				Shm2Ptr->start2=constr_paint.start2;
        				Shm2Ptr->start3=constr_paint.start3;
        				
        				
        				SemUp(Checker3,0);
					}
					SemUp(Mutex2_ID,0);
        			SemUp(Full2_ID,0);
					
        		y++;
			    }
			
    		}
      		else if(i == 4){
      			//Checker 1 Process
        		
				y=0;
        		while(y<Y){
        		Part paint_checker; //this variable will keep the part that we will read from the second shared memory
       				SemDown(Checker1,0); //Initially a checker process is blocked and may begin only when it has to read stuff for its part type.
        			SemDown(Full2_ID,0);
        			SemDown(Mutex2_ID,0);
        	
        			if(Shm2Ptr->type==1){ //If it's type 1 read the shared mem segment and indicate that is has been checked. 
        
        				paint_checker.id=Shm2Ptr->id;
        				paint_checker.created=Shm2Ptr->created;
        				paint_checker.painted=Shm2Ptr->painted;
        				paint_checker.checked=Shm2Ptr->checked;
        				paint_checker.type=Shm2Ptr->type;
        				paint_checker.timea=Shm2Ptr->timea;
        				paint_checker.start1=Shm2Ptr->start1;
        				paint_checker.start2=Shm2Ptr->start2;
        				paint_checker.start3=Shm2Ptr->start3;
        				
        				paint_checker.checked=1;
        				struct timespec d1 = {0,2000000L};  //type 1 parts will be checked for 2 ms
						nanosleep(&d1,NULL);
        	
        			}
        		SemUp(Mutex2_ID,0);
				SemUp(Empty2_ID,0);
			
				
				SemDown(Complete1,0);//The Complete semaphore works as follows:When a part of a specific type has reached the assembler block its checker process
				SemDown(Empty3_ID,0);//The Complete semaphores (3 for each type) is incremnted only when a product has been completed.
				SemDown(Mutex3_ID,0);//This is done in order to prevent multiple parts of the same type reaching the assembler
				 			
				Shm3Ptr->id=paint_checker.id;			//Writing on the last shared memory
				Shm3Ptr->created=paint_checker.created;
				Shm3Ptr->painted=paint_checker.painted;
				Shm3Ptr->checked=paint_checker.checked;
				Shm3Ptr->type=paint_checker.type;
				Shm3Ptr->timea=paint_checker.timea;
				Shm3Ptr->start1=paint_checker.start1;
				Shm3Ptr->start2=paint_checker.start2;
				Shm3Ptr->start3=paint_checker.start3;
					
				SemUp(Mutex3_ID,0);
				SemUp(Full3_ID,0);
				
        		y++;	
				}
	
    		}
    		else if(i == 5){
        		//Checker 2 Process
        		
				y=0;
        		while(y<Y){
        		Part paint_checker;
        		SemDown(Checker2,0);
        		SemDown(Full2_ID,0);
        		SemDown(Mutex2_ID,0);
        			if(Shm2Ptr->type==2){
        				
        				
        				paint_checker.id=Shm2Ptr->id;
        				paint_checker.created=Shm2Ptr->created;
        				paint_checker.painted=Shm2Ptr->painted;
        				paint_checker.checked=Shm2Ptr->checked;
        				paint_checker.type=Shm2Ptr->type;
        				paint_checker.timea=Shm2Ptr->timea;
        				paint_checker.start1=Shm2Ptr->start1;
        				paint_checker.start2=Shm2Ptr->start2;
        				paint_checker.start3=Shm2Ptr->start3;
        				
        				paint_checker.checked=1;
        				struct timespec d2 = {0,3000000L};	//type 1 parts will be checked for 3 ms
						nanosleep(&d2,NULL);
        				
        			}
        		SemUp(Mutex2_ID,0);
				SemUp(Empty2_ID,0);
				
				
				SemDown(Complete2,0);
				SemDown(Empty3_ID,0);
				SemDown(Mutex3_ID,0);
					
				Shm3Ptr->id=paint_checker.id;
				Shm3Ptr->created=paint_checker.created;
				Shm3Ptr->painted=paint_checker.painted;
				Shm3Ptr->checked=paint_checker.checked;
				Shm3Ptr->type=paint_checker.type;
				Shm3Ptr->timea=paint_checker.timea;
				Shm3Ptr->start1=paint_checker.start1;
				Shm3Ptr->start2=paint_checker.start2;
				Shm3Ptr->start3=paint_checker.start3;
						
						
				SemUp(Mutex3_ID,0);
				SemUp(Full3_ID,0);
				
        		y++;
				}
				
    		}
    		else if(i == 6){
        		//Checker 3 Process 
        		
				y=0;
        		while(y<Y){
        		Part paint_checker;
        		SemDown(Checker3,0);
        		SemDown(Full2_ID,0);
       			SemDown(Mutex2_ID,0);
        			if(Shm2Ptr->type==3){
        				
        				
        				paint_checker.id=Shm2Ptr->id;
        				paint_checker.created=Shm2Ptr->created;
        				paint_checker.painted=Shm2Ptr->painted;
        				paint_checker.checked=Shm2Ptr->checked;
        				paint_checker.type=Shm2Ptr->type;
        				paint_checker.timea=Shm2Ptr->timea;
        				paint_checker.start1=Shm2Ptr->start1;
        				paint_checker.start2=Shm2Ptr->start2;
        				paint_checker.start3=Shm2Ptr->start3;
        				
        				paint_checker.checked=1;
        				struct timespec d3 = {0,4000000L}; //type 1 parts will be checked for 4 ms
						nanosleep(&d3,NULL);
        				
        			}
        		SemUp(Mutex2_ID,0);
				SemUp(Empty2_ID,0);

				
				SemDown(Complete3,0);
				SemDown(Empty3_ID,0);
				SemDown(Mutex3_ID,0);
						
				Shm3Ptr->id=paint_checker.id;
				Shm3Ptr->created=paint_checker.created;
				Shm3Ptr->painted=paint_checker.painted;
				Shm3Ptr->checked=paint_checker.checked;
				Shm3Ptr->type=paint_checker.type;
				Shm3Ptr->timea=paint_checker.timea;
				Shm3Ptr->start1=paint_checker.start1;
				Shm3Ptr->start2=paint_checker.start2;
				Shm3Ptr->start3=paint_checker.start3;
						
					
						
				SemUp(Mutex3_ID,0);
				SemUp(Full3_ID,0);
				
        		y++;
				}
				
    		}
    		else if(i == 7){
    			//Assembler Process
        		
        		y=0;
        		
				final_id1=0;	//the final id variables keep the ids that will be used to create the product.
				final_id2=0;
				final_id3=0;
					
        		while(y<3*Y){
					
					SemDown(Full3_ID,0);
					SemDown(Mutex3_ID,0); 
        			if(Shm3Ptr->type==1 && Shm3Ptr->created==1 && Shm3Ptr->painted==1 && Shm3Ptr->checked==1){ //A part type of type 1 has reached the assembler
        																								 //and it has passed correctly from every other process 
						final_id1=Shm3Ptr->id;
						flag1=1;				//the flags are used to create the product. 
					}							//when the value of all them are 1 this means we are ready to create the product
					else if(Shm3Ptr->type==2 && Shm3Ptr->created==1 && Shm3Ptr->painted==1 && Shm3Ptr->checked==1){ //same thing for types 2 and 3
        				
						final_id2=Shm3Ptr->id;
						flag2=1;
					}
					else if(Shm3Ptr->type==3 && Shm3Ptr->created==1 && Shm3Ptr->painted==1 && Shm3Ptr->checked==1){
        				
						final_id3=Shm3Ptr->id;
						flag3=1;
					}
					SemUp(Mutex3_ID,0);
					SemUp(Empty3_ID,0);
					
					if(flag1==1 && flag2==1 && flag3==1 ){ //We may now create a product
						finale=final_id1 + final_id2 + final_id3; //We add all the ids to create the product
						struct timespec ff = {0,7000000L};		  //Assembler takes 7 ms to do its work
						nanosleep(&ff,NULL);
						
						clock_gettime(CLOCK_MONOTONIC, &cfinal);
						long double cfinal_end=cfinal.tv_sec*1000.0f + cfinal.tv_nsec/1000000.0f; //in cfinal we get how much time a product took to be created 
						
						long double st =SmallestStartTime(Shm3Ptr->start1,Shm3Ptr->start2,Shm3Ptr->start3);//here we get which part had the smallest creation time 
						fsum= fsum + (cfinal_end - st);//Here we get how much time passed from the creation of the first part 'till its completion...
														//... and we add it to the final sum for the second average time
						printf("Final product is: %d and is comprised of: %d %d and %d \n",finale,final_id1,final_id2,final_id3);
						SemUp(Complete1,0); //Product has been created. Unblock all the checkers so we can create a new one. 
						SemUp(Complete2,0); 
						SemUp(Complete3,0);
						flag1=0;			//Flags are set to 0 for every new product
						flag2=0;
						flag3=0;
					
					}
				
				y++;
				}
			
				printf("Average time for reaching the painter in milliseconds is: %Lf \n",Shm3Ptr->timea/(3*Y));//Average time all parts took to reach the painter
				printf("Average time for completion of a product in milliseconds is: is %Lf \n",fsum/Y);//Average time a product took to be created.
				 
    		}
    	else{
        	printf("Something went REALLY wrong... \n"); //If for some reason a child was not created...
    	}
    	exit(0);	//Child process exits
        }
	}
           
       if (pid != 0) {	//All a father has to do is wait for his children to finish.
          
          for(i=0; i<8; i++){
          
	  int status;
	  pid_t child_pid;	
          child_pid = wait(&status);
          
       }

    }
    

    SemDel(Full1_ID);	//Deleting the semaphores
	SemDel(Empty1_ID);
	SemDel(Mutex1_ID);
	SemDel(Full2_ID);
	SemDel(Empty2_ID);
	SemDel(Mutex2_ID);
	SemDel(Full3_ID);
	SemDel(Empty3_ID);
	SemDel(Mutex3_ID);
	SemDel(Complete1);
	SemDel(Complete2);
	SemDel(Complete3);
	SemDel(Checker1);
	SemDel(Checker2);
	SemDel(Checker3);


    if (SharedDetach(Shm1Ptr) == -1) {       //Detaching and deleting Shared Memories  	         
        printf(" shmdt failed! \n");
        exit(1);
    }
    if (SharedDelete(Shm1ID) == -1) {
        printf(" shmctl(IPC_RMID) failed! \n");
        exit(1);
    }

    if (SharedDetach(Shm2Ptr) == -1) {                  
        printf(" shmdt failed! \n");
        exit(1);
    }
    if (SharedDelete(Shm2ID) == -1) {
        printf(" shmctl(IPC_RMID) failed! \n");
        exit(1);
    }

    if (SharedDetach(Shm3Ptr) == -1) {                  
        printf(" shmdt failed! \n");
        exit(1);
    }
    if (SharedDelete(Shm3ID) == -1) {
        printf(" 2 shmctl(IPC_RMID)! \n");
        exit(1);
    }
    

    return 0;
}


