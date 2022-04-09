#include "../src/ChessBoard.h"

int main(int argc,char** args){

ChessBoard board1(400,400);


board1.loadSpritesheet("assest/Pieces.png");
board1.newGame();
while(board1.loop());

}