#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <conio.h> // za _kbhit() i _getch()
#include "functions.h"

User currentUser;

// BSEARCH I QSORT

// Funkcija za usporedbu rezultata korisnika za qsort
int compareScores(const void* a, const void* b) {
    User* userA = (User*)a;
    User* userB = (User*)b;
    return userB->score - userA->score; // Sortiranje od najveceg prema najmanjem
}

// Funkcija za pronalazenje korisnika s najvecim rezultatom pomocu bsearch
User* pronadiNajveciRezultat(User* users, int userCount) {
    if (userCount == 0) {
        return NULL;
    }

    // Sortiramo korisnike po rezultatu, najveci rezultat na početku
    qsort(users, userCount, sizeof(User), compareScores);

    // Koristimo bsearch da pronademo korisnika sa najvecim rezultatom
    User key = { .score = users[0].score };
    User* foundUser = (User*)bsearch(&key, users, userCount, sizeof(User), compareScores); //binarno pretrazivanje

    return foundUser;
}

// KRAJ BSEARCHA I QSORTA

int postojiKorisnik(const char* ime) {
    FILE* fp = fopen("users.txt", "r");
    if (!fp) {
        perror("Greska pri otvaranju datoteke");
        return 0; // pretpostavka da korisnik ne postoji ako datoteka ne moze biti otvorena
    }

    char tmpIme[MAX_USER_CHARACTERS];
    int tmpScore;

    while (fscanf(fp, "%s %d", tmpIme, &tmpScore) == 2) {
        if (strcmp(tmpIme, ime) == 0) {
            fclose(fp);
            return 1; // korisnik postoji
        }
    }

    fclose(fp);

    return 0;
}

void dodajUsera() {
    FILE* fp;
    User noviUser;

    while (1) {
        printf("Unesite ime korisnika: ");
        scanf("%49s", noviUser.ime);

        // Provjeri da li korisnik vec postoji
        int postoji = 0;
        fp = fopen("users.txt", "r");
        if (fp) {
            char tmpIme[MAX_USER_CHARACTERS];
            int tmpScore;
            while (fscanf(fp, "%49s %d", tmpIme, &tmpScore) == 2) {
                if (strcmp(tmpIme, noviUser.ime) == 0) {
                    postoji = 1;
                    break;
                }
            }
            fclose(fp);
        }

        if (postoji) {
            printf("Korisnik sa imenom '%s' vec postoji. Molimo unesite drugo ime.\n", noviUser.ime);
        }
        else {
            break;
        }
    }

    noviUser.score = 0;

    fp = fopen("users.txt", "a");
    if (!fp) {
        perror("Greska pri otvaranju datoteke");
        return;
    }

    fprintf(fp, "%s %d\n", noviUser.ime, noviUser.score);
    fclose(fp);

    printf("*Korisnik dodan!*\n\n");
}

void ucitajUsera() {
    char ime[MAX_USER_CHARACTERS];
    printf("Unesite ime korisnika za ucitavanje: ");
    scanf("%49s", ime);

    FILE* fp = fopen("users.txt", "r");
    if (!fp) {
        perror("Greska pri otvaranju datoteke");
        return;
    }

    char tmpIme[MAX_USER_CHARACTERS];
    int tmpScore;
    int found = 0;

    while (fscanf(fp, "%49s %d", tmpIme, &tmpScore) == 2) {
        if (strcmp(tmpIme, ime) == 0) {
            strcpy(currentUser.ime, tmpIme);
            currentUser.score = tmpScore;
            found = 1;
            break;
        }
    }

    if (found) {
        printf("*Korisnik '%s' uspjesno ucitan s rezultatom %d. Spreman za igru!*\n\n", currentUser.ime, currentUser.score);
    }
    else {
        printf("*Korisnik nije pronaden!*\n\n");
    }

    fclose(fp);
}

void azurirajUsera() {
    char staroIme[MAX_USER_CHARACTERS];
    char novoIme[MAX_USER_CHARACTERS];

    printf("Unesite ime korisnika kojeg zelite azurirati: ");
    scanf("%49s", staroIme);

    if (!postojiKorisnik(staroIme)) {
        printf("*Korisnik nije pronaden!*\n\n");
        return;
    }

    while (1) {
        printf("Unesite novo ime za korisnika: ");
        scanf("%49s", novoIme);

        //provjera postoji li vec user s istim imenom
        int postoji = 0;
        FILE* fp = fopen("users.txt", "r");
        if (fp) {
            char tmpIme[MAX_USER_CHARACTERS];
            int tmpScore;
            while (fscanf(fp, "%49s %d", tmpIme, &tmpScore) == 2) {
                if (strcmp(tmpIme, novoIme) == 0) {
                    postoji = 1;
                    break;
                }
            }
            fclose(fp);
        }

        if (postoji) {
            printf("Korisnik sa imenom '%s' vec postoji. Molimo unesite drugo ime.\n", novoIme);
        }
        else {
            break;
        }
    }

    FILE* fp = fopen("users.txt", "r");
    if (!fp) {
        perror("*Greska pri otvaranju datoteke*\n\n");
        return;
    }

    FILE* fpTmp = fopen("temp.txt", "w");
    if (!fpTmp) {
        perror("*Greska pri otvaranju privremene datoteke*\n\n");
        fclose(fp);
        return;
    }

    char tmpIme[MAX_USER_CHARACTERS];
    int tmpScore;

    while (fscanf(fp, "%49s %d", tmpIme, &tmpScore) == 2) {
        if (strcmp(tmpIme, staroIme) == 0) {
            fprintf(fpTmp, "%s %d\n", novoIme, tmpScore);
        }
        else {
            fprintf(fpTmp, "%s %d\n", tmpIme, tmpScore);
        }
    }

    fclose(fp);
    fclose(fpTmp);

    if (remove("users.txt") != 0) {
        perror("*Greska pri brisanju originalne datoteke*\n\n");
    }
    else if (rename("temp.txt", "users.txt") != 0) {
        perror("*Greska pri preimenovanju privremene datoteke*\n\n");
    }
    else {
        printf("*Ime korisnika uspjesno azurirano!*\n\n");
    }

    if (strcmp(currentUser.ime, staroIme) == 0) {
        strcpy(currentUser.ime, novoIme);
    }
}

void spremiScore() {
    FILE* fp = fopen("users.txt", "r");
    if (!fp) {
        perror("*Greska pri otvaranju datoteke*\n\n");
        return;
    }

    FILE* fpTmp = fopen("temp.txt", "w");
    if (!fpTmp) {
        perror("*Greska pri otvaranju privremene datoteke*\n\n");
        fclose(fp);
        return;
    }

    char tmpIme[MAX_USER_CHARACTERS];
    int tmpScore;

    while (fscanf(fp, "%49s %d", tmpIme, &tmpScore) == 2) {
        if (strcmp(tmpIme, currentUser.ime) == 0) {
            //azurira rezultat samo ako je novi rezultat veci
			if (currentUser.score > tmpScore) {
				fprintf(fpTmp, "%s %d\n", currentUser.ime, currentUser.score);
				printf("Novi rezultat za korisnika %s je %d!\n", currentUser.ime, currentUser.score);
			}

			else {
				fprintf(fpTmp, "%s %d\n", tmpIme, tmpScore);
				printf("Postignuti rezultat %d je manji od trenutnog rezultata %d i nece biti azuriran!\n", currentUser.score, tmpScore);
			}
        }
        else {
            fprintf(fpTmp, "%s %d\n", tmpIme, tmpScore);
        }
    }

    fclose(fp);
    fclose(fpTmp);

    if (remove("users.txt") != 0) {
        perror("*Greska pri brisanju originalne datoteke*\n\n");
    }
    else if (rename("temp.txt", "users.txt") != 0) {
        perror("*Greska pri preimenovanju privremene datoteke*\n\n");
    }
    else {
        printf("\n*Rezultat uspjesno spremljen!*\n\n");
    }
}

// Funkcija za ispis korisnika sortiranih po rezultatima, CALLOC
void ispisiPostojeceUsere() {
    FILE* fp = fopen("users.txt", "r");
    if (!fp) {
        perror("*Greska pri otvaranju datoteke*\n\n");
        return;
    }

    // Vracanje pokazivaca fajla na pocetak (osiguranje)
    rewind(fp);

    // Dinamicka alokacija i inicijalizacija memorije za korisnike
    User* users = (User*)calloc(100, sizeof(User));
    if (!users) {
        perror("Greska pri alokaciji memorije");
        fclose(fp);
        return;
    }

    int userCount = 0;

    while (fscanf(fp, "%49s %d", users[userCount].ime, &users[userCount].score) == 2) {
        userCount++;
    }

    fclose(fp);

    if (userCount == 0) {
        printf("*Nema spremljenih korisnika*\n\n");
        free(users); //oslobadanje memorije
        return;
    }

    qsort(users, userCount, sizeof(User), compareScores);

    printf("\nPostojeci korisnici i njihovi rezultati (sortirano od najveceg prema najmanjem):\n");
    for (int i = 0; i < userCount; i++) {
        printf("Ime: %s, Rezultat: %d\n", users[i].ime, users[i].score);
    }

    User* najboljiKorisnik = pronadiNajveciRezultat(users, userCount);
    if (najboljiKorisnik != NULL) {
        printf("\nKorisnik s najvecim rezultatom: %s, Rezultat: %d\n\n", najboljiKorisnik->ime, najboljiKorisnik->score);
    }

    free(users); // Oslobadanje memorije
}

void obrisiUsera() {
    char ime[MAX_USER_CHARACTERS];

    printf("Unesite ime korisnika za brisanje: ");
    scanf("%49s", ime);

    FILE* fp = fopen("users.txt", "r");
    if (!fp) {
        perror("*Greska pri otvaranju datoteke*");
        return;
    }

    FILE* fpTmp = fopen("temp.txt", "w");
    if (!fpTmp) {
        perror("*Greska pri otvaranju privremene datoteke*");
        fclose(fp);
        return;
    }

    char tmpIme[MAX_USER_CHARACTERS];
    int tmpScore, found = 0;

    while (fscanf(fp, "%49s %d", tmpIme, &tmpScore) == 2) {
        if (strcmp(tmpIme, ime) != 0) {
            fprintf(fpTmp, "%s %d\n", tmpIme, tmpScore);
        }
        else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(fpTmp);

    if (found) {
        if (remove("users.txt") != 0) {
            perror("*Greska pri brisanju originalne datoteke*");
        }
        else if (rename("temp.txt", "users.txt") != 0) {
            perror("*Greska pri preimenovanju privremene datoteke*");
        }
        else {
            printf("*Korisnik obrisan!*\n\n");
            // resetiranje trenutnog korisnika ako je isti kao obrisani
            if (strcmp(currentUser.ime, ime) == 0) {
                currentUser.ime[0] = '\0';
                currentUser.score = 0;
            }
        }
    }
    else {
        if (remove("temp.txt") != 0) {
            perror("*Greska pri brisanju privremene datoteke*");
        }
        printf("*Korisnik nije pronaden!*\n\n");
    }
}



// LOGIKA



void pokreniIgru() {
    if (strlen(currentUser.ime) == 0) {
        printf("*Nijedan korisnik nije ucitan*\n");

        FILE* fp = fopen("users.txt", "r");
        if (!fp) {
            perror("*Greska pri otvaranju datoteke*");
            return;
        }

        if (fgetc(fp) == EOF) { // Provjera je li datoteka prazna
            fclose(fp);
            printf("Nema spremljenih korisnika. Unesite ime novog korisnika: ");
            char ime[MAX_USER_CHARACTERS];
            scanf("%49s", ime);
            dodajUseraDirektno(ime);
        }
        else {
            fclose(fp);
            printf("Unesite ime korisnika kojeg zelite ucitati za igru: ");
            char ime[MAX_USER_CHARACTERS];
            scanf("%49s", ime);

            fp = fopen("users.txt", "r");
            if (!fp) {
                perror("*Greska pri otvaranju datoteke*");
                return;
            }

            char tmpIme[MAX_USER_CHARACTERS];
            int tmpScore;
            int found = 0;

            while (fscanf(fp, "%49s %d", tmpIme, &tmpScore) == 2) {
                if (strcmp(tmpIme, ime) == 0) {
                    strcpy(currentUser.ime, tmpIme);
                    currentUser.score = tmpScore;
                    found = 1;
                    break;
                }
            }

            fclose(fp);

            if (!found) {
                printf("Korisnik nije pronaden. Molimo pokusajte ponovno.\n\n");
                return;
            }
            else {
                printf("Korisnik %s uspjesno ucitan s rezultatom %d. Spreman za igru!\n", currentUser.ime, currentUser.score);
            }
        }
    }
    else {
        // Ponovna provjera da li je korisnik zaista prisutan u datoteci
        FILE* fp = fopen("users.txt", "r");
        if (!fp) {
            perror("Greska pri otvaranju datoteke");
            return;
        }

        char tmpIme[MAX_USER_CHARACTERS];
        int tmpScore;
        int found = 0;
        while (fscanf(fp, "%49s %d", tmpIme, &tmpScore) == 2) {
            if (strcmp(tmpIme, currentUser.ime) == 0) {
                found = 1;
                break;
            }
        }
        fclose(fp);

        //u slucaju da je korisnik obrisan (resetirat ce ga)
        if (!found) {
            printf("Trenutni korisnik vise ne postoji. Molimo ucitajte drugog korisnika.\n");
            currentUser.ime[0] = '\0';
            currentUser.score = 0;
            return;
        }
    }

    Snake snake;
    Position food;
    bool gameOver = false;

    initGame(&snake, &food);
    printf("Igra pocinje za korisnika: %s\n", currentUser.ime);
    while (!gameOver) {
        printBoard(snake, food);
        processInput(&snake);
        Sleep(200);  // Sleep funkcija na Windowsu (vrijeme je u milisekundama)
        updateSnake(&snake, &food, &gameOver);
    }
    printf("Igra zavrsena! Vas rezultat: %d\n", snake.length - 1);

    // Ažuriranje rezultata korisnika
    currentUser.score = snake.length - 1;

    // Spremanje ažuriranog rezultata u datoteku
    spremiScore();
}

void dodajUseraDirektno(char* ime) {
    FILE* fp = fopen("users.txt", "a");
    if (!fp) {
        perror("Greska pri otvaranju datoteke");
        return;
    }

    User noviUser;
    strcpy(noviUser.ime, ime);
    noviUser.score = 0;

    fprintf(fp, "%s %d\n", noviUser.ime, noviUser.score);
    fclose(fp);

    strcpy(currentUser.ime, ime);
    currentUser.score = 0;

    printf("Korisnik %s dodan i ucitan za igru.\n", currentUser.ime);
}

//deklarira se pozicija zmije i generira pozicija hrane
void initGame(Snake* snake, Position* food) { 
    srand(time(NULL)); // Inicijalizacija generatora slučajnih brojeva
    snake->body[0].x = BOARD_WIDTH / 2;
    snake->body[0].y = BOARD_HEIGHT / 2;
    snake->length = 1;
    snake->direction = 'r';

    generateFood(food, *snake);
}

void printBoard(Snake snake, Position food) {
    system("cls"); // Čisti ekran na Windowsu

    //za svaki stupac prodji kroz njegove redove x=red, y=stupac
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (x == 0 || x == BOARD_WIDTH - 1 || y == 0 || y == BOARD_HEIGHT - 1) { 
                printf("#"); // Rub ploče
            }
            else if (x == snake.body[0].x && y == snake.body[0].y) {
                printf("O"); // Glava zmije
            }
            else if (x == food.x && y == food.y) {
                printf("*"); // Hrana
            }
            else {
                bool isBodyPart = false;

                //krecemo od 1 jer je glava vec iscrtana
                for (int i = 1; i < snake.length; i++) {
                    if (x == snake.body[i].x && y == snake.body[i].y) {
                        printf("o"); // Tijelo zmije
                        isBodyPart = true;
                        break;
                    }
                }
                if (!isBodyPart) {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
}

void updateSnake(Snake* snake, Position* food, bool* gameOver) {
    // Prolazi se kroz for obrnuto (odozada, od zmijinog repa) i pomice se svaki dio odozdola za jedan iznad zbog refresha
    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }

    // Pomakni glavu zmije
    switch (snake->direction) {
    case 'l': snake->body[0].x--; break;
    case 'r': snake->body[0].x++; break;
    case 'u': snake->body[0].y--; break;
    case 'd': snake->body[0].y++; break;
    }

    // Detekcija sudara sa zidom (ako je <1 onda je to zid)
    if (snake->body[0].x < 1 || snake->body[0].x >= BOARD_WIDTH - 1 ||
        snake->body[0].y < 1 || snake->body[0].y >= BOARD_HEIGHT - 1) {
        *gameOver = true;
    }

    // Detekcija sudara sa samim sobom
    if (!*gameOver) {
        *gameOver = checkCollision(snake->body[0], *snake); //salju se kao parametri glava i cijela zmija
    }

    // Provjeri da li je zmija pojela hranu
    if (!*gameOver && snake->body[0].x == food->x && snake->body[0].y == food->y) {
        snake->length++;
        generateFood(food, *snake);
    }
}

bool checkCollision(Position pos, Snake snake) {
    //za svaki dio tijela provjeri jeli glava na istoj poziciji, ako je doslo je do sudara sa samim sobom
    for (int i = 1; i < snake.length; i++) {
        if (pos.x == snake.body[i].x && pos.y == snake.body[i].y) {
            return true; //checkCollision = true
        }
    }
    return false; //ako nije doslo do sudara, checkCollision = false
}

void generateFood(Position* food, Snake snake) {
	int validPosition = 0;

	while (!validPosition) {
		food->x = rand() % (BOARD_WIDTH - 2) + 1;
		food->y = rand() % (BOARD_HEIGHT - 2) + 1;
		validPosition = 1;

        //nakon generiranja pozicije hrane provjeravamo nalazi li se u tijelu zmije, ako da validPosition=0 i ponavlja se while
		for (int i = 0; i < snake.length; i++) {
			if (food->x == snake.body[i].x && food->y == snake.body[i].y) {
				validPosition = 0;
				break;
			}
		}
	}
}

void processInput(Snake* snake) {
    if (_kbhit()) {
        char key = _getch();
        switch (key) {
        case 'w':
        case 'W':
            if (snake->direction != 'd') {
                snake->direction = 'u';
            }
            break;
        case 's':
        case 'S':
            if (snake->direction != 'u') {
                snake->direction = 'd';
            }
            break;
        case 'a':
        case 'A':
            if (snake->direction != 'r') {
                snake->direction = 'l';
            }
            break;
        case 'd':
        case 'D':
            if (snake->direction != 'l') {
                snake->direction = 'r';
            }
            break;
        }
    }
}