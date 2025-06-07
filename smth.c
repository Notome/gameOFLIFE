#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define HEIGHT 25
#define WIDTH 80

int speed = 100;
char current_pattern = '1';

int wrap(int coord, int max) {
    if (coord < 0) return max - 1;
    if (coord >= max) return 0;
    return coord;
}

void initGlider(char field[HEIGHT][WIDTH]);
void initRandom(char field[HEIGHT][WIDTH]);
void initBlinker(char field[HEIGHT][WIDTH]);
void initGosperGliderGun(char field[HEIGHT][WIDTH]);
void initSpaceship(char field[HEIGHT][WIDTH]);
void drawField(char field[HEIGHT][WIDTH]);
void printMatrix(char field[HEIGHT][WIDTH]);
void checkRules(char field[HEIGHT][WIDTH]);
void changeField(char field[HEIGHT][WIDTH], int i, int j, char nextField[HEIGHT][WIDTH]);
void clearScreen();

int main() {
    char matrix[HEIGHT][WIDTH];
    srand((unsigned int)time(NULL));
    
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    timeout(0);

    switch(current_pattern) {
        case '1': initGlider(matrix); break;
        case '2': initRandom(matrix); break;
        case '3': initBlinker(matrix); break;
        case '4': initGosperGliderGun(matrix); break;
        case '5': initSpaceship(matrix); break;
    }

    while (1) {
        clearScreen();
        printMatrix(matrix);
        
        int input = getch();
        if (input == ' ') break;
        if (input >= 'a' && input <= 'z') {
            speed = 1000 - (input - 'a') * 38;
            if (speed < 10) speed = 10;
        }
        if (input >= '1' && input <= '5') {
            current_pattern = input;
            drawField(matrix);
            switch(current_pattern) {
                case '1': initGlider(matrix); break;
                case '2': initRandom(matrix); break;
                case '3': initBlinker(matrix); break;
                case '4': initGosperGliderGun(matrix); break;
                case '5': initSpaceship(matrix); break;
            }
        }
        
        checkRules(matrix);
        napms(speed);
    }

    endwin();
    return 0;
}

void clearScreen() {
    clear();
}

void drawField(char field[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            field[i][j] = ' ';
        }
    }
}

void printMatrix(char field[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            mvaddch(i, j, field[i][j]);
        }
    }
    mvprintw(HEIGHT, 0, "Управление: a-z (скорость), 1-5 (узоры), пробел (выход)");
    mvprintw(HEIGHT + 1, 0, "Текущая скорость: %d мс  Узoр: %c", speed, current_pattern);
}

void changeField(char field[HEIGHT][WIDTH], int i, int j, char nextField[HEIGHT][WIDTH]) {
    int count = 0;
    
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            if (di == 0 && dj == 0) continue;
            int ni = wrap(i + di, HEIGHT);
            int nj = wrap(j + dj, WIDTH);
            if (field[ni][nj] == '#') {
                count++;
            }
        }
    }

    if (field[i][j] == '#') {
        nextField[i][j] = (count == 2 || count == 3) ? '#' : ' ';
    } else {
        nextField[i][j] = (count == 3) ? '#' : ' ';
    }
}

void checkRules(char field[HEIGHT][WIDTH]) {
    char nextField[HEIGHT][WIDTH];
    drawField(nextField);

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            changeField(field, i, j, nextField);
        }
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            field[i][j] = nextField[i][j];
        }
    }
}

void initGlider(char field[HEIGHT][WIDTH]) {
    int centerY = HEIGHT / 2, centerX = WIDTH / 2;
    field[wrap(centerY, HEIGHT)][wrap(centerX, WIDTH)] = '#';
    field[wrap(centerY + 1, HEIGHT)][wrap(centerX + 1, WIDTH)] = '#';
    field[wrap(centerY + 2, HEIGHT)][wrap(centerX - 1, WIDTH)] = '#';
    field[wrap(centerY + 2, HEIGHT)][wrap(centerX, WIDTH)] = '#';
    field[wrap(centerY + 2, HEIGHT)][wrap(centerX + 1, WIDTH)] = '#';
}

void initRandom(char field[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            field[i][j] = (rand() % 5 == 0) ? '#' : ' ';
        }
    }
}

void initBlinker(char field[HEIGHT][WIDTH]) {
    int centerY = HEIGHT / 2, centerX = WIDTH / 2;
    field[wrap(centerY, HEIGHT)][wrap(centerX - 1, WIDTH)] = '#';
    field[wrap(centerY, HEIGHT)][wrap(centerX, WIDTH)] = '#';
    field[wrap(centerY, HEIGHT)][wrap(centerX + 1, WIDTH)] = '#';
}

void initGosperGliderGun(char field[HEIGHT][WIDTH]) {
    drawField(field);
    int centerY = HEIGHT / 2, centerX = WIDTH / 2;
    field[centerY][centerX] = '#';
    field[centerY + 1][centerX] = '#';
    field[centerY][centerX + 1] = '#';
    field[centerY + 1][centerX + 1] = '#';
    field[centerY][centerX + 10] = '#';
    field[centerY + 1][centerX + 10] = '#';
    field[centerY + 2][centerX + 10] = '#';
    field[centerY + 3][centerX + 11] = '#';
    field[centerY - 1][centerX + 11] = '#';
    field[centerY + 4][centerX + 12] = '#';
    field[centerY - 2][centerX + 12] = '#';
    field[centerY + 4][centerX + 13] = '#';
    field[centerY - 2][centerX + 13] = '#';
    field[centerY + 1][centerX + 14] = '#';
    field[centerY + 3][centerX + 15] = '#';
    field[centerY - 1][centerX + 15] = '#';
    field[centerY][centerX + 16] = '#';
    field[centerY + 1][centerX + 16] = '#';
    field[centerY + 2][centerX + 16] = '#';
    field[centerY + 1][centerX + 17] = '#';
    field[centerY - 2][centerX + 20] = '#';
    field[centerY - 1][centerX + 20] = '#';
    field[centerY][centerX + 20] = '#';
    field[centerY - 2][centerX + 21] = '#';
    field[centerY - 1][centerX + 21] = '#';
    field[centerY][centerX + 21] = '#';
    field[centerY - 3][centerX + 22] = '#';
    field[centerY + 1][centerX + 22] = '#';
    field[centerY - 3][centerX + 24] = '#';
    field[centerY - 4][centerX + 24] = '#';
    field[centerY + 1][centerX + 24] = '#';
    field[centerY + 2][centerX + 24] = '#';
}

void initSpaceship(char field[HEIGHT][WIDTH]) {
    drawField(field);
    int centerY = HEIGHT / 2, centerX = WIDTH / 2;
    field[centerY][centerX] = '#';
    field[centerY][centerX + 3] = '#';
    field[centerY + 1][centerX + 4] = '#';
    field[centerY + 2][centerX] = '#';
    field[centerY + 2][centerX + 4] = '#';
    field[centerY + 3][centerX + 1] = '#';
    field[centerY + 3][centerX + 2] = '#';
    field[centerY + 3][centerX + 3] = '#';
    field[centerY + 3][centerX + 4] = '#';
}

