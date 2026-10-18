#include <tamtypes.h>
#include <kernel.h>
#include <controller.h>
#include <graph.h>
#include <draw.h>

// Prosta struktura węża
typedef struct {
    int x, y;
} Point;

#define MAX_SNAKE 100

int main() {
    // Inicjalizacja systemowa PS2
    init_scr();
    scr_printf("Snake PS2 - Start Gry\n");

    // Inicjalizacja pada
    padInit(0);
    int port = 0, slot = 0;
    
    // Główna pętla gry
    while (1) {
        // Tutaj w pełnej wersji obsługujemy stan pada, ruch węża i rysowanie na ekranie
        // Na razie szkielet inicjalizujący grafikę i kontrolery dla ISO
    }

    return 0;
}
