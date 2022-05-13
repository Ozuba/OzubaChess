/*
Ozuba 2022
SDL chess interface
*/
#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <iostream>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "SDL_utils.h"
#include "Piece.h"

#define CHESSHEIGHT 8
#define CHESSWIDTH 8
#define PIECESIZE 45 // Tamaño de la Pieza de asset(OJO redundante)
#define KINGCHECKMATE

#define FLOATCMPTHRESHOLD 0.00005 // Para comprobar la igualdad de dos operaciones en coma flotante porbado con el caso limite ficha en cada esquina //es posible que haya que aumentarlo si se hace el tablero mas grande

// El objetivo es poder crearlo como una libreria en la que
// se creará un objeto chessboard con el que interactuar a traves de sus metodos publicos
// devolverá los datos al motor a traves de una pool de datos a la que se podrá acceder de manera publica
// el tablero es lo importante por lo que las piezas no tienen coordenandas
// Usar coordenadas cartesianas vs cordenadas de ajedrez + traductor, o enums con las letras

// Cambio de filosofia total, el motor solo evalua el tablero y ofrece la mejor jugada,
// es el tablero quien imposibilita los movimientos prohibidos de esta manera el motor solo recibe una instantanea de el juego en fen y devueve la jugada optima, en FEN
// El modelo de punteros a ficha facilita la creación de ajedrez cuantico, donde la ficha está en 2 lugares al mismo tiempo
typedef enum
{
   WHITEPLAYS,
   BLACKPLAYS,
   IDLE,
} State_t;

typedef struct // Struct para los colores
{
   uint8_t r;
   uint8_t g;
   uint8_t b;
   uint8_t a;

} Color_t;

typedef struct
{
   int a = 0;
   int b = 0;
} Pos_t;

// Si estan negadas son blancas

class ChessBoard
{

public:
   ChessBoard(size_t h = 400, size_t w = 400); // Constructor
   virtual ~ChessBoard();                      // Destructor

   // ChessBoard loop(Se encarga de los eventos) devuelve false al terminar el loop y salir de la aplicacion
   bool loop();

   // Funciones para Interactuar con la Board
   void getMove();
   void newGame();                                      // Genera un Juego de ajedrez clasico
   void move(int srca, int srcb, int desta, int destb); // Mueve una Pieza de una Posición a otra
   void move(Piece *piece, Pos_t pos);                  // Sobrecarga para mover piezas por puntero

   bool isValidMove(Piece *piece, Pos_t pos); // Valida el movimiento

   // Publicas por ahora
   void render(); // Renderiza piezas y tablero

   // Apartado Tema
   void loadSpritesheet(std::string path); // Pone las Piezas,Función color en futuro

   // Apartado FEN

   string getFen();//Carga Partida 
   void loadFen();//Exporta partida

private:
   // Estado del Juego Juegan blancas/negras, esperando al motor, validando movimiento, idle...
   State_t state;
   Piece* selectedPiece = nullptr;//Pieza seleccionada nullptr si no hay ninguna
   // Array de Piezas
   Piece *pieceSet[PIECESETSIZE];
   void killPiece(Piece *p);    // Elimina la pieza del array y la destruye
   void setPiecesPos();         // Asigna posición a cada fichas
   Piece *pieceAt(Pos_t pos);   // devuelve la pieza en la posicíon
   Pos_t tileSelect = {-1, -1}; // Casilla seleccionada
   bool isValidPieceMove(Piece *piece, Pos_t dest);
   Pos_t lookFor(Player_t color, Figure_t figure);
   bool isKingBeingExposed(Piece *piece, Pos_t dest); // Función

   void disablePiece(Piece *piece); // Deshabilita una Pieza en el tablero para hacer checks
   void enablePiece(Piece *piece);
   bool kingCheck = false;

   //  Spritesheet Piezas
   SDL_Texture *spriteSheet;
   // Colores
   Color_t tileColor[2] = {{102, 0, 0, 255}, {248, 231, 187, 255}}; // Chessboard colors

   // Ventana
   SDL_Window *window;
   SDL_Surface *bsurface;
   SDL_Renderer *renderer;
   int height, width; // Tamaño del tablero
   // Funciones de utilidad de ventana

   Pos_t tileAt(int x, int y);

   // Cambiador de estado
   void nextTurn();

   // Eventos de la ventana chessboard
   SDL_Event event;
   void wEventHandler(SDL_Event &event); // Handler de los eventos de ventana, focus/minimizar....
   void clickHandler();
   // RENDER
   void drawBoard();                // Dibuja el tablero
   void drawHints(Piece* piece = nullptr);                //Posible incluir en el drawBoard pero mejor por separado
   void drawPieces();               // Dibuja las pieazas en su lugar
   void renderPiece(Piece *_piece); // Toma Pieza y la renderiza con las dimensiones dadas por el tablero y su posición en el
};

#endif