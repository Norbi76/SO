#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

# define FIELD_SIZE 100

typedef struct {
    char numeFisier[FIELD_SIZE], drepturiUser[FIELD_SIZE], drepturiGrup[FIELD_SIZE], drepturiAltii[FIELD_SIZE];
    int dimensiune, inaltime, lungime, userId, nrLegaturi;
    time_t timpUltimaModificare;
}myStat;

int main(int argc, char **argv) {
    struct stat fileStat;
    struct dirent *dirent;
    DIR *dir;

    if (argc != 2) {
        printf("Usage ./program <director intrare>\n");
        exit(1);
    }

    dir = opendir(argv[1]);
    if (dir == NULL) {
        printf("Can t open directory!\n");
        exit(1);
    }

    while ((dirent = readdir(dir)) != NULL) {
        // printf("%s\n", dirent->d_name);
        char relativePath[50];
        strcpy(relativePath, argv[1]);
        strcat(relativePath, "/");
        strcat(relativePath, dirent->d_name);
        // printf("%s\n", relativePath);
        
        if ((stat(relativePath, &fileStat) == -1)) {
            printf("Something went wrong!\n");
            exit(1);
        }

        // printf("%d\n", S_ISREG(fileStat.st_mode));
        
        if (S_ISREG(fileStat.st_mode)) {
            if (strstr(relativePath, ".bmp")) {
                printf("%s\n", relativePath);
            }
            else {
                printf("%s\n", relativePath);
            }
        }

    }

    closedir(dir);

    return 0;
}