#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BFL 128
#define  RSIZ 1000

// Method findavg
// Find average of all numbers in the input file 
void findavg(char *filename){
    // read file (numbers.txt) with many numbers
    FILE *fp = NULL;
    double numArray[RSIZ];
    char buffer[BFL];
    int count = 0;
    double total = 0;

	// open and read file
    fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("Error opening file!!!\n");
        // exit program if the file pointer returns null
        exit(1);
    }

	// read line by line of file
	// find how many numbers in the file
	// find total of all these numbers
    while(fgets(buffer, BFL, fp) != NULL){
        double num = atof(buffer);
        numArray[count] = num;
        total += num;
        count++;
    }
	// print out average of all numbers (total/count)
    printf("\n%lf\n", (double) total/count);
}

int main(int argc, char* argv[]) {
    char *filename;
    int counter;
    printf("\nProgram Name is: %s\n", argv[0]);
	// when user enter ./avg from the command line only
    if(argc == 1){
        printf("\nNo input file!!!\n");
		exit(0);
    }
	// when user enter ./avg numbers from command line
    if(argc == 2){
        printf("\nNumber of Argument passed: %d", argc);
        for(counter = 0; counter < argc; counter++){
            if(counter == argc - 1){
                filename = argv[counter];
            }
            printf("\nargv[%d]: %s", counter, argv[counter]);
        }
    }
	// call function find the average of all numbers in the file
	findavg(filename);
    
    return 0;
}
