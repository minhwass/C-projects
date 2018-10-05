
#include<stdio.h>
#include<stdlib.h>

void producer(){

	FILE *d;
	FILE *t;


	FILE *mydata;
	mydata = fopen("mydata.txt", "rt");

	char input;
	
	//keep the program running until breaking
	while(1){
		
		//busy wait until the TURN is 0
		while(fgetc(t=fopen("TURN.txt","rt"))=='1') fclose(t);
		fclose(t);

		input = fgetc(mydata);

		//busy wait until DATA exists		
		while((d=fopen("DATA.txt","wt"))==NULL) fclose(d);
				
		//break out of the loop if reached end of mydata.txt
		if(feof(mydata)){
			
			//put null in DATA so that consumer can receive the data and finish it's job
			fputc('\0', d);

			fclose(d);
			
			//turn the job over to consumer
			FILE *turn = fopen("TURN.txt","wt");

			fputc('1',turn);

			fclose(turn);


			break;

		}

		//put the character read from mydata.txt into DATA
		fputc(input, d);

		fclose(d);
		
		//turn the job over to consumer
		FILE *turn = fopen("TURN.txt","wt");

		fputc('1',turn);

		fclose(turn);
		


	}

	fclose(mydata);
	




}
