# OzubaChess
![Screenshot](https://github.com/Ozuba/OzubaChess/blob/master/Screnshot.png)

Ozuba Chess is a simple Chess Gui written in c++ using the SDL2 Graphics library,
it currently supports the basic part of the Chess Ruleset:

 * Only Legal Moves
 * Turn Based Play
 * Ilegal King-Exposing Moves
          
# To be implemented: #

 - [ ] Support for FEN notation loading of game
 - [x] FEN output
 - [ ] Support for Game ending screen
 - [ ] Tie detection
 - [ ] Animations
 - [ ] Click Based piece movement
 - [x] Hints of available moves on the board;
 - [ ] Add sounds
 - [ ] En Passant
 - [ ] Castling

## To Test ##

Installed required libs with your prefered package manager:
## For Debian Based Systems: ##
```bash
sudo apt-get install libsdl2-dev libsdl2-image-1.2
```
Run:

```bash
make all
./chess
```
This will compile the example file.

 # Sources #
 Icons: https://commons.wikimedia.org/wiki/File:Chess_Pieces_Sprite.svg
 


