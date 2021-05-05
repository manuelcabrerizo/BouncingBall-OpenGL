#include <glad/glad.h>
#include "game.h"

void GameInit(MainGame* game)
{
    float triangle_vertices[] = {
        // positions         // colors
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 1.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 1.0f,   // bottom left
        0.0f,  0.5f, 0.0f,  0.5f, 0.5f, 1.0f    // top 
    };
    
    LoadShader(&game->main_shader,
            "./code/vertexShader.vert",
            "./code/fragmentShader.frag");

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

    UseShader(&game->main_shader);

}

void GameUnpdateAndRender(MainGame* game, float deltaTime)
{
    glBindVertexArray(game->triangle_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
