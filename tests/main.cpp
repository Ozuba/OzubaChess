#include "../src/ChessBoard.h"

int main(int argc,char** args){

ChessBoard board1(400,400);


board1.loadSpritesheet("assest/Pieces.png");
board1.newGame();
board1.loadFen("R1B1KB1R/P1PQP1P1/2N2N2/1P1P1P1P/1p1p1p1p/2n2n2/p1pqp1p1/r1b1kb1r");
while(board1.loop());
cout << board1.getFen();

}