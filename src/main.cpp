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

const int ANCHURA_PALA = 30;
const int ALTURA_PALA = 200;

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

		if (nuevaPosicionX + size > SIZE_PANTALLA_ANCHO || nuevaPosicionX - size < 0)
		{
			// Todo: Modificar para puntuar. Un jugador ha metido gol.
			direccion.x = direccion.x * -1;
			nuevaPosicionX = posicion.x + (direccion.x * velocidad);
			// Pelota se fue de la pantalla
		}
		if (nuevaPosicionY + size > SIZE_PANTALLA_ALTO || nuevaPosicionY - size < 0)
		{
			direccion.y = direccion.y * -1;
			nuevaPosicionY = posicion.y + (direccion.y * velocidad);
		}
		posicion = {nuevaPosicionX, nuevaPosicionY};
	}
};

struct Pala
{
	Vector2 posicion;
	int anchura;
	int altura;
	int velocidad;
	// controlesDefinidos;

	Pala(int velocidadInicial, Vector2 posicionIncial,
		 int anchuraIncial, int alturaInicial)
	{
		anchura = anchuraIncial;
		altura = alturaInicial;
		velocidad = velocidadInicial;
		posicion = posicionIncial;
	}

	void draw()
	{
		DrawRectangle(posicion.x, posicion.y, anchura, altura, WHITE);
	}

	void actualizar(float desplazamiento)
	{
		float nuevaPosicionY = posicion.y + (desplazamiento * velocidad);

		if (nuevaPosicionY >= 0 && nuevaPosicionY < SIZE_PANTALLA_ALTO - ALTURA_PALA)
		{
			posicion.y = nuevaPosicionY;
		}
	}

	bool aChocado(Vector2 otroObjeto)
	{
		// Comprobar si la pala ha chocado con la bola.
		if (posicion.x + anchura)
		{
		}
	}
};

int main()
{
	// --- crear una ventana ---
	InitWindow(SIZE_PANTALLA_ANCHO, SIZE_PANTALLA_ALTO, "Pong"); // Crea la ventana de juego
	InitAudioDevice();											 // Inicializar audio
	SetTargetFPS(FPS_MAXIMOS);									 // Limitar el máximo de Frames per second (Fotogramas por segundo)

	// --- Inicializar objetos del juego ---
	Pelota pelota = Pelota(
		{SIZE_PANTALLA_ANCHO / 2.0, SIZE_PANTALLA_ALTO / 2.0},
		{1.0, 2.0});

	Pala palaJugador1 = Pala(5, {SIZE_PANTALLA_ANCHO - ANCHURA_PALA, SIZE_PANTALLA_ALTO / 2 - ALTURA_PALA / 2},
							 ANCHURA_PALA, ALTURA_PALA);

	// bucle principal del juego
	while (!WindowShouldClose())
	{
		// --- COMPROBAR imputs "entradas" del jugador. Pulsar teclas... ---
		if (IsKeyPressed(KEY_ENTER))
		{
		}
		if (IsKeyDown(KEY_UP))
		{
			palaJugador1.actualizar(-1);
		}
		if (IsKeyDown(KEY_DOWN))
		{
			palaJugador1.actualizar(1);
		}

		// --- ACTUALIZAR objetos ---
		pelota.actualizar();

		// --- PINTAR en pantalla ---
		BeginDrawing();			// Función de la librería que se pone antes de Dibujar en pantalla.
		ClearBackground(BLACK); // Pintar de un color (NEGRO) el fondo. Esto borra lo que ya está dibujado anteriormente.

		pelota.draw();
		palaJugador1.draw();

		EndDrawing(); // Función de la librería que libera el proceso de PINTAR.
	}

	// Liberar recursos que hemos cargado al principio, como sonidos, imagenes.
	UnloadSound(sonido_gol);

	// Cerrar los recursos que hemos abierto, como audio y ventana.
	CloseAudioDevice();
	CloseWindow();
}
