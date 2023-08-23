#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>


int main(int argc, char *argv[])  {
    
    if (argc < 3) {
        fprintf(stderr, "Not enough arguements. Exiting program...");
        return EXIT_FAILURE;
    }

    if (argc == 3) {
        
        struct stat arg1;
        stat(argv[1], &arg1);  
        if (S_ISDIR(arg1.st_mode)) {
            fprintf(stderr, "Program cannot copy a directory. Exiting program...");
            return EXIT_FAILURE;
        }

        if (strcmp(argv[1], argv[2]) == 0){
            fprintf(stderr, "This program cannot copy a source file into the same file in CWD. It also cannot copy a directory. Exiting program...");
            return EXIT_FAILURE;
        }
        
    }

    // if you have more than three the last has to be a directory, and only that can be a directory
    if (argc > 3) {
        printf("%s", argv[argc-1]);
        // checks if any but the last is a dirictory
        for (int i = 1; i < argc-1; i++) {
            struct stat arg;
            stat(argv[i], &arg);
            if (S_ISDIR(arg.st_mode)) {
                fprintf(stderr, "You have entered more than 2 arguments, and one of the source files is a directory. Exiting program...");
                return EXIT_FAILURE; 
            }
        }

        struct stat arg;
        stat(argv[argc-1], &arg);
        if (! S_ISDIR(arg.st_mode)) {
            fprintf(stderr, "You have entered more than 2 arguments, and your last argument is not a directory. Exiting program...");
            return EXIT_FAILURE;
        }
    }





    int nSourceF = argc-2; // number of source files, 3 args means 1 source file

    while(nSourceF) {
        int currentSourceFile = argc-nSourceF-1;    // argc-numSourceFile-1 = current current source file
        
        int sourceFilePerm = open(argv[currentSourceFile], O_RDONLY, S_IRWXU);     
        if (sourceFilePerm == -1) {
            fprintf(stderr, "Cannot open <%s>. Exiting Program...", argv[currentSourceFile]);
            return EXIT_FAILURE;
        }


        char name[4096] = "";
        // checking if the destication is directory
        struct stat destArg;
        
        stat(argv[argc-1], &destArg);
        if (S_ISDIR(destArg.st_mode)) {  // && *argv[argc-1] != '.'
            // printf("ENTERING!");
            strcat(name, argv[argc-1]);
            strcat(name, "/");
            // strcat(name, argv[currentSourceFile]);
            char *lastSlash = strrchr(argv[currentSourceFile], '/');
                if (lastSlash) {
                    strcat(name, lastSlash+1);
                    printf(">%s<", name);
                }else{
                    strcat(name, argv[currentSourceFile]);
                }
        }else {
            strcat(name, argv[argc-1]);
        }
        
        // printf("\n\nname = %s\n\n", name);

        int destFilePerm = open(name, O_WRONLY | O_CREAT | O_TRUNC,  S_IRWXU);
        if (destFilePerm == -1) {
            fprintf(stderr, "Was not able to open <%s>. Exiting Program...", argv[argc-1]);
            return EXIT_FAILURE;
        }

        // read
        int numBytesRead;
        int size = 4096;
        char buffer[size];
        while ((numBytesRead = read(sourceFilePerm, buffer, size)) > 0) {
            if (numBytesRead == -1) {
                fprintf(stderr, "File was not able to be read. Exiting program....");
                return EXIT_FAILURE;

            }
            int checkWrite = write(destFilePerm, buffer, numBytesRead);
            if (checkWrite == -1) {
                fprintf(stderr, "File was not able to be written to. Exiting program...");
                return EXIT_FAILURE;

            }
        }
        

        int a = close(sourceFilePerm);
        if (a == -1) {
            fprintf(stderr, "Was not able to close <%s>. Exiting program...", argv[currentSourceFile]);
            return EXIT_FAILURE;
        }
        int b = close(destFilePerm);
        if (b == -1) {
            fprintf(stderr, "Was not able to close <%s>. Exiting program...", argv[argc-1]);
            return EXIT_FAILURE;
        }
        
        nSourceF--;
    }

    return 0;
}