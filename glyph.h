#ifndef GLYPH_ENGINE_H
#define GLYPH_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

/******************
 *     GLYPH      *
 ******************
 glyph game engine
 *****************/

#include <glui.h>
#include <mass.h>

typedef struct Cam3D {
    vec3 position;
    vec3 direction;
    vec3 right;
    vec3 up;
    mat4 projection;
    mat4 view;
} Cam3D;

typedef struct Model3D {
    unsigned int id;
    unsigned int VAO;
    vmesh_t* mesh;
    texture_t texture;
} Model3D;

// ...

void glyph_quit();

void glyph_renderer_init();
void glyph_render_camera(const Cam3D* camera);
void glyph_render_model(const Model3D* model, vec3 position);
void glyph_render_skybox(const Cam3D* restrict cam, const skybox_t* skybox);
void glyph_renderer_switch();
void glyph_render_framebuffer_start();
void glyph_render_framebuffer_end();

void glyph_scene_render();
void glyph_scene_init();
void glyph_scene_deinit();

Cam3D cam3D_new(vec3 position, float fov);
void cam3D_update(Cam3D* cam, vec2 dir);

void model3D_rebind(const Model3D* restrict model);
Model3D model3D_new(vmesh_t* mesh, const texture_t texture);
Model3D model3D_perlin(const unsigned int width, const unsigned int height, const bmp_t* restrict bmp, const vec2 off);
void model3D_free(Model3D* model);

#ifdef __cplusplus
}
#endif
#endif
