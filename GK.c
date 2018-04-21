#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define PID_GMIN 100
#define PID_GMAX 1000
#define MNUM_THREADS 100
#define Ma_SLEEP_SECS 15


int *pid;
int PID_Mi,PID_Ma,NUM_THREADS,SIZE;


int allo_map();
int allo_pid();
void release_pid(int pid);



void release_pid(int pid1) 
{
	pid[pid1 - PID_Mi] = 0;
}


pthread_mutex_t lock;




void *thread_fun(int arg) 
{
   int sleepTime = rand() % Ma_SLEEP_SECS;
   int waitTime=1;	
    int pid = -1;
    while(pid == -1) {
        pthread_mutex_lock(&lock);
        pid = allo_pid();
        if (pid == -1) 
	{
            printf("\n\n!!! Can't allocate PID . Due to full usage of PID !!!\t THREAD : %d has to wait for pid to release.\n",arg+1);
		sleep(waitTime);
	           }
        pthread_mutex_unlock(&lock);
    }

    printf("\n Thread: %d has taken PID: %i .\t PROCESSING for %i seconds\n",arg+1, pid, sleepTime);
    sleep(sleepTime);

    printf("\n Thread: %d has COMPLETED. \t PID: %i  has RELEASED.\n",arg+1, pid);
    release_pid(pid);

    return NULL;
}




int allo_map()
 {
	SIZE = (PID_Ma - PID_Mi + 1);
	pid = calloc(SIZE, sizeof(int));

	if (pid != NULL)
		return 1;

	return -1;
}


int allo_pid()
 {
	int i = 0;
	while (pid[i] != 0) 
	{
		i++;
	}
	if (i + PID_Mi > PID_Ma) {
		return -1;
	}
	pid[i] = 1;

	return i + PID_Mi;
}

int main()
 {
    int i;
printf("\nEnter Minimum and Maximum Value of PID :-\n\n");
printf("Minimum PID value : ");
scanf("%d",&PID_Mi);

printf("Maximum PID value : ");
scanf("%d",&PID_Ma);

printf("\n\nNow Enter number of thread you want to create : ");
scanf("%d",&NUM_THREADS);

if(PID_Mi < PID_GMIN || PID_Ma > PID_GMAX ||NUM_THREADS > MNUM_THREADS)
{
printf("\n YOU HAVE ENTER WRONG VALUES !!!\t Programme is terminating.\n");
return 1;
}


 printf("\n\n--------------------------- STARTING EXECUTION ----------------------------------\n\n");
    pthread_t thread_ids[NUM_THREADS];
  srand(time(NULL)); 

    pthread_mutex_init(&lock, NULL);

    if (allo_map() == -1) 
	{
        printf("Error: Could not initialize the pidmap. Terminating program...\n");
        return -1;
    	}

    for (i = 0; i < NUM_THREADS; i++) 
	{
        pthread_create(&thread_ids[i],NULL,thread_fun,i);
	sleep(1);
        }

    for (i = 0; i < NUM_THREADS; i++)	
	 {
        pthread_join(thread_ids[i],NULL);	
         }

    if (pid != NULL) 
	{
        free(pid);
    	}

    return 0;
}
