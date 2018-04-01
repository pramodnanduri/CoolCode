#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
//Constants to initialize SliceTime, True, False.
#define TRUE 1
#define FALSE 0
#define SLICE 10 //The slicetime

//Linked list  to hold the task data
struct task {
    int pid; //The pid of the process
    int burst; //Burst time of the process
    struct task *next; //Pointer to hold the next node
} task;

/**
 First come first serve logic below. Takes two inputs
 n : Size of the linked list
 inp : Actual Linkedlist pointer
 */
void fcfs(int n, struct task *inp) {
    
    //Array to hold the waiting time of the processes
    int waitTime[n];
    struct task *node = inp;
    struct task *prev = NULL;
    int i = 0;
    
    //Calculate the Wait time for each process
    for (i = 0; i < n ; i++ ) {
        if(i == 0) {
            //Initial process wait time is zero.
            waitTime[i] = 0;
        } else {
            //WaitTime  = Burst time of previous process + wait time of previous process.
            int preBurst = prev == NULL ? 0 : prev->burst;
            waitTime[i] =  preBurst + waitTime[i-1];
        }
        //Tracking the previous node and traversing to the next pointer in the list.
        prev = node;
        node = node->next;
    }
    
    int totalWaitTime = 0;
    //Iterate each process and print the execution order of the processes
    for (i = 0; i < n; i++) {
        totalWaitTime += waitTime[i];
        printf("P%d ", i);
    }
    //Calculate the average wait time
    float avgWaitTime = (float)totalWaitTime / (float)n;
    printf("\nAverage wait time: %f", avgWaitTime);
    
    node = inp;
    int tat = 0; //Initialize the turn around time to zero.
    //Caculate the total turn around time of all the processes.
    for (i = 0; i < n ; i++) {
        //Turn around time = Burst time + wait time
        tat += node->burst + waitTime[i];
        node = node->next;
    }
    //Average turn around time
    float avgTat = (float) tat / (float) n;
    printf("\nAverage turnaround time: %f\n", avgTat);
}

/**
 Round robin logic below. Takes two inputs
 n : Size of the linked list
 inp : Actual Linkedlist pointer
 */
void roundRobin(int n, struct task *inp)
{
    int waitTime[n];
    int remainingTime[n];
    struct task *node = inp;
    int i = 0;
    //Initializing Remaining time with node burst time
    for (i = 0 ; i < n ; i++) {
        remainingTime[i] =  node->burst;
        node = node->next;
    }
    
    int time = 0;
    /*
     Since it is round robin and slice time is 10,
     iterate all the processes till each ones burst time is complete.
    */
    while (TRUE) {
        int done = TRUE;
        node = inp;
        for (i = 0 ; i < n; i++) { // Traverse all processes one by one repeatedly
            //While consuming , if remaining time to burst is still > 0 ,
            //then consume and reduce remaining time
            if (remainingTime[i] > 0) {
                done = FALSE;
                if (remainingTime[i] > SLICE) {
                    time += SLICE;
                    remainingTime[i] -= SLICE;
                    printf("P%d ", i);
                } else {
                    //When all the remaining time is consumed,
                    //calculate the time and wait time and save them
                    printf("P%d ", i);
                    time = time + remainingTime[i];
                    waitTime[i] = time - node->burst;
                    remainingTime[i] = 0;
                }
            }
            //Traverse the next pointer
            node = node->next;
        }
        if (done) {
            break;
        }
    }
    
    //Iterate each process and compute total wait time
    int totalWaitTime = 0;
    for (i = 0; i < n; i++) {
        totalWaitTime += waitTime[i];
    }
    //Calculate the average wait time
    float avgWaitTime = (float)totalWaitTime / (float)n;
    printf("\nAverage wait time: %f", avgWaitTime);
    
    int tat[n];
    node = inp;
    int tatime = 0;
    //Calculating the turnaround time
    for (i = 0; i < n ; i++) {
        tat[i] = node->burst + waitTime[i];
        node = node->next;
        tatime += tat[i];
    }
    //Calculating the average turn around time
    float avgTat = (float) tatime / (float) n;
    printf("\nAverage turnaround time: %f\n", avgTat);
}

int main(int argc, char **argv) {
    //Error handling to check minimum 2 commandline arguments (excluding the first one)
    if(argc != 3) {
        printf("Usage: ./schedule <option> <filename>");
        printf("\nAvailable options:\n\t0: FCFS\n\t1: Round robin\n");
        return 0;
    }
    
    //Convert string to integer
    //int option = atoi(argv[1]);
    char *nptr = argv[1];
    char *endptr = NULL;
    long option = strtol(nptr, &endptr, 10);
    //Error handling for command line options.
    if (option < 0  || option > 1 || errno == EINVAL) {
        printf("Usage: ./schedule <option> <filename>");
        printf("\nAvailable options:\n\t0: FCFS\n\t1: Round robin\n");
        return 0;
    }
    
    //Open the input file
    FILE *input = fopen(argv[2],"r");
    if(input == NULL) {
        printf("Filepath not found! Please give proper data path!\n");
        return 0;
    }
    struct task *list = NULL;
    int n = 0;
    int burst_time;
    struct task *current = list;
    
    //Read each line in the input file
    while(fscanf(input,"%d",&burst_time)!=EOF){
        //Create the node in the linked list
        struct task *node = (struct task*)malloc(sizeof(struct task));
        node->burst = burst_time;
        node->next = NULL;
        
        //Head node
        if (list == NULL) {
            list = node;
            current = list;
        }
        //Traverse next node
        current->next = node;
        current = node;
        n++;
    }
    //Call respective option
    if (option == 0) {
        fcfs(n, list);
    } else {
        roundRobin(n, list);
    }
    return  0;
}



