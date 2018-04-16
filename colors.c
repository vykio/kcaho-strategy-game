//Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <windows.h>   

//Pour gerer les couleurs de la console
HANDLE  hConsole;

int color(int colorAttr){
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorAttr);
}

int resetColor(){
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
}

int main() {
    int i=0;
    for (i=0; i<256; i++){
        printf("%i\t", i);
        color(i);
        printf("Colourful text !\n");
        resetColor();
    }
    system("PAUSE");
}