#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
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
    // read(in, &buffer, 4);


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
    stat->dimensiune = fileStat.st_size;

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

void fprintStat(myStat *stat, int out) {
    char buffer[BUFFER_SIZE];

    sprintf(buffer, "Nume fisier: %s\n", stat->numeFisier);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Dimensiune fisier: %d\n", stat->dimensiune);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    if (stat->inaltime != -1 || stat->lungime != -1) {
        sprintf(buffer, "Inaltime imagine: %d\n", stat->inaltime);
        write(out, buffer, strlen(buffer));
        strcpy(buffer, "");

        sprintf(buffer, "Lungime imagine: %d\n", stat->lungime);
        write(out, buffer, strlen(buffer));
        strcpy(buffer, "");
    }

    sprintf(buffer, "Identificatorul utilizatorului: %d\n", stat->userId);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Contor de legaturi: %d\n", stat->nrLegaturi);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Timpul ultimei modificari: %s", ctime(&stat->timpUltimaModificare));
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Drepturi de acces user: %s\n", stat->drepturiUser);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Drepturi de acces grup: %s\n", stat->drepturiGrup);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Drepturi de acces altii: %s\n\n", stat->drepturiAltii);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");
}

void fprintDirStat(int out, struct stat *fileStat, char *dirName) {
    char buffer[BUFFER_SIZE];

    sprintf(buffer, "Nume director: %s\n", dirName);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Indentificatorul utilizatorului: %d\n", fileStat->st_uid);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Drepturi de acces user: %s%s%s\n", (fileStat->st_mode & S_IRUSR) ? "r" : "-", (fileStat->st_mode & S_IWUSR) ? "w" : "-", (fileStat->st_mode & S_IXUSR) ? "x" : "-");
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Drepturi de acces grup: %s%s%s\n", (fileStat->st_mode & S_IRGRP) ? "r" : "-", (fileStat->st_mode & S_IWGRP) ? "w" : "-", (fileStat->st_mode & S_IXGRP) ? "x" : "-");
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Drepturi de acces altii: %s%s%s\n\n", (fileStat->st_mode & S_IROTH) ? "r" : "-", (fileStat->st_mode & S_IWOTH) ? "w" : "-", (fileStat->st_mode & S_IXOTH) ? "x" : "-");
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

}

void fprintSymLnkStat(int out, struct stat *fileStat, char *dirName, myStat stat, long targetFileDim) {
    char buffer[BUFFER_SIZE];

    sprintf(buffer, "Nume link simbolic: %s\n", dirName);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Dimensiune fisier: %ld\n", fileStat->st_size);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Dimensiune fisier target: %ld\n", targetFileDim);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Drepturi de acces user: %s%s%s\n", (fileStat->st_mode & S_IRUSR) ? "r" : "-", (fileStat->st_mode & S_IWUSR) ? "w" : "-", (fileStat->st_mode & S_IXUSR) ? "x" : "-");
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Drepturi de acces grup: %s%s%s\n", (fileStat->st_mode & S_IRGRP) ? "r" : "-", (fileStat->st_mode & S_IWGRP) ? "w" : "-", (fileStat->st_mode & S_IXGRP) ? "x" : "-");
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Drepturi de acces altii: %s%s%s\n\n", (fileStat->st_mode & S_IROTH) ? "r" : "-", (fileStat->st_mode & S_IWOTH) ? "w" : "-", (fileStat->st_mode & S_IXOTH) ? "x" : "-");
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");
}

void createDirEntityFilePath(char *entName, char *outputRelativePath) {
    char fileName[BUFFER_SIZE];

    if (strcmp(entName, ".") == 0) {
            strcpy(fileName, "current_dir_statistic.txt");
        }
        else if (strcmp(entName, "..") == 0) {
            strcpy(fileName, "previous_dir_statistic.txt");
        }
        else {
            char fileNameCopy[BUFFER_SIZE];
            strcpy(fileNameCopy, entName);

            char *token = strtok(fileNameCopy, ".");
            strcpy(fileName, token);
            strcat(fileName, "_statistic.txt");
        }

        strcpy(outputRelativePath, "output_dir/");
        strcat(outputRelativePath, fileName);
}

void recordWrittenLines(int out, int writtenLines, char *fileName) {
    char buffer[BUFFER_SIZE];
    sprintf(buffer, "S au scris %d linii in %s.\n", writtenLines, fileName);
    write(out, buffer, strlen(buffer));
}

int countLinesInFile(char *filePath) {
    FILE *in;
    if((in = fopen(filePath, "r")) == NULL) {
        printf("Counting lines in %s failed!\n", filePath);
        exit(1);
    }
    int lines = 0;

    char buffer[BUFFER_SIZE];
    while(fgets(buffer, BUFFER_SIZE, in) != NULL) {
        lines++;
    }
    
    return lines - 1;
}

int getRasterDataStartAddress(int bmpIn) {
    int rasterDataOff;

    lseek(bmpIn, 10, SEEK_SET);
    read(bmpIn, &rasterDataOff, 4);

    // printf("%d\n", rasterDataOff);

    return rasterDataOff;
}

void toBlackAndWhite(int bmpIn, int width, int heigth) {
    unsigned char bluePixel, greenPixel, redPixel;
    
    int rasterStart = getRasterDataStartAddress(bmpIn);
    lseek(bmpIn, rasterStart, SEEK_SET);

    for (int i = 0; i < (width * heigth); i++){
        read(bmpIn, &bluePixel, 1) ? : printf("error\n");
        read(bmpIn, &greenPixel, 1) ? : printf("error\n");
        read(bmpIn, &redPixel, 1) ? : printf("error\n");

        unsigned char greyValue = 0.114 * bluePixel + 0.587 * greenPixel + 0.299 * redPixel;

        lseek(bmpIn, -3, SEEK_CUR);

        write(bmpIn, &greyValue, 1) ? : printf("error\n");
        write(bmpIn, &greyValue, 1) ? : printf("error\n");
        write(bmpIn, &greyValue, 1) ? : printf("error\n");
    }
}

void parseDir(DIR *dir, struct dirent *dirent, char *parsedFolder, char *outputFolder, struct stat *fileStat, myStat *statistic) {
    char statisticFilePath[BUFFER_SIZE];
    strcpy(statisticFilePath, outputFolder);
    strcat(statisticFilePath, "/");
    strcat(statisticFilePath, "statistic.txt");

    int statisticFile = open(statisticFilePath, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR);
    if (statisticFile == -1) {
        printf("Couldn't open output file!\n");
        exit(1);
    }
    
    while ((dirent = readdir(dir)) != NULL) {
        char relativePath[50];
        strcpy(relativePath, parsedFolder);
        strcat(relativePath, "/");
        strcat(relativePath, dirent->d_name);

        if ((lstat(relativePath, fileStat) == -1)) {
            printf("Could not get stat for %s!\n", relativePath);
            continue;
        }
        
        char outputRelPath[BUFFER_SIZE];
        createDirEntityFilePath(dirent->d_name, outputRelPath);

        int out = open(outputRelPath, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR);
        if (out == -1) {
            printf("Couldn t open output file\n");
            exit(1);
        }

        int pfd[2];
        if (pipe(pfd) < 0) {
            printf("Error creating pipe!\n");
            exit(1);
        }

        int writtenLines, buffer;
        pid_t pid;
        if (S_ISREG(fileStat->st_mode)) {
            int in = open(relativePath, O_RDWR);
            getData(in, statistic, dirent->d_name);

            if ((pid = fork()) < 0) {
                printf("error\n");
                exit(1);
            }
            else if (pid == 0) {
                pid_t toBlackAndWhitePid;
                fprintStat(statistic, out);
                close(out);

                buffer = countLinesInFile(outputRelPath);
                close(pfd[0]);
                write(pfd[1], &buffer, sizeof(int));
                close(pfd[1]);

                if (strstr(relativePath, ".bmp")) {
                    if ((toBlackAndWhitePid = fork()) < 0) {
                        printf("Can't create process!\n");
                        exit(1);
                    }
                    else if (toBlackAndWhitePid == 0) {
                        toBlackAndWhite(in, statistic->lungime, statistic->inaltime);

                        exit(0);
                    }
                }

                
                exit(0);
            }
            
            close(pfd[1]);
            read(pfd[0], &writtenLines, sizeof(int));
            close(pfd[0]);

            recordWrittenLines(statisticFile, writtenLines, outputRelPath);

            close(in);
        }
        else if (S_ISDIR(fileStat->st_mode)) {
            if ((pid = fork()) < 0) {
                printf("error\n");
                exit(1);
            }
            else if (pid == 0) {
                fprintDirStat(out, fileStat, dirent->d_name);
                close(out);
                
                buffer = countLinesInFile(outputRelPath);
                close(pfd[0]);
                write(pfd[1], &buffer, sizeof(int));
                close(pfd[1]);

                exit(0);
            }

            close(pfd[1]);
            read(pfd[0], &writtenLines, sizeof(int));
            close(pfd[0]);

            recordWrittenLines(statisticFile, writtenLines, outputRelPath);

        }
        else if (S_ISLNK(fileStat->st_mode)) {
            struct stat statForTargetFile;

            if ((stat(relativePath, &statForTargetFile) == -1)) {
                printf("Could not get stat for %s!\n", relativePath);
                continue;
            }

            if ((pid = fork()) < 0) {
                printf("error\n");
                exit(1);
            }
            else if (pid == 0) {
                fprintSymLnkStat(out, fileStat, dirent->d_name, *statistic, statForTargetFile.st_size);
                close(out);

                buffer = countLinesInFile(outputRelPath);
                close(pfd[0]);
                write(pfd[1], &buffer, sizeof(int));
                close(pfd[1]);

                exit(0);
            }

            close(pfd[1]);
            read(pfd[0], &writtenLines, sizeof(int));
            close(pfd[0]);

            recordWrittenLines(statisticFile, writtenLines, outputRelPath);
        }
    }
    close(statisticFile);
}

int main(int argc, char **argv) {
    // if (argc != 3) {
    //     printf("Usage ./a.out <director_intrare> <director_iesire>\n");
    //     exit(1);
    // }

    struct dirent *dirent = NULL;
    DIR *dir = opendir("input_dir");

    if (dir == NULL) {
        printf("Can t open directory!\n");
        exit(1);
    }

    struct stat fileStat;
    myStat statistic;

    parseDir(dir, dirent, "input_dir", "output_dir", &fileStat, &statistic);

    closedir(dir);

    return 0;
}