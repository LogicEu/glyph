#include <glyph.h>
#include <stdlib.h>
#include <stdio.h>

static inline void glyph_update(float delta_time)
{
    glyph_scene_render();
}

static inline void glyph_run()
{
    float time = glee_time_get();
    while (glee_window_is_open()) {
        glee_screen_clear();
        if (glee_key_pressed(GLFW_KEY_ESCAPE)) break;
        glyph_update(glee_time_delta(&time));
        glee_screen_refresh();
    }
}

static inline void glyph_init()
{
    glee_init();
    glee_window_create("glyph", 800.0, 600.0, 0, 1);

    glyph_renderer_init();
    glyph_scene_init();
}

static inline void glyph_deinit()
{
    glyph_scene_deinit();
    glee_deinit(); 
}

void glyph_quit()
{
    glyph_deinit();
    exit(EXIT_SUCCESS);
}

int main(void)
{
    glyph_init();
    glyph_run();
    glyph_deinit();
    return EXIT_SUCCESS;
}