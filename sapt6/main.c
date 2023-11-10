#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

# define SIZE 100

typedef struct {
    char numeFisier[SIZE];
    int dimensiune, inaltime, lungime, userId;
}myStat;

int main(int argc, char **argv) {
    struct stat fileStat;


    if (argc != 2 || strstr(argv[1], ".bmp") == NULL) {
        printf("Usage ./program <fisier_intrare>\n");
        exit(1);
    }

    FILE *out;
    out = fopen("statistica.txt", "w");

    if (out == NULL) {
        printf("Couldn t open output file!\n");
        exit(1);
    }

    int in_file = open("imagine.bmp", O_RDONLY);

    if (in_file == -1) {
        printf("Couldn t open input file!\n");
        exit(1);
    }

    //numele, inaltimea, lungimea si dimensiunea din header restu din stat

    int buffer;

    lseek(in_file, 18, SEEK_SET);
    read(in_file, &buffer, 4);
    printf("%d\n", buffer); //dimensiune
    read(in_file, &buffer, 4);
    printf("%d\n", buffer); //lungime
    read(in_file, &buffer, 4);
    printf("%d\n", buffer); //inaltime


    fstat(in_file, &fileStat);
    
    printf("Number of Links: %d\n",fileStat.st_nlink);
    printf("File Permissions: \t");
    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");
    printf("%lld\n", fileStat.st_mtime);

    return 0;
}