#ifndef SDL_UTILS_H
#define SDL_UTILS_H
//Aqui se incluiran funciones para sdl para simplificar el resto del programa en favor de la legibilidad
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
using namespace std;




SDL_Texture* loadTexture(std::string path,SDL_Renderer* renderer);
bool loadMedia();




#endif