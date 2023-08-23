#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <string.h>
#include <dirent.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

void recStep(struct dirent **namelist, int numFiles, int depth, char *relPath);
void justCurrDir(struct dirent **namelist, int numFiles);

int main(int argc,char **argv)  {
    
    struct dirent **namelist;
    // creates dirent stucture, that will hold files that are in the directory that is called to 
    // by the argument after tuls (if there is one)
    
    int n; 
    // will hold number or files

    if (argc == 1 || argc > 2) {        // if there is no arguement (ie: ./tuls) then prints current directory and nothing else, if there are more than just one argument than just prints directory
        // printf("HERE!");
        n = scandir(".", &namelist, NULL, alphasort);       // scandir scans a dir and stores at the location of dirent **namelist, sorted
        printf("Number Files %d\n", n);
        if (n == -1) {
            perror("scandir");          // prints error message and exits with failure if could not scan file
            exit(EXIT_FAILURE);
        }
        justCurrDir(namelist, n);
        
    }else{
        n = scandir(argv[1], &namelist,NULL,alphasort);  // scans the first argument after command call (ie: ./tuls dir1)
        // printf("Number Files %d\n", n);
        if (n == -1) {
            perror("scandir");
            exit(EXIT_FAILURE);
        }
        recStep(namelist, n, 0, argv[1]);   // sends into a recursive helper function
    }
    
    // this double setup was suggested by my TA, and was very helpful
    
    free(namelist); // frees the pointer
    return 0;
}

void recStep(struct dirent **namelist, int numFiles, int depth, char *relPath) {
    // Parameters for this function
    // namelist used as a storage of files
    // number files is used as an index point in the namelist
    // depth is used as a way to know how many tabs to shift over per print
    // the relPath is used as path-name. like if we started at . (current directory), when we go into the dir1 it would be ./dir1

    char space[4096] = "";  
    for (int d = 0; d < depth; d++) {
        strcat(space, "\t");
    }                   // using the depth to determine how many tab spaces to use prior to putting the name, 4096 is a number suggested by a TA
    
    while (numFiles--)      // while there exists files keep going
    {
        if (*namelist[numFiles] -> d_name == '.') {         // away to ignore not only . but also ..
            // printf("GOING IN HERE!");
            continue;
        }

        int dirCheck = namelist[numFiles]->d_type;          // if returns 4 means it is a directory

        // struct stat path_stat;                              // this did not work properly for some reason so I decided to go with d_type
        // stat(namelist[numFiles]->d_name, &path_stat);

        if (dirCheck == 4)  // 4 means directory   // S_ISDIR(path_stat.st_mode - away to check if it was a file but became useless
        {
            printf("%s> [%s]\n", space, namelist[numFiles]->d_name);        // prints directories in its own formating where if dir then > [dir]

            // at this point u need to do what you originally did in the main. you want to create a dirent structure that can store the files/directories of a directory
            struct dirent **newDir;             
            int numSubDir = 0;      // will hold the number of files in the new directory, used in looping through each item in directory

            char newPathName[4096];
            strcpy(newPathName, relPath);
            strcat(newPathName, "/");           // this is entirly to create a newPathName for a direcetory so that it can be passed back into this function
                                                // ./dir1/ adds the "/"" after the already ./dir1

            strcat(newPathName, namelist[numFiles]->d_name);    // adds dir2 to ./dir1/ becoming ./dir1/dir2 which the path name
            numSubDir = scandir(newPathName, &newDir, NULL, alphasort); // uses scandir to do the same thing as it did in main

            if (numSubDir == -1) {          // again in main, checks to see if there is an error
                perror("scandir");
                exit(EXIT_FAILURE);
            }

            recStep(newDir, numSubDir, depth+1, newPathName);       // recursive step
        }else{
            printf("%s> %s\n", space ,namelist[numFiles]->d_name);  // prints without brackets if it is just a regular file
        }

    }
}

void justCurrDir(struct dirent **namelist, int numFiles) {  // this is called if you put no areguemnts after "./tuls". It will simple print the current directory, and distinguishes files with []

    if (numFiles == -1) {
        perror("scandir");
        exit(EXIT_FAILURE);
    }

    while (numFiles--) {
        if (*namelist[numFiles] -> d_name == '.') {
            continue;
        }

        struct stat path_stat;
        stat(namelist[numFiles]->d_name, &path_stat);
        if (S_ISDIR(path_stat.st_mode)) 
        {
            printf("> [%s]\n", namelist[numFiles]->d_name);
        }else{
            printf("> %s\n", namelist[numFiles]->d_name);
        }
    }
} 

// GIT - https://github.com/CIS-3207-S23/project-0-s23-arafatrahaman1