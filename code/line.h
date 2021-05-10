#ifndef LINE_H
#define LINE_H

#include "shader.h"
#include "math.h"

struct Line
{
    unsigned int vao;
    float vertices[12];
    Shader shader;
};

Line GenLine(Vec3 a, Vec3 b, Vec3 color, Shader* shader);
void DrawLine(Line* line);



#endif
