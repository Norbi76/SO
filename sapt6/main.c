#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>

# define FIELD_SIZE 100
# define BUFFER_SIZE 200 

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

    read(in, &buffer, 4);
    stat->lungime = buffer;

    read(in, &buffer, 4);
    stat->inaltime = buffer;

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

    sprintf(buffer, "Inaltime imagine: %d\n", stat.inaltime);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Lungime imagine: %d\n", stat.lungime);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

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

    sprintf(buffer, "Drepturi de acces altii: %s\n", stat.drepturiAltii);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

}

int main(int argc, char **argv) {
    myStat statistic;

    if (argc != 2 || strstr(argv[1], ".bmp") == NULL) {
        printf("Usage ./program <fisier_intrare>\n");
        exit(1);
    }

    int in_file = open("imagine.bmp", O_RDONLY);

    if (in_file == -1) {
        printf("Couldn t open input file!\n");
        exit(1);
    }

    int out_file = open("statistica.txt", O_WRONLY);

    if (out_file == -1) {
        printf("Couldn t open output file!\n");
        exit(1);
    }

    getData(in_file, &statistic, argv[1]);
    fprintStat(statistic, out_file);

    close(in_file);
    close(out_file);

    return 0;
}