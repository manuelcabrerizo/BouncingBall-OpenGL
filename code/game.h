#ifndef GAME_H
#define GAME_H

#include "shader.h"
#include "utility.h"
#include "input.h"
#include "camera.h"

#define global_variable static
#define WNDWIDTH 800
#define WNDHEIGHT 600
#define LEFTBUTTON 0
#define RIGHTBUTTON 1

struct Terrain
{
    unsigned int vao;
    VertexBuffer* vertexBuffer;
    int*  indexBuffer;
    int numIndex;
};

struct MainGame
{
    Shader main_shader;
    Shader mesh_shader; 
    unsigned int triangle_vao;
    Mesh sphere;
    Mesh SuperPene;
    Input input;
    Camera camera;
    Terrain terrain;
};

void GameInit(MainGame* game);
void GameUnpdateAndRender(MainGame* game, float deltaTime);

#endif
