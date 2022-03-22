#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BFL 128     // size for buffer
#define PS 2        // pipe size

// method check if file is empty or not
int isEmpty(FILE *file){
    char ch;
    int count = 0;
    do{
        ch = fgetc(file);
        if(ch >= '0' && ch <= '9'){
            count++;
        }
    }while(ch != EOF);

    if(count > 0)           // file is not empty
        return 0;
    return 1;               // file is empty
}

//method count how many number in file
double *findTotalAndCount(char *filename){
    // read file (numbers.txt) with many numbers
    FILE *fp = NULL;
    char buffer[BFL];
    double count = 0;
    double total = 0;
    static double result[PS];

    fp = fopen(filename, "r");
    if(fp == NULL){
        printf("Error opening file!!!\n");
        exit(1);
    }
    else{   // can open file
        if(isEmpty(fp) == 1){
            printf("File is empty!!!\n");
            result[0] = 0;
            result[1] = 0;
        }
        else{       // file is not empty
            fp = fopen(filename, "r");
            while (fgets(buffer, BFL, fp) != NULL) {
                double num = atof(buffer);
                total += num;
                count++;
            }
            result[0] = total;
            result[1] = count;
            fclose(fp);
        }
    }

    // return array of double total and count
    return result;
}

int main(int argc, char* argv[]) {
    // array of inputted file names
    char *listFilename[BFL];
    int counter;
    int fileNo = 0;
    printf("\nProgram Name is: %s\n", argv[0]);
    if(argc == 1){
        printf("No input file!!!\n");
        exit(0);
    }
        // save all input filename from argc into listFilename
    else if(argc >= 2){
        for(counter = 1; counter < argc; counter++){
            listFilename[fileNo] = argv[counter];
            fileNo++;
        }
    }

    // handle each of filename in listFilename
    if(fileNo == 1)      // only 1 input text file
    {
        double total_proVal, count_proVal;
        printf("%s\n", listFilename[fileNo - 1]);
        double *proVal = findTotalAndCount(listFilename[fileNo - 1]);
        total_proVal = proVal[0];
        count_proVal = proVal[1];
        printf("%lf\n", total_proVal/count_proVal);
    }
    else        // more than 2 input text file
    {
        double totalArray[fileNo];         //save all total numbers from files
        double countArray[fileNo];         // save all count numbers from files
        // exit status
        int stat;
        // array of pid
        pid_t pid[fileNo];
        // buffer to save information from child
        double buf[PS];
        // initialize pipe to send information between child and parent
        int pd[fileNo][PS];

        for(int i = 0; i < fileNo; i++){
            // make pipe connection
            pipe(pd[i]);
            // case pipe failed
            if(pipe(pd[i]) == -1){
                fprintf(stderr, "Pipe failed!\n");
                return 1;
            }
            if((pid[i] = fork()) == 0)
            {
                // get total and count from each file
                double *childVal = findTotalAndCount(listFilename[i]);
                buf[0] = childVal[0];       // total
                buf[1] = childVal[1];       // count
                // write total and count to buf to pd so that parent can read
                write(pd[i][1], buf, sizeof(buf));

                exit(100 + i);
            }
        }

        //using waitpid() and printing exit status
        for(int i = 0; i < fileNo; i++)
        {
            pid_t cpid = waitpid(pid[i], &stat, 0);
            if(WIFEXITED(stat))
            {
                printf("Child %d terminated with status: %d\n", cpid, WEXITSTATUS(stat));
                read(pd[i][0], buf, sizeof(buf));
                totalArray[i] = buf[0];         // save total value from child process to totalArray
                countArray[i] = buf[1];         // save count value from child process to countArray
            }
        }

        // use information from totalArray and countArray to find the average
        double sumTotal = 0;
        double sumCount = 0;
        for(int k = 0; k < fileNo; k++)
        {
            sumTotal += totalArray[k];
            sumCount += countArray[k];
        }
        // find average
        double finalResult = sumTotal/sumCount;
        printf("Final Average = %lf\n", finalResult);
    }
    return 0;
}
 
