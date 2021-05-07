#include "camera.h"
#include <math.h>

void InitializeCamera(Camera* camera)
{
    camera->position = {0.0f, 0.0f, -10.0f}; 
    camera->target   = {0.0f, 0.0f,  1.0f};
    camera->up       = {0.0f, 1.0f,  0.0f};
    camera->right    = vec3_cross(camera->up, camera->target);
    camera->viewMat  = get_view_matrix(camera->position, camera->position + camera->target, camera->up);
    camera->pitch    = 0.0f;
    camera->yaw      = 90.0f; 
}

void UpdateCamera(Camera* camera, Input* input, float deltaTime)
{ 
    camera->pitch -= (input->mouseIncY * 10.0f) * deltaTime;
    camera->yaw   -= (input->mouseIncX * 10.0f) * deltaTime;

    if(camera->pitch  >  89.0f)
        camera->pitch =  89.0f;
    if(camera->pitch  < -89.0f)
        camera->pitch = -89.0f;

    camera->target.x = cosf(to_radiant(camera->yaw)) * cosf(to_radiant(camera->pitch));
    camera->target.y = sinf(to_radiant(camera->pitch));
    camera->target.z = sinf(to_radiant(camera->yaw)) * cosf(to_radiant(camera->pitch));
    camera->right = vec3_cross(camera->target, camera->up);

    if(GetKeyDown(input, 'W'))
    {
        camera->position +=  camera->target * deltaTime;
    }

    if(GetKeyDown(input, 'S'))
    {
        camera->position -= camera->target * deltaTime;
    }

    if(GetKeyDown(input, 'A'))
    {
        camera->position += camera->right * deltaTime;
    }

    if(GetKeyDown(input, 'D'))
    {
        camera->position -= camera->right * deltaTime;
    }

    camera->viewMat = get_view_matrix(camera->position, camera->position + camera->target, camera->up);

}

