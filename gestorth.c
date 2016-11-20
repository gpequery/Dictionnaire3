#include <dirent.h>

#define color(param) printf("\033[%sm",param)

int my_strlen(char*);
int my_strcmp(char*, char*);
char my_cTolower(char c);
char* my_strcat(char*, char*);  //Malloc !
int getFiles();                 //Affiche la liste des fichiers
char* getNameFile(int);         //Retourne le nom du fichier numero int
char** getWords(char*);
char* finalyWord(char*);        // Retourne le mot sans caractère speciale  Malloc !!
int getCountWord(char*);

int my_strlen(char* word) {
    int count = 0;

    while ( word[count] != '\0' ) {
        count ++;
    }

    return count;
}

int my_strcmp(char* wordA, char* wordB) {
    int count = 0;
    int i = 0;
    int nbLettreA = my_strlen(wordA);
    int nbLettreB = my_strlen(wordB);
    int diff = nbLettreB - nbLettreA;

    while (wordA[i] != '\0') {
        if (wordA[i] != wordB[i] ) {
            count ++;
        }
        i++;
    }

    if (diff > 0 ) {
        count += diff;
    }

    return count;
}

char my_cTolower(char c) {
    char lower;
    int i;

    if (c >= 65 && c <= 90) {
        lower = c + 32;
    } else {
        lower = c;
    }

    return lower;
}

char* my_strcat(char* wordA, char* wordB) {
    int lenthA = my_strlen(wordA);
    int lenthB = my_strlen(wordB);
    char* result = malloc(sizeof(char) * (lenthA + lenthB + 1));

    int a = 0;
    int b = 0;
    int i;

    for (i = 0; i < lenthA + lenthB; i ++) {
            if (a < lenthA) {
                result[i] = wordA[a];
                a++;
            } else {
                result[i] = wordB[b];
                b++;
            }
    }
    result[i] = '\0';

    return result;
}

int getFiles() {
    DIR* dossier = opendir("LesFichiers");
    struct dirent* fichier = NULL;
    int count = 0;

    char* name;
    printf("Les fichiers disponible: \n");
    color("33");
    if ( dossier != NULL ) {
        while ( fichier = readdir(dossier) ) {
            if ( (my_strcmp(fichier->d_name, ".") != 0) && (my_strcmp(fichier->d_name, "..") != 0) ) {
                name = fichier->d_name;
                name[my_strlen(name)-4] = '\0';
                count++;
                printf("%d. %s\n", count, name);
            }
        }
        closedir(dossier);
    }
    printf("\n");
    color("37");
    return count;
}

char* getNameFile(int numFile)  {
    DIR* directory = opendir("LesFichiers");
    struct dirent* file = NULL;
    int count = 1;

    char* name;

    if ( directory != NULL ) {
        while ( file = readdir(directory) ) {
            if ( (my_strcmp(file->d_name, ".") != 0) && (my_strcmp(file->d_name, "..") != 0) ) {
                if ( count == numFile ) {
                    name = file->d_name;
                    name[my_strlen(name)-4] = '\0';
                    break;
                }
                count ++;
            }
        }
        closedir(directory);
    }
    return name;
}

char** getWords(char* nameFile) {
    char* pathFile = "LesFichiers/";
    pathFile = my_strcat(pathFile, nameFile);
    pathFile = my_strcat(pathFile, ".txt");

    FILE* file = fopen(pathFile, "r");
    int countLine = 1;
    int countWordInLine;
    int i;
    if (file) {
        while(!feof(file)) {
            char* line = malloc(sizeof(char) * 1000);
            fgets(line, 1000, file);
            line = finalyWord(line);

            countWordInLine = getCountWord(line);

            printf("%d : %s ", countWordInLine, line);
            printf("\n");
            countLine ++;
        }

        fclose(file);
    }

    free(pathFile);

    return NULL;
}

char* finalyWord(char* word) {
    char* result = malloc(sizeof(char)*1000);

    int j = 0;
    int i;

    for (i = 0; i < my_strlen(word); i ++ ) {
        if ( (word[i] == 32) || (word[i] == 39 && (j != 0 && j != my_strlen(word)-2)) || (word[i] == 45 && (j != 0 && j != my_strlen(word)-2)) || (word[i] > 64 && word[i] < 91) || (word[i] > 96 && word[i] < 123) || (word[i] > -65 && word[i] < -48) || (word[i] > -48 && word[i] < -41) || (word[i] > -40 && word[i] < -34) || (word[i] > -33 && word[i] < -16) || (word[i] > -16 && word[i] < -9) || (word[i] > -8 && word[i] < -2) ) {
            result[j] = my_cTolower(word[i]);
            j ++;
        }
    }
    result[j] = '\0';

    return result;
}

int getCountWord(char* line) {
    int count = 0;

    int i;
    for (i = 0; i < my_strlen(line); i++) {
        if ( line[i] == ' ' && i != my_strlen(line)-1 ) {
            count ++;
        }
    }

    return count + 1;
}
