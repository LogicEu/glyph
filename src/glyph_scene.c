#include <glyph.h>

static Model3D model;
static Cam3D cam;

void glyph_scene_render()
{
    static vec3 pos = {0.0, 0.0, 0.0};
    static vec2 mouse;
    
    glee_mouse_pos_3d(&mouse.x, &mouse.y);
    vec2_scale(&mouse, 0.01);
    
    cam = cam3D_new(cam.position, 45.0);
    cam.direction = vec3_sub(pos, cam.position);
    cam.right = vec3_new(1.0, 0.0, 0.0);
    cam.up = vec3_cross(cam.right, cam.direction);
    cam.view = mat4_look_at_RH(cam.position, cam.direction, cam.up);
    
    glyph_render_camera(&cam);
    glyph_render_model(&model, pos);
}

void glyph_scene_deinit()
{
    model3D_free(&model);
}

void glyph_scene_init()
{
    uint8_t color[] = {255, 100, 100, 255};
    model = model3D_new(vmesh_shape_cube(1), texture_color(1, 1, color));
    cam = cam3D_new(vec3_new(-10.0, 10.0, -10.0), 45.0);
}