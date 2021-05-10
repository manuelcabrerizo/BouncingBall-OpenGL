#include <stdio.h>
#include <windows.h>
#include <glad/glad.h>


#include "game.h"
#include "math.h"
#include <math.h>

void GameInit(MainGame* game)
{    
    LoadShader(&game->main_shader,
            "./code/vertexShader.vert",
            "./code/fragmentShader.frag");
    LoadShader(&game->mesh_shader,
            "./code/sphereVertexShader.vert",
            "./code/sphereFragmentShader.frag"); 

    Matrix proj = get_projection_perspective_matrix(to_radiant(45), WNDWIDTH/WNDHEIGHT, 0.1f, 100.0f); 
    UseShader(&game->main_shader);
    SetShaderMatrix(proj, game->main_shader.projMatLoc);
    UseShader(&game->mesh_shader);
    SetShaderMatrix(proj, game->mesh_shader.projMatLoc);

    InitializeCamera(&game->camera);
   
    game->xAxis = GenLine({-100.0f, 0.0f, 0.0f},
                          { 100.0f, 0.0f, 0.0f},
                          { 1.0f, 0.0f, 0.0f},
                          &game->main_shader); 
    game->yAxis = GenLine({0.0f, -100.0f, 0.0f},
                          { 0.0f, 100.0f, 0.0f},
                          { 0.0f, 1.0f, 0.0f},
                          &game->main_shader);
    game->zAxis = GenLine({0.0f, 0.0f, -100.0f},
                          { 0.0f, 0.0f, 100.0f},
                          { 0.0f, 0.0f, 1.0f},
                          &game->main_shader);

    GenerateTerrain(&game->terrain, -10, -10, 20, 20, 1, "./data/grass.bmp");
}

void GameUnpdateAndRender(MainGame* game, float deltaTime)
{
    // Update...
    UpdateCamera(&game->camera, &game->input, deltaTime);
    UseShader(&game->main_shader);
    SetShaderMatrix(game->camera.viewMat, game->main_shader.viewMatLoc);
    UseShader(&game->mesh_shader);
    SetShaderMatrix(game->camera.viewMat, game->mesh_shader.viewMatLoc);

    // Render...
    DrawLine(&game->xAxis);
    DrawLine(&game->yAxis);
    DrawLine(&game->zAxis);

    UseShader(&game->mesh_shader);
    glBindTexture(GL_TEXTURE_2D, game->terrain.texId);
    glBindVertexArray(game->terrain.vao);
    Matrix model = get_identity_matrix();
    SetShaderMatrix(model, game->mesh_shader.worldMatLoc);
    glDrawElements(GL_TRIANGLES, game->terrain.numIndex, GL_UNSIGNED_INT, 0);

}
