#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <windows.h>
#include <string.h>

typedef struct Joueur {
    char name[15];
    int stat[2];
} Joueur;

#define fileName "profil2.bsav"

int file_exist(const char * filename){
    /* try to open file to read */
    FILE *file;
    if (file = fopen(filename, "r")){
        fclose(file);
        return 1;
    }
    return 0;
}

void createBinFile() {
    if (file_exist(fileName)) {
        FILE *file = fopen(fileName, "rb");
        if (file != NULL) {
            fclose(file);
        }
    } else {
        FILE *file = fopen(fileName, "wb");
        if (file != NULL) {
            fclose(file);
        }
    }
}

bool checkIfFileEmpty() {
    FILE *file2 = fopen(fileName, "rb");
    if (NULL != file2) {
        fseek (file2, 0, SEEK_END);
        int size = ftell(file2);

        if (0 == size) {
            fclose(file2);
            return 1;

        }
        fclose(file2);
        return 0;

    }
}



/*player.name = "test"; //name
player.stat[0] = 3; //played
player.stat[1] = 1; //wins*/

void save(Joueur _joueur) {
    FILE *saveFile = fopen(fileName, "wb");
    if (saveFile != NULL) {
        fwrite (&_joueur, sizeof (Joueur), 1, saveFile);
        printf("\t-> Save successful\n");
        //fwrite (&p_statTab, sizeof (p_statTab), 1, saveFile);
        fclose(saveFile);

    }
}

Joueur restore(Joueur _joueur) {
    FILE *restoreFile = fopen(fileName, "rb");
    if (restoreFile != NULL) {
        fread(&_joueur, sizeof (Joueur), 1, restoreFile);
        printf("\t-> Restore successful\n");
        //fread(&p_statTab, sizeof (p_statTab), 1, restoreFile);
        fclose(restoreFile);
        /*player_name = p_name;
        player_statTab = p_statTab;*/
        printf("=== %s [%i, %i]\n", _joueur.name, _joueur.stat[0], _joueur.stat[1]);
        return _joueur;
    }
}

void askPlayer(Joueur * _joueur) {
    //Demande au joueur de donner son pseudo pour créer une sauvegarde
    printf("Pas de sauvegarde trouvee.\nVeuillez entrer votre pseudo : ");
    scanf("%s", &_joueur->name);
    printf("Vous avez choisi le pseudo %s.\nSauvegarde...\n", _joueur->name);
    /*save(p_name, p_statTab);
    restore(p_name, p_statTab);*/

}
Joueur player = {"", {0,1}};
int main() {

    createBinFile();
    if (checkIfFileEmpty()) {

        askPlayer(&player);
        printf("###\nFichier Vide:\nNom: %s\nParties jouees: %i\nParties gagnees: %i\n###\n", player.name, player.stat[0], player.stat[1]);
        save(player);
        //system("PAUSE");
        //restore(&player);
    } else {
        player = restore(player);
    }

    return 0;
}
