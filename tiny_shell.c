#define _GNU_SOURCE
//#define _POSIX_C_SOURCE 199309
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sched.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>

//global variables 
//fifo and stream
char * fifo; 
int iomode;
double total_time = 0;
int counter = 0;

int clone_function(void * arg){
       // printf("\nInfo: this code is running under child process .\n");
        char *line = (char*)arg;
	 execl("/bin/sh","sh","-c",line,(char *)NULL);
        return 0;
}

int my_system(char *line){
	int status;
	
	clock_t start,finish;
	double time_used;
	double avg_time;

	#ifdef FORK
	start = clock();
        if (fork () ==0){
                ////runs the command you passed as the argument to the call  
                execl("/bin/sh","sh","-c",line,(char *)NULL);
        }
        wait(&status);
	finish = clock();
	time_used = ((double) (finish - start)) / CLOCKS_PER_SEC;
	total_time += time_used;
	counter++;
	avg_time = total_time/counter;
	printf("clock time used: %f \n",time_used);
	printf("avg time used: %f \n",avg_time);

           
	#elif VFORK

	start = clock();     

        if (vfork() ==0 ) {
                execl("/bin/sh","sh","-c",line,(char *)NULL);
        }
        wait(&status);
	finish = clock();
        time_used = ((double) (finish - start)) / CLOCKS_PER_SEC;
	total_time += time_used;
        counter++;
        avg_time = total_time/counter;
        printf("clock time used: %f \n",time_used);
        printf("avg time used: %f \n",avg_time);

	#elif CLONE
 
	start = clock();
	//allocate stack 
	void *stack = malloc(1024*1024);
	void *stackTop;
	if(stack == NULL){
		puts("stack allocation failed.");
		exit(EXIT_FAILURE);
	}
	stackTop = stack + (1024*1024); 	
	 puts("stack allocated");
	 
	int pid = clone(clone_function,stackTop,CLONE_FS|SIGCHLD,line);
	if (pid < 0) {
		printf("Error: unable to create the child process.");
		exit(EXIT_FAILURE);
	}
	wait(&status);
	finish = clock();
        time_used = ((double) (finish - start)) / CLOCKS_PER_SEC;


        total_time += time_used;
        counter++;
        avg_time = total_time/counter;
        printf("clock time used: %f \n",time_used);
        printf("avg time used: %f \n",avg_time);

	free(stack);

	
	#elif PIPE    
	int fd;
		
	if (fork() == 0){
		
		//if iomode = 1, write (the input)
		if(iomode == 1){
			close(1);
			fd = open(fifo,O_WRONLY);
			if(fd<0){
				perror("pipe can't open for write");
			}
			printf("fd was %d",fd);
			execl("/bin/sh","sh","-c",line,(char *)NULL);
		
		}	
		//if iomode = 0, read (the output) 
		else if(iomode == 0){
			close(0);
			fd = open(fifo,O_RDONLY);
			if(fd<0) {
				perror("pipe can't open for read.");
			}
			execl("/bin/sh","sh","-c",line,(char*)NULL);

        	}
	}
	wait(&status);
	
	#else 
	#endif
	
}



int main(int argc, char *argv[]){
	size_t charnums;
	size_t length=0;
	char *line = NULL;
	int status;
	int fd;	
	

	//fifo
	fifo=argv[1];
	iomode = atoi(argv[2]);
	
	
	while(1){			
		printf("Shell>>");
		charnums = getline(&line,&length,stdin);
		if (strcmp(line,"exit\n")==0){
			exit(0);
		}	
		if (strlen(line) > 1){		
			int value = my_system(line);
		}
		else exit(0);
	}
}
