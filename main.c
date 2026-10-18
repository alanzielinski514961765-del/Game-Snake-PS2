#include <tamtypes.h>
#include <kernel.h>
#include <sifcmd.h>
#include <sifrpc.h>
#include <graph.h>
#include <draw.h>
#include <libpad.h>

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 448
#define BLOCK_SIZE    20

typedef struct {
    int x, y;
} Vector2;

static char pad_buf[256] __attribute__((aligned(64)));

int main(int argc, char *argv[]) {
    SifInitRpc(0);
    padInit(0);
    
    graph_set_mode(0, GRAPH_MODE_NTSC, GRAPH_MODE_INTERLACED, GRAPH_MODE_FIELD);
    
    qword_t packet[cache_align(128)];
    qword_t *q;

    padPortOpen(0, 0, pad_buf);

    // Inicjalizacja węża
    Vector2 snake[50];
    int snake_len = 3;
    snake[0] = (Vector2){16, 11};
    snake[1] = (Vector2){15, 11};
    snake[2] = (Vector2){14, 11};

    int dx = 1;
    int dy = 0;
    Vector2 apple = {20, 11};
    int frame_counter = 0;

    while(1) {
        // Obsługa pada PS2 (strzałki do zmiany kierunku)
        struct padButtonStatus buttons;
        if (padRead(0, 0, &buttons) > 0) {
            u32 paddata = 0xffff ^ buttons.btns;
            if ((paddata & PAD_LEFT) && dx == 0) { dx = -1; dy = 0; }
            else if ((paddata & PAD_RIGHT) && dx == 0) { dx = 1; dy = 0; }
            else if ((paddata & PAD_UP) && dy == 0) { dx = 0; dy = -1; }
            else if ((paddata & PAD_DOWN) && dy == 0) { dx = 0; dy = 1; }
        }

        // Logika ruchu węża co kilka klatek
        if (++frame_counter > 15) {
            frame_counter = 0;
            for (int i = snake_len - 1; i > 0; i--) {
                snake[i] = snake[i - 1];
            }
            snake[0].x += dx;
            snake[0].y += dy;

            // Zawijanie na krawędziach ekranu
            if (snake[0].x < 0) snake[0].x = (SCREEN_WIDTH / BLOCK_SIZE) - 1;
            if (snake[0].x >= SCREEN_WIDTH / BLOCK_SIZE) snake[0].x = 0;
            if (snake[0].y < 0) snake[0].y = (SCREEN_HEIGHT / BLOCK_SIZE) - 1;
            if (snake[0].y >= SCREEN_HEIGHT / BLOCK_SIZE) snake[0].y = 0;

            // Zjadanie jabłka i rośnięcie
            if (snake[0].x == apple.x && snake[0].y == apple.y) {
                if (snake_len < 50) snake_len++;
                apple.x = (apple.x * 7 + 3) % (SCREEN_WIDTH / BLOCK_SIZE);
                apple.y = (apple.y * 5 + 2) % (SCREEN_HEIGHT / BLOCK_SIZE);
            }
        }

        // Renderowanie grafiki na ekranie PS2
        q = packet;
        q = draw_clear(q, 0, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 15, 15, 30);

        // Rysowanie jabłka (czerwony kwadrat)
        q = draw_rect(q, 0, 
            (float)(apple.x * BLOCK_SIZE), (float)(apple.y * BLOCK_SIZE), 
            (float)BLOCK_SIZE, (float)BLOCK_SIZE, 
            255, 0, 0);

        // Rysowanie węża (zielone kwadraty)
        for (int i = 0; i < snake_len; i++) {
            q = draw_rect(q, 0, 
                (float)(snake[i].x * BLOCK_SIZE), (float)(snake[i].y * BLOCK_SIZE), 
                (float)BLOCK_SIZE, (float)BLOCK_SIZE, 
                0, 255, 0);
        }

        q = draw_finish(q);
        draw_exec_packet(packet, q - packet);

        graph_wait_vsync();
    }
    return 0;
}
