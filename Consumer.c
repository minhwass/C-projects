#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void consumer()
{
	char c='a';
	FILE *d;
	FILE *t;
	
//need to add a while loop//need to stop when the producer finishes 
	
	while(c!='\0'){	
			
		

		while(fgetc(t=fopen("TURN.txt","rt"))=='0') fclose(t); 
		fclose(t);
		
				
		
		while((d=fopen("DATA.txt","rt"))==NULL) fclose(d); //busy wait
		//print the character read from DATA.TXT
		//d=fopen("DATA.txt","rt");
		c=fgetc(d);
		
	
		fflush(stdout);

		fclose(d);	
		
		//'\0' signals that the producer ended its job 
		if(c=='\0'){
			//fclose(d);
		 	break;
		}
		
		printf("%c",c);
		
		
		t=fopen("TURN.txt","wt");
		fputc('0',t);
		fclose(t);
		

	}
	t=fopen("TURN.txt","wt");
        fputc('0',t);
        fflush(stdout);
	fclose(t);

}


