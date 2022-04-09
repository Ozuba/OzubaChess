#include "SDL_utils.h"


SDL_Texture* loadTexture(std::string path,SDL_Renderer* renderer){
    SDL_Texture *texture;
   
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
     if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        texture = SDL_CreateTextureFromSurface(renderer,loadedSurface);
     if( texture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
            SDL_FreeSurface(loadedSurface);

    }
    
    return texture;

}
