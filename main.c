/*
-> TO-DO LIST:

    - (V) Verification du bon placement des pieces au début
    - Sauvegarde locale (nombre de parties; Niveau; Description; Pseudo; Couleur favorite; )
    - Coder un bot
    - (V) Mise en surbrillance de certaines cases (pour tutoriel ou coups possibles)
    - (V) Coder les coups possibles pour chaque type de pieces
    - Apres la partie, le joueur gagne un certain nombre d'exp -> affichage du niveau, etc...
    - (V) Menu avec fleches directionnelles (mise en surbrillance)
    - Menu d'options


-> Stats:

    Accomplished : 4/8 ~ 50%

    Lines : ~800 lines


-> Project:

    Project Name : KCAHO - A Console Strategy Game (in C)

    App Version  : 0.1.0

    Project Idea : A simple strategy game made in pure C where you have to eliminate the other's 'King'

    Developper   : Alexandre V. (Vykio)

    Website      : www.bytsstudio.esy.es (not released yet)

    Other Games  : SlimeRaider (2D javascript platformer) - & - Legacy (text-based RPG in python 2.7)


-> Sources:

    > stackoverflow

    > github

    > codepen

-> Notes

    Pour le bot :   Mode apprentissage (simple deep learning ?)
                    Mode aleatoire (Bouge les pieces aleatoirement)
    Sauvegarde : locale + essayer de faire un ranking ONLINE
    Menu d'options (ou on peut changer les couleurs des pieces et de la surbrillance des coups du joueur 1 & 2)


*/

//Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <windows.h>
#include <string.h>

//Pour gerer les couleurs de la console
HANDLE  hConsole;

//Couleurs Attr
#define blue 1
#define green 2
#define cyan 3
#define red 4
#define pink 5
#define yellow 6
#define white 7
#define darkgrey 8

//Couleurs pour surbrillance
#define OVER_red 192
#define OVER_green 32
#define OVER_cyan 176
#define OVER_darkgrey 128

//Pieces
#define roi 1
#define tourelle 2
#define attaquant 3
#define defenseur 4
#define pion 5

//Grille
#define lin 9
#define col 16

//Couleur du joueur 1 et joueur 2
#define j_1 2
#define j_2 4

//Nombre de piece par jeu
#define nombreRoi 1
#define nombreTourelle 2
#define nombreAttaquant 3
#define nombreDefenseur 1
#define nombrePion 3
#define nombrePieces 10 //1+2+3+1+3

//nombre de joueur par partie, ne changera certainement jamais
#define nombreJoueurPartie 2

//Variables globales pour le menu principal
#define MIN_MENU_ITEM 1
#define MAX_MENU_ITEM 4

//Variables Fichiers sauvegarde
#define fileName "profil.bsav"

//int pieceJeu = (nombreRoi + nombreTourelle + nombreAttaquant + nombreDefenseur + nombrePion);
int pieces1[nombrePieces][4];
int pieces2[nombrePieces][4];
/*
Exemple d'utilisation:
piecesi= [
            [id, coordX, coordY, aliveOrDestroyed],
            [...,...,...],
            [...,...,...],
            [...,...,...],
            [...,...,...]
            ...
        ];
 */

int file_exist(const char * filename){
    /* try to open file to read */
    FILE *file;
    if (file = fopen(filename, "r")){
        fclose(file);
        return 1;
    }
    return 0;
}

typedef struct Joueur {
    char name;
    int statTab[3];
} Joueur;

//Variables utilisees pour la sauvegarde
//int statTab[]
//char name

/*

statTab[] = [numberofPlays, numberofWins, numberofNONwins]

*/

//Variables utilisees plus loin dans le programme
int numeroJoueur = 1;
bool toutplace = false;

//Creation de la grille contenant les ID des pieces
int grille[lin][col];
//Creation de la grille contenant les les valeurs correspondantes aux numero du joueur (0: Personne, 1: Joueur 1, 2: Joueur 2)
int grillePlacement[lin][col];

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
            return 1;
        }
        return 0;
    }
}

void askPlayer() {
    printf("EMPTY");
}

int gen(int tab[lin][col]) {
    //numCase = (rand() % ((lin)*(col)-1));
    int i,j;
    int count = 0;
    for (i=0;i<lin;i++){
        for (j=0;j<col;j++){
            tab[i][j] = 0;
            grillePlacement[i][j] = 0;

            count++;
        }
    }
}

bool alreadyPlaced(int x, int y) {
    int i,j;
    if (grillePlacement[y][x] != 0) return true;
    return false;
}

int whom(int x, int y){
    int i,j,k,l;
    for (i=0;i<nombrePieces;i++){
        if (pieces1[i][1] == x && pieces1[i][2] == y && pieces1[i][3] == true){
            return 0; //Piece du joueur 1
        } else if (pieces2[i][1] == x && pieces2[i][2] == y && pieces2[i][3] == true){
            return 1; //Piece du joueur 2
        }

    }
}

int affiche(int tab[lin][col]){
    int i,j;
    printf("\t");
    for (j=97;j<97+col/2;j++){
        printf("  %c ",j);
    }
    printf("  ");
    for (j=97+8;j<97+8+col/2;j++){
        printf("  %c ",j);
    }
    puts("\n\n\t+---+---+---+---+---+---+---+---+ +---+---+---+---+---+---+---+---+");

    int u;
    for (u=0;u<nombrePieces;u++){
        if (pieces1[u][3] == true){
            tab[pieces1[u][2]][pieces1[u][1]] = pieces1[u][0];
        }
        if (pieces2[u][3] == true){
            tab[pieces2[u][2]][pieces2[u][1]] = pieces2[u][0];
        }
    }

    for (i=0;i<lin;i++){
        printf("%i\t",i);
        for (j=0;j<col;j++){
            if (j == col/2){
                printf("| | ");
            } else {
                printf("| ");
            }
            if (grillePlacement[i][j] == 0) resetColor();
            if (grillePlacement[i][j] == 1) color(j_1);
            if (grillePlacement[i][j] == 2) color(j_2);

            if (tab[i][j] == 0){
                printf(" ");
            } else if (tab[i][j] == pion){
                printf("O");
            } else if(tab[i][j] == defenseur){
                printf("H");
            } else if(tab[i][j] == attaquant){
                printf("A");
            } else if(tab[i][j] == tourelle){
                printf("C");
            } else if(tab[i][j] == roi){
                printf("K");
            }
            resetColor();
            printf(" ");
        }
        printf("|\t%i",i);
        puts("\n\t+---+---+---+---+---+---+---+---+ +---+---+---+---+---+---+---+---+");
    }

    printf("\n");
}

//Fonction qui change les attributs de la console pour y afficher de la couleur
//Voir programme 'colors.c' & 'colors.exe' pour voir l'entier correspondant aux couleurs
int color(int colorAttr){
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorAttr);
}

//Reset la couleur de la console (7: Ecriture blanche console sur fond noir)
int resetColor(){
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
}

/* Fonction MAIN */
int main(){
    Joueur player;

    createBinFile();
    if (checkIfFileEmpty()) {
        askPlayer();
    }
    menu(); //On passe directement au menu
}

int place(int numJoueur, int idPiece, int coordX, int coordY){
    return 0;
}

//Retourne la coodonnee numerique en fonction de la lettre de la grille affichee
int convert(char x){
    return (int)x-97; //97 : a; 97 : b;... en ascii
}

void placementBot() {  //Placement des pieces par le bot
    return 0;
}

//Fonction qui affiche le nom de la piece en fonction de l'identifiant
void printNameById(int id){
    if (id == roi) printf("Roi");
    if (id == tourelle) printf("Tourelle");
    if (id == attaquant) printf("Attaquant");
    if (id == defenseur) printf("Defenseur");
    if (id == pion) printf("Pion");
}

void deplacement(int oldx, int oldy, int newx, int newy) {
    int u;
    for (u=0;u<nombrePieces;u++){
        if (pieces1[u][1] == newx && pieces1[u][2] == newy && numeroJoueur != grillePlacement[newy][newx]) {
            pieces1[u][3] = false;
        }
        if (pieces2[u][1] == newx && pieces2[u][2] == newy && numeroJoueur != grillePlacement[newy][newx]) {
            pieces2[u][3] = false;
        }
    }
    for (u=0;u<nombrePieces;u++){
        if (pieces1[u][1] == oldx && pieces1[u][2] == oldy){
            pieces1[u][1] = newx;
            pieces1[u][2] = newy;
        }
        if (pieces2[u][1] == oldx && pieces2[u][2] == oldy){
            pieces2[u][1] = newx;
            pieces2[u][2] = newy;
        }
    }
    grillePlacement[oldy][oldx] = 0;
    grillePlacement[newy][newx] = numeroJoueur;
    int temp_id;
    temp_id = grille[oldy][oldx];
    grille[oldy][oldx] = 0;
    //sgrille[newy][newx] = temp_id;
}

bool deplacementPossible(int t_x, int t_y, int d_x, int d_y, int ID, int numeroJoueur) {
    //La variable eps est utilise pour le sens dans lequel le joueur peut jouer
    //De la gauche vers la droite pour le joueur 1 et de la droite vers la gauche pour le joueur 2.
    int eps;
    if (numeroJoueur == 1){
        eps = 1;
    } else if (numeroJoueur == 2){
        eps = -1;
    }

    if (numeroJoueur != grillePlacement[d_y][d_x]) { //Les cases possibles ne sont pas celles qui sont utilisees par les pieces du meme joueur
        if (ID == pion) {
            if ((t_x + eps*1 == d_x) && ((t_y + 1 == d_y) || (t_y == d_y) || (t_y - 1 == d_y)) || (t_x - eps*1 == d_x) && ((t_y + 1 == d_y) || (t_y - 1 == d_y))) {
                return true;
            } else {
                return false;
            }
        }

        if (ID == defenseur){
            if ( (((t_x + eps*1 == d_x) || t_x - eps*1 == d_x) && (t_y == d_y)) || ((t_x == d_x) && ((t_y + 1 == d_y) || (t_y + 2 == d_y) || (t_y - 1 == d_y) || (t_y - 2 == d_y)))){
                return true;
            } else {
                return false;
            }
        }

        if (ID == attaquant) {
            if ( (t_x + eps*1 == d_x) && ((t_y + 1 == d_y) || (t_y == d_y) || (t_y - 1 == d_y)) || (t_x - eps*1 == d_x) && ((t_y == d_y)) || ((t_x + eps*2 == d_x) && (t_y == d_y))) {
                return true;
            } else {
                return false;
            }
        }

        if (ID == tourelle) {
            if ( ( (t_x == d_x) && ((t_y - 1 == d_y) || (t_y + 1 == d_y)) ) || (((t_x + eps*1 == d_x) || (t_x - eps*1 == d_x)) && (t_y == d_y)) ){
                return true;
            } else {
                return false;
            }
        }

        if (ID == roi) {
            if ( ( (t_x == d_x) && ((t_y - 1 == d_y) || (t_y + 1 == d_y)) ) || (((t_y + eps*1 == d_y) || (t_y == d_y) || (t_y - eps*1 == d_y)) && ((t_x + eps*1 == d_x) || (t_x - eps*1 == d_x))) ){
                return true;
            } else {
                return false;
            }
        }
    } else {
        return false;
    }
}

int counter2;

/* Fonction afficheGlow utilisee pour afficher les coups possibles en surbrillance */
int afficheGlow(int tab[lin][col], int ID2, int x, int y, int numeroJoueur){
    int i,j;
    printf("\t");
    for (j=97;j<97+col/2;j++){
        printf("  %c ",j);
    }
    printf("  ");
    for (j=97+8;j<97+8+col/2;j++){
        printf("  %c ",j);
    }
    puts("\n\n\t+---+---+---+---+---+---+---+---+ +---+---+---+---+---+---+---+---+");

    int u;
    for (u=0;u<nombrePieces;u++){
        if (pieces1[u][3] == true){
            tab[pieces1[u][2]][pieces1[u][1]] = pieces1[u][0];
        }
        if (pieces2[u][3] == true){
            tab[pieces2[u][2]][pieces2[u][1]] = pieces2[u][0];
        }
    }

    counter2 = 0;

    for (i=0;i<lin;i++){
        printf("%i\t",i);
        for (j=0;j<col;j++){
            if (j == col/2){
                printf("| |");
            } else {
                printf("|");
            }
            if (i == y && j == x) {
                color(OVER_cyan);
                printf(" ");
                if (tab[i][j] == 0){
                    printf(" ");
                } else if (tab[i][j] == pion){
                    printf("O");
                } else if(tab[i][j] == defenseur){
                    printf("H");
                } else if(tab[i][j] == attaquant){
                    printf("A");
                } else if(tab[i][j] == tourelle){
                    printf("C");
                } else if(tab[i][j] == roi){
                    printf("K");
                }
            } else {
                if (deplacementPossible(x,y,j,i,ID2,numeroJoueur) == true) {
                    color(240);
                    counter2++;
                } else {
                    if (grillePlacement[i][j] == 1) color(j_1);
                    if (grillePlacement[i][j] == 2) color(j_2);
                }
                printf(" ");


                if (tab[i][j] == 0){
                    printf(" ");
                } else if (tab[i][j] == pion){
                    printf("O");
                } else if(tab[i][j] == defenseur){
                    printf("H");
                } else if(tab[i][j] == attaquant){
                    printf("A");
                } else if(tab[i][j] == tourelle){
                    printf("C");
                } else if(tab[i][j] == roi){
                    printf("K");
                }
            }

            printf(" ");
            resetColor();

        }
        printf("|\t%i",i);
        puts("\n\t+---+---+---+---+---+---+---+---+ +---+---+---+---+---+---+---+---+");
    }

    printf("\n");
}


char t_x, d_x;
void game(bool pvp){
    int t_y, d_y;

    numeroJoueur = 1;

    int temp_NB = 1;
    if (pvp) temp_NB = 2;
    int i,j,k;

    for (k=0;k < temp_NB;k++){
        while (pieces1[0][3] == true && pieces2[0][3] == true){
            system("cls");
            affiche(grille);

            printf("Au ");
            if (numeroJoueur == 1) color(j_1);
            if (numeroJoueur == 2) color(j_2);
            printf("joueur %i", numeroJoueur);
            resetColor();
            printf(" de jouer !\n");

            /*
            /////
            for (i=0;i<lin;i++){
                for (j=0;j<col;j++){
                    printf(" %i ", grillePlacement[i][j]);
                }
                printf("\n");
            }
            ////
            int xxx;
            for (xxx = 0; xxx < nombrePieces ; xxx++){
                printf("%i - %i - %i\n", pieces1[xxx][3], pieces1[xxx][1], pieces1[xxx][2]);
            }*/
            //////////////////////////////////////////

            printf("Selectionner la piece: ");

            //printf("%i",convert('b'));
            printf("\n\tx > ");
            scanf("%s", &t_x);
            printf("\ty > ");
            scanf("%i", &t_y);

            //Convertir la lettre de Ox, en chiffre Entier
            t_x = convert(t_x);

            while (t_x < 0 || t_x > 15 || t_y < 0 || t_y > 8 || !alreadyPlaced(t_x,t_y) || (grillePlacement[t_y][t_x] != numeroJoueur)) {
                printf("Il n'y a pas de piece a cet emplacement\n-> Selectionner la piece: ");

            // printf("<%i-%i><%i-%i>",t_x,t_y,grillePlacement[t_y][t_x],numeroJoueur);
                //printf("%i",convert('b'));
                printf("\n\tx > ");
                scanf("%s", &t_x);
                t_x = convert(t_x);
                printf("\ty > ");
                scanf("%i", &t_y);
            }

            system("cls");
            afficheGlow(grille, grille[t_y][t_x], t_x, t_y, numeroJoueur);

            while (counter2 == 0) {
                printf("Vous ne pouvez pas deplacer cette piece !\n");

                printf("Selectionner la piece: ");

                //printf("%i",convert('b'));
                printf("\n\tx > ");
                scanf("%s", &t_x);
                printf("\ty > ");
                scanf("%i", &t_y);

                //Convertir la lettre de Ox, en chiffre Entier
                t_x = convert(t_x);

                while (t_x < 0 || t_x > 15 || t_y < 0 || t_y > 8 || !alreadyPlaced(t_x,t_y) || (grillePlacement[t_y][t_x] != numeroJoueur)) {
                    printf("Il n'y a pas de piece a cet emplacement\n-> Selectionner la piece: ");

                // printf("<%i-%i><%i-%i>",t_x,t_y,grillePlacement[t_y][t_x],numeroJoueur);
                    //printf("%i",convert('b'));
                    printf("\n\tx > ");
                    scanf("%s", &t_x);
                    t_x = convert(t_x);
                    printf("\ty > ");
                    scanf("%i", &t_y);
                }

                system("cls");
                afficheGlow(grille, grille[t_y][t_x], t_x, t_y, numeroJoueur);
            }

            printf("Vous avez selectionne ");
            if (numeroJoueur == 1) {
                color(j_1);
                printNameById(grille[t_y][t_x]);
            }
            if (numeroJoueur == 2) {
                color(j_2);
                printNameById(grille[t_y][t_x]);
            }
            resetColor();
            printf("\n");

            //Demande des coordonnees pour placement
            printf("Ou voulez-vous deplacer la piece ");
            if (numeroJoueur == 1) color(j_1);
            if (numeroJoueur == 2) color(j_2);
            printNameById(grille[t_y][t_x]);
            resetColor();
            printf(" ?\n\tx > ");
            scanf("%s", &d_x);
            printf("\ty > ");
            scanf("%i", &d_y);
            d_x = convert(d_x);

            while (!deplacementPossible(t_x, t_y, d_x, d_y, grille[t_y][t_x], numeroJoueur)){
                color(192);
                printf("Deplacement impossible !");
                resetColor();
                printf("\n\tx > ");
                scanf("%s", &d_x);
                d_x = convert(d_x);
                printf("\ty > ");
                scanf("%i", &d_y);
            }

            deplacement(t_x, t_y, d_x, d_y);

            //system("PAUSE");
            if (numeroJoueur == 1) {
                numeroJoueur = 2;
            } else if (numeroJoueur == 2) {
                numeroJoueur = 1;
            }
        }
    }

    if (pieces1[0][3] == false) printf("Le joueur 2 a gagne la partie.\n");
    if (pieces2[0][3] == false) printf("Le joueur 1 a gagne la partie.\n");
    system("PAUSE");
}


char x;

void placement(bool pvp) {
    int y,z,k;

    numeroJoueur = 1;
    toutplace = false;

    system("cls");
    gen(grille);
    int u;
    for (u=0; u<nombrePieces ; u++) {
        pieces1[u][3] = false;
        pieces2[u][3] = false;
    }

    affiche(grille);

    int NB = 1;
    if (pvp) NB = 2;


    if (toutplace == false && numeroJoueur <= 2){

        for (k=0;k<NB;k++){
            int identificateur = 1;
            int iden = 0;
            int counter = 1;
            int temptemp = 0;

            for (z=0; z<nombrePieces; z++){
                printf("Placement des pieces pour le ");
                if (numeroJoueur == 1) color(j_1);
                if (numeroJoueur == 2) color(j_2);
                printf("joueur %i", numeroJoueur);
                resetColor();
                if (identificateur == roi){
                    printf("\nPlacement ROI (reste: %i)", nombreRoi-counter+1);
                    iden = roi;
                    if (counter < nombreRoi) {
                        counter++;
                    } else {
                        counter = 1;
                        identificateur++;
                    }
                    temptemp++;
                } else if (identificateur == tourelle) {
                    printf("\nPlacement TOURELLE (reste: %i)", nombreTourelle-counter+1);
                    iden = tourelle;
                    if (counter < nombreTourelle) {
                        counter++;
                    } else {
                        counter = 1;
                        identificateur++;
                    }
                    temptemp++;
                } else if (identificateur == attaquant) {
                    printf("\nPlacement ATTAQUANT (reste: %i)", nombreAttaquant-counter+1);
                    iden = attaquant;
                    if (counter < nombreAttaquant) {
                        counter++;
                    } else {
                        counter = 1;
                        identificateur++;
                    }
                    temptemp++;
                } else if (identificateur == defenseur) {
                    printf("\nPlacement DEFENSEUR (reste: %i)", nombreDefenseur-counter+1);
                    iden = defenseur;
                    if (counter < nombreDefenseur) {
                        counter++;
                    } else {
                        counter = 1;
                        identificateur++;
                    }
                    temptemp++;
                } else if (identificateur == pion) {
                    printf("\nPlacement PION (reste: %i)", nombrePion-counter+1);
                    iden = pion;
                    if (counter < nombrePion) {
                        counter++;
                    } else {
                        counter = 1;
                        identificateur++;
                    }
                    temptemp++;
                }

                //printf("%i",convert('b'));
                printf("\n\tx > ");
                scanf("%s", &x);
                printf("\ty > ");
                scanf("%i", &y);

                //Convertir la lettre de Ox, en chiffre Entier
                x = convert(x);

                if (numeroJoueur == 1) {
                    while (x > 1 || x < 0 || y > 8 || y < 0 || alreadyPlaced(x,y)){
                        printf("Erreur, veuillez placer les pieces sur la colonne A ou B mais pas sur les pieces existantes");
                        printf("\n\tx > ");
                        scanf("%s", &x);
                        printf("\ty > ");
                        scanf("%i", &y);
                        x = convert(x);
                    }
                } else if (numeroJoueur == 2) {
                    while (x > 15 || x < 14 || y > 8 || y < 0 || alreadyPlaced(x,y)){
                        printf("Erreur, veuillez placer les pieces sur la colonne O ou P mais pas sur les pieces existantes");
                        printf("\n\tx > ");
                        scanf("%s", &x);
                        printf("\ty > ");
                        scanf("%i", &y);
                        x = convert(x);
                    }
                }

                //Si le joueur est le joueur 1
                if (numeroJoueur == 1) {
                    pieces1[temptemp - 1][0] = iden; //Ajoute l'ID
                    pieces1[temptemp - 1][1] = x; //Ajoute la composante X
                    pieces1[temptemp - 1][2] = y; //Ajoute la composante Y
                    pieces1[temptemp - 1][3] = true; //True: piece en vie
                    grillePlacement[y][x] = 1;
                } else if (numeroJoueur == 2) {
                    pieces2[temptemp - 1][0] = iden;
                    pieces2[temptemp - 1][1] = x;
                    pieces2[temptemp - 1][2] = y;
                    pieces2[temptemp - 1][3] = true;
                    grillePlacement[y][x] = 2;
                }

                system("cls");
                affiche(grille);
            }
            numeroJoueur++;
        }

        if (!pvp) placementBot(); //Placement des pièces par le bot

        toutplace = true;

    } else {
        return 1;
    }

    //Suite du jeu, apres le placement
    system("cls");
    printf("Fin du placement des pieces.\n");

    game(pvp);

    menu();
}

int profil() {
    return 0;
}

int launch(int index) {
    if (index == 1) {
        placement(true);
    } else if (index == 2) {
        placement(false);
    } else if (index == 3) {
        profil();
    } else if (index == 4) {
        exit(0);
    }
}

int menu() {
    /* Bloc utile pour le menu utilisant les fleches pour se diriger */
    DWORD mode;
    HANDLE hstdin;
    hstdin = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hstdin, &mode);
    SetConsoleMode(hstdin,0);
    unsigned char a; //Variable de la valeur de la touche sur laquelle nous appuyons.
    int selected = 1;
    //scanf("%i");
    while(1) {

        system("cls");

        //Affichage options avec couleurs
        color(224);
        printf(" Kcaho - Jeu de strategie en C \n\n\n");
        resetColor();
        printf("Options:\n\t1. ");


        if (selected == 1) {
            color(OVER_red);
        } else {
            color(red);
        }
        printf("Partie PVP\n\t");
        resetColor();
        printf("2. ");

        if (selected == 2) {
            color(OVER_green);
        } else {
            color(green);
        }
        printf("Partie PVE\n\t");
        resetColor();
        printf("3. ");

        if (selected == 3) {
            color(OVER_cyan);
        } else {
            color(cyan);
        }
        printf("Profil\n\t");
        resetColor();
        printf("4. ");

        if (selected == 4) {
            color(OVER_darkgrey);
        } else {
            color(darkgrey);
        }
        printf("Quitter\n");
        resetColor();

        //printf(" > ");
        //int numChoix;
        //scanf("%i", &numChoix);

        //launch(numChoix);

        a=getch();
		//for detect the function\arrow keys
		//we must call the getch() again
		//testing if a is '0' or '0xE0'
        //'source:stackoverflow.com'
		if (a==0 || a==0xE0) a=getch();

        /*if (a==27) //ESC for exit the 'while'
			break;*/
        if (a==80 || a==115) //DOWN ARROW
            if (selected == MAX_MENU_ITEM)
                selected = MIN_MENU_ITEM;
            else
                selected++;
        else if (a==72 || a ==122) //UP ARROW
            if (selected == MIN_MENU_ITEM)
                selected = MAX_MENU_ITEM;
            else
                selected--;
        else if (a==13 || a==32) //ENTER KEY
            launch(selected);
            SetConsoleMode(hstdin,mode);

    }
    return 0;

}
