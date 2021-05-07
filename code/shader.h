#ifndef SHADER_H
#define SHADER_H

struct Shader
{
    unsigned int id;
};

void LoadShader(Shader* shader,
        const char* vertex_filepath,
        const char* fragment_path);

void UseShader(Shader* shader);

#endif
