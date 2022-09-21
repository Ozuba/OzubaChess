#Objetos a compilar
OBJS = ./tests/main.cpp ./src/ChessBoard.cpp ./src/SDL_utils.cpp ./src/Piece.cpp
#Librerias dinamicas para el linker
LIBS = -lSDL2 -lSDL2_image
#flags del compilador
FLAGS = -w -o2 
#Flags de debug

#Nombre del ejecutable
NAME = chess

debug:FLAGS += -DDEBUG -g
debug:all


all:
	g++ $(OBJS) $(FLAGS) $(LIBS) -o $(NAME)
