#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Only one argument is allowed!\n");
        exit(1);
    }

    if (strstr(argv[1], ".bmp") == NULL) {
        printf("The file given as argument should have .bmp format!\n");
        exit(1);
    }

    

    return 0;
}