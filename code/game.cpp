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
    // Generate the two planes...
    game->planes[0] = GenPlane({0.0f, 1.0f, 0.0f}, {4.0f, 1.0f, 0.0f}, {0.0f, 6.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, &game->main_shader);
    game->planes[1] = GenPlane({0.0f, 1.0f, 4.0f}, {4.0f, 1.0f, 4.0f}, {0.0f, 6.0f, 4.0f}, {0.0f, 1.0f, 0.0f}, &game->main_shader);
    game->planes[2] = GenPlane({0.0f, 1.0f, 0.0f}, {4.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 4.0f}, {1.0f, 0.0f, 0.0f}, &game->main_shader);
    game->planes[3] = GenPlane({0.0f, 6.0f, 0.0f}, {4.0f, 6.0f, 0.0f}, {0.0f, 6.0f, 4.0f}, {0.0f, 0.0f, 1.0f}, &game->main_shader);
    game->planes[4] = GenPlane({0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 4.0f}, {0.0f, 6.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, &game->main_shader);
    game->planes[5] = GenPlane({4.0f, 1.0f, 0.0f}, {4.0f, 1.0f, 4.0f}, {4.0f, 6.0f, 0.0f}, {0.3f, 0.5f, 1.0f}, &game->main_shader);

    game->ballDirection = GenLine({-2.0f, 2.0f, 4.0f}, {5.0f, 8.0f, -4.0f}, {1.0f, 0.0f, 1.0f}, &game->main_shader);
    game->intersectionLine = GenLine({-4.0f, 2.0f,-8.0f}, { 4.0f, 2.0f, 8.0f}, { 1.0f, 0.7f, 0.3f}, &game->main_shader);
    GenerateTerrain(&game->terrain, -10, -10, 20, 20, 1, "./data/terrain.bmp");
    LoadOBJFileIndex(&game->ball, "./data/tree.obj", "./data/tree.bmp");
    
    for(int i = 0; i < 200; i++)
    {
        game->projectile[i].start    = {0.0f, 0.0f, 0.0f};
        game->projectile[i].position = {0.0f, 0.0f, 0.0f};
        game->projectile[i].end      = {0.0f, 0.0f, 0.0f};
        game->projectile[i].speed    = 0.3f;
        game->projectile[i].angle    = 0.0f;
    }

}

void ShootProjectile(Projectile* projectile, Plane plane[], int numberPlanes, Vec3 start, Vec3 end)
{
    projectile->start = start;
    projectile->end   = end;
    projectile->distance = 0.0f;

    float intersections[6];
    for(int i = 0; i < numberPlanes; i++)         
    {
        intersections[i] = Vec3PlaneIntersectsAt(projectile->start, projectile->end, &plane[i]);
        if(intersections[i] < 0)
        {
            intersections[i] = 100000;
        }
    }
   
    for(int i = 0; i < numberPlanes; i++)
    {
        if(i == 0)
        {
            projectile->planeIntersection = intersections[i];
            projectile->index = 0;
        }
        else
        {
            if(projectile->planeIntersection > intersections[i])
            {
                projectile->planeIntersection = intersections[i];
                projectile->index = i;
            }
        }   
    }
}

Matrix UpdateProjectile(Projectile* projectile, Plane plane[], int numberPlanes, float deltaTime)
{
    Matrix model = get_identity_matrix();
    if(projectile->distance <= 1.0f)
    {
        if(projectile->planeIntersection >= 0.0f && projectile->planeIntersection <= 1.0f)
        {
            if(projectile->distance <= projectile->planeIntersection)
            {   
                projectile->position = Lerp(projectile->start, projectile->end, projectile->distance); 
            } 
            else
            {
                Vec3 newTraget = normaliza_vec3(Vec3Reflect(projectile->start,
                                                projectile->end,
                                                vec3_cross(plane[projectile->index].u, plane[projectile->index].v)));
                ShootProjectile(projectile, plane, numberPlanes, projectile->position, projectile->position + (newTraget * 10));
            }
        }
        else
        {
            projectile->position = Lerp(projectile->start, projectile->end, projectile->distance); 
        }
    }
    
    model = get_translation_matrix(projectile->position);
    projectile->distance += projectile->speed * deltaTime;
    return model;
    
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
    DrawPlane(&game->planes[0], get_translation_matrix(game->planes[0].a));
    DrawPlane(&game->planes[1], get_translation_matrix(game->planes[1].a));
    DrawPlane(&game->planes[2], get_translation_matrix(game->planes[2].a)); 
    DrawPlane(&game->planes[3], get_translation_matrix(game->planes[3].a));
    DrawPlane(&game->planes[4], get_translation_matrix(game->planes[4].a)); 
    DrawPlane(&game->planes[5], get_translation_matrix(game->planes[5].a));


    UseShader(&game->mesh_shader);
    glBindTexture(GL_TEXTURE_2D, game->terrain.texId);
    glBindVertexArray(game->terrain.vao);
    Matrix model = get_identity_matrix();
    SetShaderMatrix(model, game->mesh_shader.worldMatLoc);
    glDrawElements(GL_TRIANGLES, game->terrain.numIndex, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, game->ball.texId);
    glBindVertexArray(game->ball.vao);
    
    float planeIntersection = LinePlaneIntersectsAt(&game->ballDirection, &game->planes[0]);
    static float ballPosition = 0.0f; 
    if(planeIntersection >= 0.0f && planeIntersection <= 1.0f)
    {
        if(ballPosition <= planeIntersection)
        {
            game->ballTrans = Lerp(game->ballDirection.a, game->ballDirection.b, ballPosition);
        }
        else
        {
            game->ballTrans += Reflect(&game->ballDirection, vec3_cross(game->planes[0].u, game->planes[0].v)) * deltaTime; 
        }
        model = get_scale_matrix({0.1f, 0.1f, 0.1f}) * get_translation_matrix(game->ballTrans);
        SetShaderMatrix(model, game->mesh_shader.worldMatLoc);
        glDrawElements(GL_TRIANGLES, game->ball.numIndex * 3, GL_UNSIGNED_INT, 0);
    }
    ballPosition += 0.1f * deltaTime;

    float planeIntersection2 = LinePlaneIntersectsAt(&game->intersectionLine, &game->planes[0]);
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



    // START::PROCCESING::OUR::PROJECTILE::STUFF...
    static bool mouseLButtonPress = false;
    static int actualProjectile   = 0;
    if(GetMouseButtonPress(&game->input, LEFTBUTTON) && mouseLButtonPress == false)
    {
        ShootProjectile(&game->projectile[actualProjectile], game->planes, 6, game->camera.position, game->camera.position + (game->camera.target * 10.0f));
        mouseLButtonPress = true;
        actualProjectile++;
    }
    if(!GetMouseButtonPress(&game->input, LEFTBUTTON) && mouseLButtonPress == true)
    {
        mouseLButtonPress = false;
    }
    actualProjectile %= 200;
    for(int i = 0 ; i < 200; i++)
    {
        game->projectile[i].angle += 10.0f * deltaTime;
        Matrix projectileModel = get_scale_matrix({0.1f, 0.1f, 0.1f}) * get_rotation_y_matrix(game->projectile[i].angle) * UpdateProjectile(&game->projectile[i], game->planes, 6, deltaTime);
        if(game->projectile[i].distance <= 1.0f)
        {
            SetShaderMatrix(projectileModel, game->mesh_shader.worldMatLoc);
            glDrawElements(GL_TRIANGLES, game->ball.numIndex * 3, GL_UNSIGNED_INT, 0);
        }
    }
}
