#include <dirent.h>

#define color(param) printf("\033[%sm",param)

int my_strlen(char*);
int my_strcmp(char*, char*);
char my_cTolower(char c);
char* my_tolower(char* word);
char* my_strcat(char*, char*);                                      //Malloc !
int getFiles();                                                     //Affiche la liste des fichiers
char* getNameFile(int);                                             //Retourne le nom du fichier numero int
char*** getWords(char*);                                            //Affiche les mots du fichier char* absent dans le dico et les mots proches present dans le deco(retourne le tableau)
char** getWordsLineToTab(char*);
char* finalyWord(char*);                                            // Retourne le mot sans caractère speciale  Malloc !!
short wordInDico(char*);                                            // Boolean : moot dans dico ? (char* mot a chercher)
int getDiff(char*, char*);                                          // Retourne le nombre de difference entre le mot char* et char*,
void afficherMotProche(char*, int, int, char*);                            // Affiche les mots proche de char* avec le seuil int, int nb ligne dans le fichier
short isNewLetter(char* a, char letter, int i);                     //Boolean, Si la lettre char est présent dans le mot char* avant la position int
short letterInOtherWord(char lettre, char* word, int lengthWord);
void remakeLigne(char*, char*, char*);


struct CountLettre {
    char lettre;
    int count;
};
typedef struct CountLettre CountLettre;

struct Replace {
    char* oldWorld;
    char* newWorld;
};
typedef struct Replace Replace;


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

char* my_tolower(char* word) {
    char* lower = malloc(sizeof(char)*(my_strlen(word)+1));
    int i;

    for (i=0; i< my_strlen(word); i++) {
        if (word[i] >= 65 && word[i] <= 90) {
            lower[i] = word[i] + 32;
        } else {
            lower[i] = word[i];
        }
    }
    lower[i] = '\0';

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

char*** getWords(char* nameFile) {
    char*** tabF = malloc(sizeof(char[25]) * 500);
    char** tab = malloc(sizeof(char[25]) * 100);
    int countLine = 0;

    char* pathFile = "LesFichiers/";
    char* pathFile2 = my_strcat(pathFile, nameFile);
    char* pathFile3 = my_strcat(pathFile2, ".txt");

    FILE* file = fopen(pathFile3, "r");

    if (file) {
        char* line;
        int countWordInLine;
        while(!feof(file)) {

            fgets(line, 1000, file);
            line = finalyWord(line);

            tab = getWordsLineToTab(line);
            tabF[countLine] = tab;
            countLine ++;
        }
        fclose(file);
    }

    free(pathFile);
    free(pathFile3);

    printf("Mots absent dans le dictionnaire : \n");
    char** replaceWords = malloc(sizeof(char[50]) * countLine);
    Replace* tabReplace = malloc(sizeof(Replace) * countLine);
    int countReplace = 0 ;
    int i;
    int j;
    for (i = 0; i < countLine ; i++) {
        j = 0;
        while (tabF[i][j]) {
            color("36");
            printf("Ligne %d : ", i+1);
            color("33");
            printf("%s \t", tabF[i][j]);
            color("37");

            //Affichage des mots proche de seuil 2 du mot absent
            //Ajout du premier mot avec un seuil de 1 dans le tableau replaceWords avec le mot qu'il remplaceras.
            printf("( ");
            FILE* dico = fopen("LesDictionnaires\\Dictionnaire.txt", "r");
            if (dico) {
                short can = 1;

                while (!feof(dico)) {
                    char* curentWord = malloc(sizeof(char) * 25);
                    fscanf(dico, "%s", curentWord);

                    if ( getDiff(tabF[i][j], curentWord) <= 2 ) {
                        if( getDiff(tabF[i][j], curentWord) == 1 ) {

                            if (can) {
                                color("32");
                                printf("%s ", curentWord);
                                color("37");


                                tabReplace[countReplace].oldWorld = tabF[i][j];
                                tabReplace[countReplace].newWorld = curentWord;

                                countReplace ++;
                                can = 0;
                            } else {
                                printf("%s ", curentWord);
                            }
                        } else {
                            printf("%s ", curentWord);
                        }
                    }
                }
                fclose(dico);
            }
            printf(")\n");
            j ++;
        }
    }
    color("37");

    //Remplacer les mot dans un nouveau fichier
    char oldWord[25];
    char newWord[25];

    char* pathFileOLD = my_strcat(pathFile2, ".txt");
    char* pathFileNEW = my_strcat(pathFile2, "_correction.txt");

    FILE* oldFile = fopen(pathFileOLD, "r");
    FILE* newFile = fopen(pathFileNEW, "w+");

    if (oldFile && newFile) {
        char* curentWord2;
        char oldLine[1000];
        short replace;
        while(!feof(oldFile)) {
            //fscanf(oldFile, "%s ", curentWord2);
            //fprintf(newFile, "%s ", curentWord2);

            fgets(oldLine, 1000, oldFile);  //Fgets pour avoir les mêmes retour à la ligne que le fichier d'origine

            curentWord2 = strtok(oldLine, " ");

            if (curentWord2 != NULL) {
                if (curentWord2 != NULL) {
                    //printf("%s ", curentWord2);

                    replace = 0;
                    for (i = 0; i < countReplace; i++) {
                        if ( my_strcmp(tabReplace[i].oldWorld, curentWord2) == 0 ) {
                            replace = 1;
                            break;
                        }
                    }

                    if (replace == 1) {
                        fprintf(newFile, " %s", tabReplace[i].newWorld);
                    } else {
                        fprintf(newFile, " %s", curentWord2);
                    }
                }

                while (curentWord2 != NULL) {
                    curentWord2 = strtok(NULL, " ");
                    if (curentWord2 != NULL) {
                        //printf("%s ", curentWord2);

                        replace = 0;
                        for (i = 0; i < countReplace; i++) {
                            if ( my_strcmp(tabReplace[i].oldWorld, curentWord2) == 0 ) {
                                replace = 1;
                                break;
                            }
                        }

                        if (replace == 1) {
                            fprintf(newFile, " %s", tabReplace[i].newWorld);
                        } else {
                            fprintf(newFile, " %s", curentWord2);
                        }
                    }
                }
            }
        }
        fclose(oldFile);
        fclose(newFile);
    }
    free(pathFile2);

    return tabF;
}

char** getWordsLineToTab(char* ligne) {
    char** tab = malloc(sizeof(char[25]) * 100);
    int countWord = 0;

    char* word;
    char* buffer = strdup (ligne);


    word = strtok(buffer, " ");

    if (!wordInDico(my_tolower(word))) {
        tab[countWord] = word;
        countWord ++ ;
    }


    while (word != NULL) {
        word = strtok(NULL, " ");
        if (word != NULL && !wordInDico(my_tolower(word))) {
            tab[countWord] = word;
            countWord ++ ;
        }
    }

    tab[countWord + 1] = '\0';
    return tab;
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

short wordInDico(char* word) {
    char* directoryDico = "LesDictionnaires\\Dictionnaire.txt";
    int in = 0;

    if (my_strcmp(word, "") == 0) {
        return 1;
    } else {
        FILE* dico = fopen(directoryDico, "r");
        if (dico) {
            char compare[50];
            while(!feof(dico)) {
                fscanf(dico, "%s", compare);
                if ( my_strcmp(word, compare) == 0) {
                    return 1;
                }
            }
            fclose(dico);
        }
    }
    return in;
}

int getDiff(char* a, char* b) {
    int result = 0;
    int i;
    int j;
    int lengthA = my_strlen(a);
    int lengthB = my_strlen(b);
    int count;
    int nbLettreA = 0;
    int nbLettreB = 0;

    CountLettre lettresA[lengthA];
    CountLettre lettresB[lengthB];
    //Count de chaque lettre de a dans la structure
    for (i = 0; i < lengthA; i ++) {
        count = 0;
        if ( isNewLetter(a, a[i], i) ) {
            for (j = 0; j < lengthA; j ++) {
                if ( a[i] == a[j] ) {
                    count ++;
                }
            }
            lettresA[nbLettreA].count = count;
            lettresA[nbLettreA].lettre = a[i];
            nbLettreA ++;
        }
    }

    //Count de chaque lettre de b dans la structure
    for (i = 0; i < lengthB; i ++) {
        count = 0;
        if ( isNewLetter(b, b[i], i) ) {
            for (j = 0; j < lengthB; j ++) {
                if ( b[i] == b[j] ) {
                    count ++;
                }
            }
            lettresB[nbLettreB].count = count;
            lettresB[nbLettreB].lettre = b[i];
            nbLettreB ++;
        }
    }



    if (nbLettreA >= nbLettreB) {
        for (i = 0; i < nbLettreA; i ++) {
            if (!letterInOtherWord(lettresA[i].lettre, b, nbLettreA)) {
                result += lettresA[i].count;
            } else {
                for (j = 0; j < nbLettreB; j ++) {
                    if (lettresA[i].lettre == lettresB[j].lettre) {
                        if (lettresA[i].count >= lettresB[j].count) {
                            result += lettresA[i].count - lettresB[j].count;
                        } else {
                            result += lettresB[j].count - lettresA[i].count;
                        }
                    }
                }
            }
        }
    } else {
        for (i = 0; i < nbLettreB; i ++) {
            if (!letterInOtherWord(lettresB[i].lettre, a, nbLettreB)) {
                result += lettresB[i].count;
            } else {
                for (j = 0; j < nbLettreA; j ++) {
                    if (lettresB[i].lettre == lettresA[j].lettre) {
                        if (lettresB[i].count >= lettresA[j].count) {
                            result += lettresB[i].count - lettresA[j].count;
                        } else {
                            result += lettresA[j].count - lettresB[i].count;
                        }
                    }
                }
            }
        }
    }

    return result;
}

short isNewLetter(char* a, char letter, int i) {
    short result;
    int j;

    result = 1;
    for (j = 0; j < i; j ++) {
        if (a[j] == letter) {
            result = 0;
            break;
        }
    }

    return result;
}

short letterInOtherWord(char lettre, char* word, int lengthWord) {
    short result = 0;
    int i;

    for ( i = 0 ; i < lengthWord; i ++) {
        if ( word[i] == lettre ) {
            result = 1;
        }
    }

    return result;
}

