#include "line.h"
#include <glad/glad.h>

Line GenLine(Vec3 a, Vec3 b, Vec3 color, Shader* shader)
{
    Line newLine;
    newLine.vertices[0]  = a.x; 
    newLine.vertices[1]  = a.y;
    newLine.vertices[2]  = a.z;
    newLine.vertices[3]  = color.x; 
    newLine.vertices[4]  = color.y;
    newLine.vertices[5]  = color.z;
    newLine.vertices[6]  = b.x; 
    newLine.vertices[7]  = b.y;
    newLine.vertices[8]  = b.z;
    newLine.vertices[9]  = color.x; 
    newLine.vertices[10] = color.y;
    newLine.vertices[11] = color.z;
    newLine.shader = *shader;

    glGenVertexArrays(1, &newLine.vao);
    glBindVertexArray(newLine.vao);
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(newLine.vertices), newLine.vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    return newLine;
}

void DrawLine(Line* line)
{
    UseShader(&line->shader);
    glBindVertexArray(line->vao);
    Matrix model = get_identity_matrix();
    SetShaderMatrix(model, line->shader.worldMatLoc);
    glDrawArrays(GL_LINES, 0, 2); 
}
