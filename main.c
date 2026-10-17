#include <stdio.h>
#include <stdlib.h>
#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <libpad.h>

// Ustawienia planszy (siatka)
#define GRID_WIDTH 32
#define GRID_HEIGHT 24
#define MAX_SNAKE_LENGTH 100

typedef struct {
    int x, y;
} Vector2;

typedef struct {
    Vector2 body[MAX_SNAKE_LENGTH];
    int length;
    Vector2 dir;
} Snake;

static char padBuf[256] __attribute__((aligned(64)));

void initPad() {
    SifInitRpc(0);
    while (!SifBindRpc(NULL, 0x0000001e, 0)); // Czekaj na moduł PAD
    padInit(0);
    padPortOpen(0, 0, padBuf);
}

int readPadDirection(Vector2 *currentDir) {
    struct padButtonStatus buttons;
    int ret = padRead(0, 0, &buttons);
    
    if (ret > 0) {
        u32 paddata = 0xffff ^ buttons.btns;
        
        // Obsługa D-Pad (Strzałek) na padzie PS2
        if ((paddata & PAD_UP) && currentDir->y == 0) {
            currentDir->x = 0; currentDir->y = -1;
        } else if ((paddata & PAD_DOWN) && currentDir->y == 0) {
            currentDir->x = 0; currentDir->y = 1;
        } else if ((paddata & PAD_LEFT) && currentDir->x == 0) {
            currentDir->x = -1; currentDir->y = 0;
        } else if ((paddata & PAD_RIGHT) && currentDir->x == 0) {
            currentDir->x = 1; currentDir->y = 0;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    initPad();

    Snake snake;
    snake.length = 3;
    snake.dir.x = 1; // Startowy ruch w prawo
    snake.dir.y = 0;

    // Początkowa pozycja węża
    for (int i = 0; i < snake.length; i++) {
        snake.body[i].x = 10 - i;
        snake.body[i].y = 10;
    }

    Vector2 food = { 15, 10 };
    int score = 0;

    // Główna pętla gry
    while (1) {
        // 1. Odczyt stanu pada (sterowanie)
        readPadDirection(&snake.dir);

        // 2. Aktualizacja pozycji węża
        Vector2 newHead = { snake.body[0].x + snake.dir.x, snake.body[0].y + snake.dir.y };

        // Zawijanie krawędzi planszy
        if (newHead.x < 0) newHead.x = GRID_WIDTH - 1;
        if (newHead.x >= GRID_WIDTH) newHead.x = 0;
        if (newHead.y < 0) newHead.y = GRID_HEIGHT - 1;
        if (newHead.y >= GRID_HEIGHT) newHead.y = 0;

        // Sprawdzenie zjedzenia jedzenia
        if (newHead.x == food.x && newHead.y == food.y) {
            score++;
            food.x = (food.x * 3 + 5) % GRID_WIDTH;
            food.y = (food.y * 7 + 2) % GRID_HEIGHT;
            if (snake.length < MAX_SNAKE_LENGTH) {
                snake.length++;
            }
        } else {
            // Przesunięcie ciała węża
            for (int i = snake.length - 1; i > 0; i--) {
                snake.body[i] = snake.body[i - 1];
            }
        }

        snake.body[0] = newHead;

        // Proste opóźnienie klatki, żeby gra nie pędziła w nieskończoność
        for (volatile int i = 0; i < 1500000; i++);
    }
    
    return 0;
}
