#ifndef CAMERA_H
#define CAMERA_H

#include <stdint.h>
#include "math.h"
#include "input.h"

struct Camera
{
    Vec3 position;
    Vec3 right;
    Vec3 target;
    Vec3 up;
    Matrix viewMat;
    float pitch;
    float yaw;
};

void InitializeCamera(Camera* camera);
void UpdateCamera(Camera* camera, Input* input, float deltaTime);

#endif
