#include <raylib.h>
#include <random>

// --- VARIABLES GLOBALES ---
int puntos_j1 = 0;
int puntos_j2 = 0;

bool pausa = true;
int ronda_gan = 0; // 0 - Ninguno
				   // 1 - Jugador 1
				   // 2 - Jugador 2

Sound sonido_gol; // Sonido
Sound sonido_golpe;

const int SIZE_PANTALLA_ANCHO = 1280;
const int SIZE_PANTALLA_ALTO = 800;
const int FPS_MAXIMOS = 60;

const int ANCHURA_PALA = 30;
const int ALTURA_PALA = 200;

// --- OBJETOS DEL JUEGO ---

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

	void draw() // Dibuja la pala.
	{
		DrawRectangle(posicion.x, posicion.y, anchura, altura, WHITE);
	}

	void actualizar(float desplazamiento) // Actualiza la posición de la pala.
	{
		float nuevaPosicionY = posicion.y + (desplazamiento * velocidad);

		if (nuevaPosicionY >= 0 && nuevaPosicionY < SIZE_PANTALLA_ALTO - ALTURA_PALA)
		{
			posicion.y = nuevaPosicionY;
		}
	}
};

struct Pelota
{
	int velocidad = 2; // factor multiplicativo de la dirección.
	Vector2 posicion;
	Vector2 direccion;
	int numeroChoques = 0;
	int size = 15;

	Pelota(
		Vector2 posicionIncial,
		Vector2 direccionIncial) // Constructor del objeto Pelota
	{
		posicion = posicionIncial;
		direccion = direccionIncial;
	}

	void botar() // invertir la dirección. Se aumenta el contador de choques.
	{
		direccion.x = direccion.x * -1;
		direccion.y = direccion.y * -1;
		numeroChoques++;
		PlaySound(sonido_golpe);
	}

	void draw() // dibuja la pelota.
	{
		DrawCircle(posicion.x, posicion.y, size, WHITE);
	}

	void actualizar(Pala pala, Pala pala2) // desplazar la pelota y comprobar coches con palas y pantalla.
	{
		float nuevaPosicionX = posicion.x + (direccion.x * velocidad);
		float nuevaPosicionY = posicion.y + (direccion.y * velocidad);

		if (aChocado({nuevaPosicionX, nuevaPosicionY}, pala, pala2))
		{
			botar();
			nuevaPosicionX = posicion.x + (direccion.x * velocidad);
			nuevaPosicionY = posicion.y + (direccion.y * velocidad);
		}

		// Comprobar si cuando la pelota se desplace choca con el borde izquierdo o derecho.
		if (nuevaPosicionX + size > SIZE_PANTALLA_ANCHO || nuevaPosicionX - size < 0)
		{
			// Por hacer: Modificar para puntuar, a cada jugador.
			direccion.x = direccion.x * -1;
			nuevaPosicionX = posicion.x + (direccion.x * velocidad);
		}
		// Comprobar si cuando la pelota se desplace choca con el borde superior o inferior.
		if (nuevaPosicionY + size > SIZE_PANTALLA_ALTO || nuevaPosicionY - size < 0)
		{
			direccion.y = direccion.y * -1;
			nuevaPosicionY = posicion.y + (direccion.y * velocidad);
		}
		posicion = {nuevaPosicionX, nuevaPosicionY};
	}

	bool aChocado(Vector2 nuevaPosicion, Pala pala, Pala pala2) //  Comprobar si alguna pala ha chocado con la bola.
	{
		// Generamos un centro ficticio en el rectángulo que define la Pala.
		Vector2 centroRectangulo = {
			pala.posicion.x + pala.anchura / 2,
			pala.posicion.y + pala.altura / 2};

		// Distancia entre los centros de la bola y la pala.
		Vector2 distanciaCentros = {
			fabsf(nuevaPosicion.x - centroRectangulo.x),
			fabsf(nuevaPosicion.y - centroRectangulo.y)};

		// Generamos un centro ficticio en el rectángulo que define la Pala2.
		Vector2 centroRectangulo2 = {
			pala2.posicion.x + pala2.anchura / 2,
			pala2.posicion.y + pala2.altura / 2};

		// Distancia entre los centros de la bola y la pala.
		Vector2 distanciaCentros2 = {
			fabsf(nuevaPosicion.x - centroRectangulo2.x),
			fabsf(nuevaPosicion.y - centroRectangulo2.y)};

		// Si la distancia entre centros es menor que el radio y la mitad de la anchura, han chocado.
		if (distanciaCentros.x <= pala.anchura / 2 + size &&
			distanciaCentros.y <= pala.altura / 2 + size)
		{
			return true; // choca con la pala 1.
		}
		// Si la distancia entre centros es menor que el radio y la mitad de la anchura, han chocado.
		if (distanciaCentros2.x <= pala2.anchura / 2 + size &&
			distanciaCentros2.y <= pala2.altura / 2 + size)
		{
			return true; // choca con la pala 2.
		}
		return false; // no hay choque.
	}
};

int main()
{
	// --- crear una ventana ---
	InitWindow(SIZE_PANTALLA_ANCHO, SIZE_PANTALLA_ALTO, "Pong"); // Crea la ventana de juego
	InitAudioDevice();											 // Inicializar audio
	SetTargetFPS(FPS_MAXIMOS);									 // Limitar el máximo de Frames per second (Fotogramas por segundo)

	// cargar sonidos
	sonido_golpe = LoadSound("resources/sounds/pongblipa4.wav");
	sonido_gol = LoadSound("resources/sounds/objective-complete.wav");

	// --- Inicializar objetos del juego ---
	Pelota pelota = Pelota(
		{SIZE_PANTALLA_ANCHO / 2.0, SIZE_PANTALLA_ALTO / 2.0},
		{1.0, 2.0});

	Pala palaJugador1 = Pala(5, {SIZE_PANTALLA_ANCHO - ANCHURA_PALA, SIZE_PANTALLA_ALTO / 2 - ALTURA_PALA / 2},
							 ANCHURA_PALA, ALTURA_PALA);

	Pala palaJugador2 = Pala(5, {0, SIZE_PANTALLA_ALTO / 2 - ALTURA_PALA / 2},
							 ANCHURA_PALA, ALTURA_PALA);

	// bucle principal del juego
	while (!WindowShouldClose())
	{
		// --- COMPROBAR imputs "entradas" del jugador. Pulsar teclas... ---
		if (IsKeyPressed(KEY_ENTER))
		{
			pausa = !pausa;
		}
		// Jugador 1
		if (IsKeyDown(KEY_UP))
		{
			palaJugador1.actualizar(-1);
		}
		if (IsKeyDown(KEY_DOWN))
		{
			palaJugador1.actualizar(1);
		}
		// Jugador 2.
		if (IsKeyDown(KEY_W))
		{
			palaJugador2.actualizar(-1);
		}

		if (IsKeyDown(KEY_S))
		{
			palaJugador2.actualizar(1);
		}

		// --- ACTUALIZAR objetos ---
		pelota.actualizar(palaJugador1, palaJugador2);

		// --- PINTAR en pantalla ---
		BeginDrawing();			// Función de la librería que se pone antes de Dibujar en pantalla.
		ClearBackground(BLACK); // Pintar de un color (NEGRO) el fondo. Esto borra lo que ya está dibujado anteriormente.

		pelota.draw();
		palaJugador1.draw();
		palaJugador2.draw();

		// Mostrar marcadores
		const int fontSize = 80;
		DrawText(TextFormat("%i", puntos_j1), GetScreenWidth() * 1 / 3, 10, fontSize, WHITE);
		DrawText(TextFormat("%i", puntos_j2), GetScreenWidth() * 2 / 3, 10, fontSize, WHITE);

		EndDrawing(); // Función de la librería que libera el proceso de PINTAR.
	}

	// Liberar recursos que hemos cargado al principio, como sonidos, imagenes.
	UnloadSound(sonido_gol);

	// Cerrar los recursos que hemos abierto, como audio y ventana.
	CloseAudioDevice();
	CloseWindow();
}
