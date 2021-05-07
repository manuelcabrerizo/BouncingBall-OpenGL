#include <stdio.h>
#include <windows.h>
#include <glad/glad.h>


#include "game.h"
#include "math.h"
#include <math.h>


void GameInit(MainGame* game)
{
    float triangle_vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.5f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.5f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.5f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.5f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.5f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.5f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.5f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.5f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.5f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.5f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.5f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.5f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.5f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.5f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.5f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.5f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.5f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.5f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.5f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.5f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.5f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.5f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.5f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.5f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.5f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.5f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.5f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.5f
    };
    game->input.mouseDefaultPosX = WNDWIDTH  / 2;
    game->input.mouseDefaultPosY = WNDHEIGHT / 2;
    InitializeCamera(&game->camera);
    
    LoadShader(&game->main_shader,
            "./code/vertexShader.vert",
            "./code/fragmentShader.frag");
    LoadShader(&game->mesh_shader,
            "./code/sphereVertexShader.vert",
            "./code/sphereFragmentShader.frag"); 

    glGenVertexArrays(1, &game->triangle_vao);
    glBindVertexArray(game->triangle_vao);

    unsigned int VBO;  // vertex buffer obj...
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(triangle_vertices),
            triangle_vertices,
            GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Matrix proj = get_projection_perspective_matrix(to_radiant(45), WNDWIDTH/WNDHEIGHT, 0.1f, 100.0f); 

    UseShader(&game->main_shader);
    int projMatLoc = glGetUniformLocation(game->main_shader.id, "Proj");
    glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, proj.m[0]);

    UseShader(&game->mesh_shader);
    int SphProjMatLoc = glGetUniformLocation(game->mesh_shader.id, "Proj");
    glUniformMatrix4fv(SphProjMatLoc, 1, GL_FALSE, proj.m[0]);

    LoadOBJFileIndex(&game->sphere, "./data/tree.obj", "./data/tree.bmp");
}

void GameUnpdateAndRender(MainGame* game, float deltaTime)
{
    UpdateCamera(&game->camera, &game->input, deltaTime);

    UseShader(&game->mesh_shader);
    int SphViewMatLoc = glGetUniformLocation(game->mesh_shader.id, "View");
    glUniformMatrix4fv(SphViewMatLoc, 1, GL_FALSE, game->camera.viewMat.m[0]);

    UseShader(&game->main_shader);
    int viewMatLoc = glGetUniformLocation(game->main_shader.id, "View");
    glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, game->camera.viewMat.m[0]);
    

    UseShader(&game->main_shader);
    glBindVertexArray(game->triangle_vao);
    Matrix model = get_identity_matrix();
    int worldMatLoc = glGetUniformLocation(game->main_shader.id, "World"); 
    static float time = 0.0f;
    float t = absf(sinf(time));
    Vec3 startPos  = {2.0f,  0.0f, 0.0f};
    Vec3 endPos    = {-2.0f, 0.0f, 0.0f};
    Vec3 actualPos = Lerp(startPos, endPos, t);

    if(GetMouseButtonPress(&game->input, LEFTBUTTON))  // see if the left arroy is DOWN...
        time += 1 * deltaTime;

    // Draw the start position
    model = get_translation_matrix(startPos);
    glUniformMatrix4fv(worldMatLoc, 1, GL_FALSE, model.m[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // Draw the end position
    model = get_translation_matrix(endPos);
    glUniformMatrix4fv(worldMatLoc, 1, GL_FALSE, model.m[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    UseShader(&game->mesh_shader);
    int SphWorldMatLoc = glGetUniformLocation(game->mesh_shader.id, "World");
    glBindTexture(GL_TEXTURE_2D, game->sphere.texId);
    glBindVertexArray(game->sphere.vao);
    model = get_translation_matrix(actualPos);
    glUniformMatrix4fv(SphWorldMatLoc, 1, GL_FALSE, model.m[0]);
    glDrawElements(GL_TRIANGLES, game->sphere.numIndex * 3, GL_UNSIGNED_INT, 0);
}
