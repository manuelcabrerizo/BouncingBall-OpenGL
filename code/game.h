#ifndef GAME_H
#define GAME_H

#include "shader.h"
#include "utility.h"
#include "input.h"
#include "camera.h"
#include "line.h"

#define global_variable static
#define WNDWIDTH 800
#define WNDHEIGHT 600
#define LEFTBUTTON 0
#define RIGHTBUTTON 1

struct MainGame
{
    Shader main_shader;
    Shader mesh_shader; 
    Input input;
    Camera camera;
    Terrain terrain;
    Line xAxis;
    Line yAxis;
    Line zAxis;
    Plane testPlane;
    Mesh ball;
    Line ballDirection;
    Line intersectionLine;
};

void GameInit(MainGame* game);
void GameUnpdateAndRender(MainGame* game, float deltaTime);

#endif
