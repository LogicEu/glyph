#include <glyph.h>

static framebuffer_t framebuffer;
static mat4 matId;

static int window_width, window_height;
static unsigned int quadID;
static unsigned int drawMode;
static unsigned int lightshader, skyboxshader, framebuffer_shader;

static inline void glyph_shaders_init()
{
    lightshader = glee_shader_load("assets/shaders/lightvert.glsl", "assets/shaders/lightfrag.glsl");
    glUniform3f(glGetUniformLocation(lightshader, "global_light.direction"), -0.5f, -0.5f, 0.0f);
    glUniform3f(glGetUniformLocation(lightshader, "global_light.ambient"), 0.7f, 0.7f, 0.7f);
    glUniform3f(glGetUniformLocation(lightshader, "global_light.diffuse"), 0.8f, 0.8f, 0.8f);
    glUniform3f(glGetUniformLocation(lightshader, "global_light.specular"), 0.0f, 0.0f, 0.0f);

    glUniform3f(glGetUniformLocation(lightshader, "point_light.position"), 4.0f, 8.0f, 4.0f); 
    glUniform3f(glGetUniformLocation(lightshader, "point_light.ambient"), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(lightshader, "point_light.diffuse"), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(lightshader, "point_light.specular"), 0.0f, 0.0f, 0.0f);

    glUniform1f(glGetUniformLocation(lightshader, "point_light.constant"), .01f);
    glUniform1f(glGetUniformLocation(lightshader, "point_light.linear"), 0.01f);
    glUniform1f(glGetUniformLocation(lightshader, "point_light.quadratic"), 0.01f);
    glUniform1f(glGetUniformLocation(lightshader, "shininess"), 32.0f);

    //skyboxshader = glee_shader_load("assets/shaders/skyboxvert.frag", "assets/shaders/skyboxfrag.frag");
    //framebuffer_shader = glee_shader_load("assets/shaders/framebufferv.frag", "assets/shaders/framebufferf.frag");
}

void glyph_renderer_init()
{   
    drawMode = GL_TRIANGLES;
    matId = mat4_id();
    framebuffer = framebuffer_new();
    quadID = glee_buffer_quad_create();

    glyph_shaders_init();
    glee_set_3d_mode();
    glee_screen_color(0.5, 0.5, 0.5, 1.0);
}

void glyph_render_camera(const Cam3D* restrict cam)
{
    glUseProgram(lightshader);
    glUniformMatrix4fv(glGetUniformLocation(lightshader, "view"), 1, GL_FALSE, &cam->view.data[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(lightshader, "projection"), 1, GL_FALSE, &cam->projection.data[0][0]);
    glUniform3f(glGetUniformLocation(lightshader, "viewPos"), cam->position.x, cam->position.y, cam->position.z);
}

void glyph_render_model(const Model3D* restrict model, vec3 position)
{
    mat4 m = mat4_translate(matId, position);

    glUseProgram(lightshader);
    glBindVertexArray(model->id);
    glBindTexture(GL_TEXTURE_2D, model->texture.id);
    glUniformMatrix4fv(glGetUniformLocation(lightshader, "model"), 1, GL_FALSE, &m.data[0][0]);
    glDrawElements(drawMode, model->mesh->indices->used, GL_UNSIGNED_INT, 0);
}

void glyph_render_skybox(const Cam3D* restrict cam, const skybox_t* restrict skybox)
{
    mat4 m = mat4_mult(cam->projection, mat4_mult(mat4_look_at(vec3_uni(0.0), cam->direction, cam->up), mat4_id()));
    
    glDepthMask(GL_FALSE);
    glUseProgram(skyboxshader);
    glBindVertexArray(skybox->VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->cubemap.id);
    glUniformMatrix4fv(glGetUniformLocation(skyboxshader, "MVP"), 1, GL_FALSE, &m.data[0][0]);
    glDrawArrays(drawMode, 0, 36);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glDepthMask(GL_TRUE);
}

void glyph_renderer_switch()
{
    if (drawMode == GL_TRIANGLES) drawMode = GL_LINES;
    else drawMode = GL_TRIANGLES;
}

void glyph_renderer_update()
{
    int w, h;
    glee_window_get_size(&w, &h);
    if (window_width == w && window_height == h) return;

    window_width = w;
    window_height = h;
}

void glyph_render_framebuffer_start()
{
    framebuffer_bind(framebuffer.id);
    glee_set_3d_mode();
    glee_screen_clear();
}

void glyph_render_framebuffer_end()
{
    framebuffer_bind(0);

    glee_set_2d_mode();
    glUseProgram(framebuffer_shader);
    glBindVertexArray(quadID);
    glBindTexture(GL_TEXTURE_2D, framebuffer.texture.id);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}