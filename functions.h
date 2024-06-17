#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdbool.h>

#define BOARD_WIDTH 20
#define BOARD_HEIGHT 20

#define MAX_USER_CHARACTERS 50
#define MAX_BODY_LENGTH 100

typedef struct {
    char ime[MAX_USER_CHARACTERS]; // STATICKI ZAUZETO POLJE
    int score;
} User;

extern User currentUser; // globalna varijabla za trenutnog korisnika, EXTERN

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position body[MAX_BODY_LENGTH]; // max duzina zmije
    int length;
    char direction;
} Snake;

int compareScores(const void* a, const void* b);
User* pronadiNajveciRezultat(User* users, int userCount);
void ispisiPostojeceUsere();

int postojiKorisnik(const char* ime);
void dodajUsera();
void ucitajUsera();
void azurirajUsera();
void ispisiPostojeceUsere();
void obrisiUsera();
void dodajUseraDirektno(char* ime);
void pokreniIgru();
inline void zavrsiProgram() {
    exit(0);
} //INLINE FUNKCIJA

void initGame(Snake* snake, Position* food);
void printBoard(Snake snake, Position food);
void updateSnake(Snake* snake, Position* food, bool* gameOver);
bool checkCollision(Position pos, Snake snake);
void generateFood(Position* food, Snake snake);
void processInput(Snake* snake);

#endif