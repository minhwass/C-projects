//Student  name: Minhwa Song
//Student  ID: 260622284
//Partner's name: John Kim
//Partner's  ID: 260732804

#include <stdlib.h> 
#include <unistd.h> 
#include<stdio.h>
#include "header.h"


int main(){
		
	int pid=fork();
	if(pid==-1) exit(1);
	if(pid==0){producer();}
	if(pid!=0){consumer();}
	

	
	return 0;
}
