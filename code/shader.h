#ifndef SHADER_H
#define SHADER_H

struct Shader
{
    unsigned int id;
};

char* ReadFile(char* file_path);
void LoadShader(Shader* shader,
        const char* vertex_filepath,
        const char* fragment_path);
void UseShader(Shader* shader);

#endif
