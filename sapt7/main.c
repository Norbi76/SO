#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<time.h>

# define BUFFER_SIZE 200 
# define FIELD_SIZE 100

typedef struct {
    char numeFisier[FIELD_SIZE], drepturiUser[FIELD_SIZE], drepturiGrup[FIELD_SIZE], drepturiAltii[FIELD_SIZE];
    int dimensiune, inaltime, lungime, userId, nrLegaturi;
    time_t timpUltimaModificare;
}myStat;

void getData(int in, myStat *stat, char *numeFisier) {
    struct stat fileStat;
    int buffer;

    strcpy(stat->numeFisier, numeFisier);

    lseek(in, 18, SEEK_SET);
    read(in, &buffer, 4);
    stat->dimensiune = buffer;

    if (strstr(numeFisier, ".bmp")) {
        read(in, &buffer, 4);
        stat->lungime = buffer;

        read(in, &buffer, 4);
        stat->inaltime = buffer;
    }
    else {
        stat->lungime = -1;
        stat->inaltime = -1;
    }

    fstat(in, &fileStat);
    stat->nrLegaturi = fileStat.st_nlink;
    stat->userId = fileStat.st_uid;
    stat->timpUltimaModificare = fileStat.st_mtime;

    (S_ISDIR(fileStat.st_mode)) ? strcpy(stat->drepturiUser, "d") : strcpy(stat->drepturiUser, "-");
    (fileStat.st_mode & S_IRUSR) ? strcat(stat->drepturiUser, "r") : strcat(stat->drepturiUser, "-");
    (fileStat.st_mode & S_IWUSR) ? strcat(stat->drepturiUser, "w") : strcat(stat->drepturiUser, "-");
    (fileStat.st_mode & S_IXUSR) ? strcat(stat->drepturiUser, "x") : strcat(stat->drepturiUser, "-");

    (fileStat.st_mode & S_IRGRP) ? strcpy(stat->drepturiGrup, "r") : strcpy(stat->drepturiGrup, "-");
    (fileStat.st_mode & S_IRGRP) ? strcat(stat->drepturiGrup, "w") : strcat(stat->drepturiGrup, "-");
    (fileStat.st_mode & S_IXGRP) ? strcat(stat->drepturiGrup, "x") : strcat(stat->drepturiGrup, "-");

    (fileStat.st_mode & S_IROTH) ? strcpy(stat->drepturiAltii, "r") : strcpy(stat->drepturiAltii, "-");
    (fileStat.st_mode & S_IWOTH) ? strcat(stat->drepturiAltii, "w") : strcat(stat->drepturiAltii, "-");
    (fileStat.st_mode & S_IXOTH) ? strcat(stat->drepturiAltii, "x") : strcat(stat->drepturiAltii, "-");

}

void fprintStat(myStat stat, int out) {
    char buffer[BUFFER_SIZE];

    sprintf(buffer, "Nume fisier: %s\n", stat.numeFisier);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Dimensiune fisier: %d\n", stat.dimensiune);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    if (stat.inaltime != -1 || stat.lungime != -1) {
        sprintf(buffer, "Inaltime imagine: %d\n", stat.inaltime);
        write(out, buffer, strlen(buffer));
        strcpy(buffer, "");

        sprintf(buffer, "Lungime imagine: %d\n", stat.lungime);
        write(out, buffer, strlen(buffer));
        strcpy(buffer, "");
    }

    sprintf(buffer, "Identificatorul utilizatorului: %d\n", stat.userId);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Contor de legaturi: %d\n", stat.nrLegaturi);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Timpul ultimei modificari: %s", ctime(&stat.timpUltimaModificare));
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Drepturi de acces user: %s\n", stat.drepturiUser);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Drepturi de acces grup: %s\n", stat.drepturiGrup);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Drepturi de acces altii: %s\n\n", stat.drepturiAltii);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    
}

int main(int argc, char **argv) {
    struct stat fileStat;
    struct dirent *dirent;
    myStat statistic;
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
        char relativePath[50];
        strcpy(relativePath, argv[1]);
        strcat(relativePath, "/");
        strcat(relativePath, dirent->d_name);
        
        if ((stat(relativePath, &fileStat) == -1)) {
            printf("Something went wrong!\n");
            exit(1);
        }
        
        if (S_ISREG(fileStat.st_mode)) {
            int in = open(relativePath, O_RDONLY);
            int out = open("statistica.txt", O_WRONLY | O_APPEND);

            if (in == -1 || out == -1) {
                printf("Something went wrong!\n");
                exit(1);
            }

            getData(in, &statistic, dirent->d_name);
            fprintStat(statistic, out);

            close(in);
            close(out);
        }

    }

    closedir(dir);

    return 0;
}