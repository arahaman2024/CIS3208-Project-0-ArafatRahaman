Project Name - tuls

GIT - https://github.com/CIS-3207-S23/project-0-s23-arafatrahaman1

This program is a modified version of the Unix command "ls". When the program is ran it will print the contents of 
the current directory of files and folders. If an arguement is added after the command (ie: ./tuls dir), it will 
tuls will print the contents of that directory, as well as the contents of any subfolders in a recursive way.
It should continue searching recursively until it hits the end of the file tree.

Compiling Command: 
gcc -o tuls tuls.c -Wall -Werror

Running Commands
./tuls [no argument]
./tuls [ arguement ]

If the arguement that is put after the command ./tuls is not a valid path/directory-name it will throw an error, and the
program will not run. If it has too many commands (ie: ./tuls a b c) it will simply print the contents of the current directory.



------------------------------------------

PART B

This program is a modified version of Unix command "cp". When the program is run it takes in 2 or more arguments (after running call ./tucp)
if it is not given 2 args it fails with error warning. It is able to take a path name to a file and copy it to a destination. It is able to also take
multiple source files, and copy them to a directory.

Compiling Command: 
gcc -o tucp tucp.c -Wall -Werror

Running Commands
./tuls [>= 2 arguements]
