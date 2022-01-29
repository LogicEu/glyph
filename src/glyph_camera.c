#include <glyph.h>

#define HALF_PI 1.57079632679
#define DEPTH_MAX 10000.0f
#define DEPTH_MIN 0.01f

Cam3D cam3D_new(vec3 position, float fov)
{
    int width, height;
    glee_window_get_size(&width, &height);

    Cam3D cam;
    cam.position = position;
    cam.projection = mat4_perspective_RH(deg_to_rad(fov), (float)width / (float)height, DEPTH_MIN, DEPTH_MAX);
    return cam;
}

void cam3D_update(Cam3D* cam, vec2 dir)
{
    cam->direction = vec3_new(cos(dir.y) * sin(dir.x), sin(dir.y), cos(dir.y) * cos(dir.x));
    cam->right = vec3_new(sin(dir.x - HALF_PI), 0.0f, cos(dir.x - HALF_PI));
    cam->up = vec3_cross(cam->right, cam->direction);
    cam->view = mat4_look_at_RH(cam->position, vec3_add(cam->position, cam->direction), cam->up);
}

void cam3D_update_smooth(Cam3D* cam, vec2 dir)
{
    cam->direction = vec3_lerp(cam->direction, vec3_new(cos(dir.y) * sin(dir.x), sin(dir.y), cos(dir.y) * cos(dir.x)), 0.1);
    cam->right = vec3_new(sin(dir.x - HALF_PI), 0.0f, cos(dir.x - HALF_PI));
    cam->up = vec3_cross(cam->right, cam->direction);
    cam->view = mat4_look_at_RH(cam->position, vec3_add(cam->position, cam->direction), cam->up);
}
