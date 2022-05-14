#include "Piece.h"

Piece::Piece(Player_t _player, Figure_t _figure, int _a, int _b)
{
    color = _player;
    figure = _figure;
    a = _a;
    b = _b;
    x = 0;
    y = 0;
}

