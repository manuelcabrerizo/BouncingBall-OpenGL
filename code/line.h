#ifndef LINE_H
#define LINE_H

#include "shader.h"
#include "math.h"

struct Line
{
    unsigned int vao;
    float vertices[12];
    Vec3 a;
    Vec3 b;
    Shader shader;
};

struct Plane
{
    unsigned int vao;
    float vertices[24];
    int indices[6];
    Shader shader;
    Vec3 a;
    Vec3 u;
    Vec3 v;
};

Line GenLine(Vec3 a, Vec3 b, Vec3 color, Shader* shader);
void DrawLine(Line* line, Matrix model);
Plane GenPlane(Vec3 a, Vec3 b, Vec3 c, Vec3 color, Shader* shader);
void DrawPlane(Plane* plane, Matrix model);



#endif
