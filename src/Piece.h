#ifndef PIECE_H
#define PIECE_H

#include <SDL2/SDL.h>
#include "SDL_utils.h"

// Utilizamos notacion algebraica y por tanto se empieza a contar de abajo a la derecha

// Tamaño iconos
#define ICON_H 45
#define ICON_W 45

typedef enum
{
    WHITE,
    BLACK,
} Player_t; // Player property

typedef enum
{
    NONE,
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING
} Figure_t; // Figure property



class Piece
{

private:
public:
    // Constructores
    Piece(Player_t _player, Figure_t figure, int _a = 0, int _b = 0); // Constructor
    // ~Piece();

    bool checkMov();//Implementar algo para pasar direcciones
    // Propiedades
    int x, y; // Posición en el tablero en pixeles
    // Posicion en el juego en casillas
    int a, b;

    bool neverMoved = true;//For pawn double move

    Player_t color;
    Figure_t figure;
};

#endif