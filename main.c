#include <stdio.h>
#include <stdlib.h>

#include "gestorth.c"

#define color(param) printf("\033[%sm",param)

void clearConsole(char*);   //Affichage en tete (char* le titre a afficher)
void accueil();             //Gestion des dicos
void menu(char*);

int main() {
    accueil();
    return 0;
}

void clearConsole(char* dico) {
    system("cls");
    color("35");
    printf("Fichier %s\n", dico);
    printf("--------");

    int size = my_strlen(dico);
    int i;
    for ( i = 0; i < size; i ++ ) {
        printf("-");
    }
    printf("\n\n");
    color("37");
}

void accueil() {
    clearConsole("");

    int countFile = getFiles();
    color("36");
    printf("%d. Quitter\n\n", countFile + 1);

    color("37");
    int action;
    printf("Saisir le numero d'un fichier : ");
    scanf("%d", &action);
    printf("\n");

    if (action > 0 && action <= countFile) {
        char* nameFile = getNameFile(action);
        menu(nameFile);
    } else if (action == countFile + 1) {
        system("exit");
    } else {
        accueil();
    }
}

void menu(char* titre) {
    clearConsole(titre);

    getWords(titre);

}





