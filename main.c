#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "functions.h"

//ENUMERACIJA
typedef enum {
    DODAJ = 1,
    UCITAJ,
    ISPISI,
    OBRISI,
    ZAVRSI,
    POKRENI,
    AZURIRAJ
} Izbornik;

int main() {
    int izbor;

    Izbornik izbornik;
    
    do {
        printf("1. Dodaj usera\n");
        printf("2. Ucitaj usera\n");
        printf("3. Ispis postojecih usera\n");
        printf("4. Brisanje usera\n");
        printf("5. Izlaz iz programa\n");
        printf("6. Pokreni igru\n");
        printf("7. Azuriraj korisnicko ime\n\n");
        printf("Unesite izbor: ");

        if (scanf("%d", &izbor) != 1) {
            while (getchar() != '\n'); //ocisti unos
            printf("*Nepoznat izbor!*\n\n");
            continue;
        }

        if (izbor < DODAJ || izbor > AZURIRAJ) {
            printf("*Nepoznat izbor!*\n\n");
            continue;
        }

        izbornik = (Izbornik)izbor;

        switch (izbornik) {
        case DODAJ:
            dodajUsera();
            break;
        case UCITAJ:
            ucitajUsera();
            break;
        case ISPISI:
            ispisiPostojeceUsere();
            break;
        case OBRISI:
            obrisiUsera();
            break;
        case ZAVRSI:
            zavrsiProgram();
            break;
        case POKRENI:
            pokreniIgru();
            break;
        case AZURIRAJ:
            azurirajUsera();
            break;
        default:
            printf("*Nepoznat izbor!*\n\n");
        }
    } while (izbor != 5);

    return 0;
}