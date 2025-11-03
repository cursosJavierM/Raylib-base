#include <raylib.h>
#include <random>

// --- VARIABLES GLOBALES ---
int puntos_j1 = 0;
int puntos_j2 = 0;

bool pausa = true;
int ronda_gan = 0; // 0 - Ninguno
				   // 1 - Jugador 1
				   // 2 - Jugador 2

Sound sonido_gol;

const int SIZE_PANTALLA_ANCHO = 1280;
const int SIZE_PANTALLA_ALTO = 800;
const int FPS_MAXIMOS = 60;

// --- OBJETOS DEL JUEGO ---

struct Pelota
{
	int velocidad = 1;
	Vector2 posicion;
	Vector2 direccion;
	int numeroChoques = 0;
	int size = 15;

	Pelota(
		Vector2 posicionIncial,
		Vector2 direccionIncial)
	{
		posicion = posicionIncial;
		direccion = direccionIncial;
	}

	void botar()
	{
		// invertir la dirección
		direccion.x = direccion.x * -1;
		direccion.y = direccion.y * -1;
	}

	void draw()
	{
		DrawCircle(posicion.x, posicion.y, size, WHITE);
	}

	void actualizar()
	{
		float nuevaPosicionX = posicion.x + (direccion.x * velocidad);
		float nuevaPosicionY = posicion.y + (direccion.y * velocidad);

		posicion = {nuevaPosicionX, nuevaPosicionY};
	}
};

struct Pala
{
	Vector2 posicion;
	int size;
	int velocidad;
	// controlesDefinidos;

	Pala(int tamano, int velocidadInicial, Vector2 posicionIncial)
	{
		size = tamano;
		velocidad = velocidadInicial;
		posicion = posicionIncial;
	}
};

int main()
{
	// --- crear una ventana ---
	InitWindow(SIZE_PANTALLA_ANCHO, SIZE_PANTALLA_ALTO, "Pong"); // Crea la ventana de juego
	InitAudioDevice();							 // Inicializar audio
	SetTargetFPS(FPS_MAXIMOS);					 // Limitar el máximo de Frames per second (Fotogramas por segundo)

	// --- Inicializar objetos del juego ---
	Pelota pelota = Pelota(
		{SIZE_PANTALLA_ANCHO / 2.0, SIZE_PANTALLA_ALTO / 2.0},
		{1.0, 2.0});

	// bucle principal del juego
	while (!WindowShouldClose())
	{
		// --- COMPROBAR imputs "entradas" del jugador. Pulsar teclas... ---
		if (IsKeyPressed(KEY_ENTER))
		{
		}

		// --- ACTUALIZAR objetos ---
		pelota.actualizar();

		// --- PINTAR en pantalla ---
		BeginDrawing();			// Función de la librería que se pone antes de Dibujar en pantalla.
		ClearBackground(BLACK); // Pintar de un color (NEGRO) el fondo. Esto borra lo que ya está dibujado anteriormente.

		pelota.draw();

		EndDrawing(); // Función de la librería que libera el proceso de PINTAR.
	}

	// Liberar recursos que hemos cargado al principio, como sonidos, imagenes.
	UnloadSound(sonido_gol);

	// Cerrar los recursos que hemos abierto, como audio y ventana.
	CloseAudioDevice();
	CloseWindow();
}
