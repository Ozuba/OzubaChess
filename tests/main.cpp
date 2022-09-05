#include "../src/ChessBoard.h"

int main(int argc,char** args){

ChessBoard board(400,400);
board.loadSpritesheet("assest/Pieces.png");
board.newGame();

if(argc > 1){

board.loadFen(args[1]);
} 

while(board.loop());
cout << board.getFen();

}