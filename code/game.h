#ifndef GAME_H
#define GAME_H

#include "shader.h"

struct MainGame
{
    Shader main_shader; 
    unsigned int triangle_vao;
};

void GameInit(MainGame* game);
void GameUnpdateAndRender(MainGame* game, float deltaTime);

#endif
