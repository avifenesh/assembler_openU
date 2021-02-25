#include <stdio.h>   
#include <fcntl.h>   
#include <stdlib.h>  
#include <ctype.h>   
#include <math.h>   
#include <string.h>

#define MAXNAME 1000 /*the max length of file name*/
#define MAXLINE 80 /*max length of line*/

int firstPass(char line, int newFile); /*first pass method*/
int commOrDat(void); /*check if its data or command*/
int secondPass(int i, int IC); /*second pass method*/
int buildFiles(char *fname, int IC, int DC); /*file builder method*/
int freeTab(void); /*free the symbol table*/

/*main open the files, call pass  one and then pass two and if everything allright call build function 
that create the files create the file */

int main(int argc, char *argv[])
{

    FILE *f; /*pointer to file*/
    char fname[MAXNAME]; /*name of input file with max char of 1K*/
    char fnameAs[MAXNAME + 3]; /*name of input file with .as*/
    char line[MAXLINE]; /*line from input file with max value of 80 char*/

    int i; /*for loops*/
    int passTwo = 1; /*equal to one if no error found and we go to pass two*/
    int IC; /*Instruction counter*/
    int DC; /*data counter*/
    int cnt; /*input files counter*/
    int tmp; /*return value from pass one befor we know wihch kind of instruction it is*/
    int newFile; /*for signal that we open a new file and we need to make IC and DC 0*/

    /*check if we got right input*/
    if (argc < 1){ 
        printf("Error! no file name in command line."); 
        return 0;  
    }/*if*/

    /*add to the file .as*/
    for (cnt=1; cnt<argc; cn++){
        newFile = 1;  IC = 0; DC = 0;
        fname = argv[cnt];
        strcpy(fnameAs, fname);
        strcat(fnameAs,".as");
        /*open the file, if have a problem with a file raise an erorr*/
        if ((f = fopen(fnameAs,"r")) == NULL){  
            error("Cannot open source file %s\n",fnameAs);  
            continue;  
        }/*if*/

        /*check if we got the line, send to firstPass method
        if we got an error with the first pass we pass the second and going to the next file*/
        while (fgets(line, MAXINPUT, f) != NULL){
            passTwo = firstPass(line, newFile); 
            newFile = 0;
            if (passTwo == -1){
                ok = passTwo; 
                continue; 
            }/*if*/
            /*check if its a command or data*/
            if (commOrDat())
                DC = passTwo;
            else 
                IC = passTwo;  
        }/*while*/

        /*if everything ok go to the second pass*/
        for (i=0; i<IC; i++){ 
            passTwo = secondPass(i,IC);
            if (passTwo == -1) 
                ok = -1; 
        }/*for*/

        /*if no error build the files*/
        if (ok != -1)
            buildFiles(fname, IC, DC);
        /*free the symbol table*/
        freeTab();
        
        /*close the file*/
        fclose(f);
}/*big for*/
    return 0;
}/*main*/
