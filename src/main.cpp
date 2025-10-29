#include <raylib.h>
#include <random>

// variables globales
int puntos_j1 = 0;
int puntos_j2 = 0;

bool pausa = true;
int ronda_gan = 0; // 0 - Ninguno
				   // 1 - Jugador 1
				   // 2 - Jugador 2

Sound sonido_gol;

struct Bola
{
};

struct Pala
{
};

int main()
{
	// crear una ventana
	InitWindow(1280, 800, "Pong");
	InitAudioDevice(); // Inicializar audio
	SetTargetFPS(60);  // Limitar el máximo de Frames per second (Fotogramas por segundo)

	// Inicializar objetos del juego

	// bucle del juego
	while (!WindowShouldClose())
	{
		// --- COMPROBAR imputs "entradas" del jugador. Pulsar teclas... ---
		if (IsKeyPressed(KEY_ENTER))
		{
		}

		// --- ACTUALIZAR objetos ---

		// PINTAR en pantalla
		BeginDrawing();			// Función de la librería que se pone antes de Dibujar en pantalla.
		ClearBackground(BLACK); // Pintar de un color (NEGRO) el fondo. Esto borra lo que ya está dibujado anteriormente.

		EndDrawing(); // Función de la librería que libera el proceso de PINTAR.
	}

	// Liberar recursos que hemos cargado al principio, como sonidos, imagenes.
	UnloadSound(sonido_gol);

	// Cerrar los recursos que hemos abierto, como audio y ventana.
	CloseAudioDevice();
	CloseWindow();
}
