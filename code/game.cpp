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
    Matrix proj = get_projection_perspective_matrix(to_radiant(90), WNDWIDTH/WNDHEIGHT, 0.1f, 100.0f); 
    UseShader(&game->main_shader);
    SetShaderMatrix(proj, game->main_shader.projMatLoc);
    UseShader(&game->mesh_shader);
    SetShaderMatrix(proj, game->mesh_shader.projMatLoc);
    InitializeCamera(&game->camera); 
    game->xAxis = GenLine({-100.0f, 0.0f, 0.0f}, {100.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, &game->main_shader);  
    game->yAxis = GenLine({0.0f, -100.0f, 0.0f}, {0.0f, 100.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, &game->main_shader);
    game->zAxis = GenLine({0.0f, 0.0f, -100.0f}, {0.0f, 0.0f, 100.0f}, {0.0f, 0.0f, 1.0f}, &game->main_shader);
    game->testPlane = GenPlane({0.0f, 0.0f, 0.0f}, {4.0f, 0.0f, 0.0f}, {0.0f, 6.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, &game->main_shader);
    game->ballDirection = GenLine({-2.0f, 2.0f, 4.0f}, {5.0f, 8.0f, -4.0f}, {1.0f, 0.0f, 1.0f}, &game->main_shader);
    game->intersectionLine = GenLine({-4.0f, 2.0f,-8.0f}, { 4.0f, 2.0f, 8.0f}, { 1.0f, 0.7f, 0.3f}, &game->main_shader);
    GenerateTerrain(&game->terrain, -10, -10, 20, 20, 1, "./data/terrain.bmp");
    LoadOBJFileIndex(&game->ball, "./data/bullet.obj", "./data/bullet.bmp");
 
    int AbsNumber = Abs(-5565);

    char buffer[100];
    sprintf(buffer,"Abs: %d\n", AbsNumber);
    OutputDebugString(buffer);

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
    DrawLine(&game->xAxis, get_identity_matrix());
    DrawLine(&game->yAxis, get_identity_matrix());
    DrawLine(&game->zAxis, get_identity_matrix());
    DrawLine(&game->ballDirection, get_identity_matrix());
    DrawLine(&game->intersectionLine, get_identity_matrix());
    DrawPlane(&game->testPlane, get_translation_matrix(game->testPlane.a));
    
    UseShader(&game->mesh_shader);
    glBindTexture(GL_TEXTURE_2D, game->terrain.texId);
    glBindVertexArray(game->terrain.vao);
    Matrix model = get_identity_matrix();
    SetShaderMatrix(model, game->mesh_shader.worldMatLoc);
    glDrawElements(GL_TRIANGLES, game->terrain.numIndex, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, game->ball.texId);
    glBindVertexArray(game->ball.vao);
    
    float planeIntersection = LinePlaneIntersectsAt(&game->ballDirection, &game->testPlane);
    static float ballPosition = 0.0f; 
    if(planeIntersection >= 0.0f && planeIntersection <= 1.0f)
    {
        if(ballPosition <= planeIntersection)
        {
            game->ballTrans = Lerp(game->ballDirection.a, game->ballDirection.b, ballPosition);
        }
        else
        {
            game->ballTrans += Reflect(&game->ballDirection, vec3_cross(game->testPlane.u, game->testPlane.v)) * deltaTime; 
        }
        model = get_scale_matrix({0.1f, 0.1f, 0.1f}) * get_translation_matrix(game->ballTrans);
        SetShaderMatrix(model, game->mesh_shader.worldMatLoc);
        glDrawElements(GL_TRIANGLES, game->ball.numIndex * 3, GL_UNSIGNED_INT, 0);
    }
    ballPosition += 0.1f * deltaTime;

    float planeIntersection2 = LinePlaneIntersectsAt(&game->intersectionLine, &game->testPlane);
    if(planeIntersection2 >= 0.0f && planeIntersection2 <= 1.0f)
    {
        model = get_scale_matrix({0.1f, 0.1f, 0.1f}) *
                get_translation_matrix(Lerp(game->intersectionLine.a, game->intersectionLine.b,
                planeIntersection2));
        SetShaderMatrix(model, game->mesh_shader.worldMatLoc);
        glDrawElements(GL_TRIANGLES, game->ball.numIndex * 3, GL_UNSIGNED_INT, 0); 
    }

    float intersectionT = LineIntersectsAt(&game->ballDirection, &game->intersectionLine);
    float intersectionS = LineIntersectsAt(&game->intersectionLine, &game->ballDirection);
    if(intersectionT >= 0.0f && intersectionT <= 1.0f && intersectionS >= 0.0f && intersectionS <= 1.0f)
    {
        model = get_scale_matrix({0.1f, 0.1f, 0.1f}) *
                get_translation_matrix(Lerp(game->ballDirection.a, game->ballDirection.b,
                intersectionT));
        SetShaderMatrix(model, game->mesh_shader.worldMatLoc);
        glDrawElements(GL_TRIANGLES, game->ball.numIndex * 3, GL_UNSIGNED_INT, 0);
    }
}
