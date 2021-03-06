#include "window.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "image.h"
#include "font.h"
#include "../base/base.h"
#include "../base/time.inl"
#include "../base/config.h"
#include <SDL2/SDL_events.h>

config_t* c_quit;

config_t* c_window_width;
config_t* c_window_height;
config_t* c_window_fullscreen;
config_t* c_window_borderless;

config_t* c_render_vsync;
config_t* c_render_scale;

config_t* c_camera_zoom;
config_t* c_camera_speed;

config_t* c_audio_master_volume;
config_t* c_audio_music_volume;
config_t* c_audio_sfx_volume;
config_t* c_audio_voice_volume;
config_t* c_audio_ambient_volume;

static const char* config_file_name = "config.txt";

static void handle_mouse_events(void);
static void handle_keyboard_events(void);

int main(int argc, char* argv[])
{
    base_init();

    config_load(config_file_name);

    c_quit = config_get_bool("c_quit", false, 0);

    c_window_width = config_get_int("c_window_width", 1280, CONFIG_SAVE);
    c_window_height = config_get_int("c_window_height", 720, CONFIG_SAVE);
    c_window_fullscreen = config_get_bool("c_window_fullscreen", false, CONFIG_SAVE);
    c_window_borderless = config_get_bool("c_window_borderless", false, CONFIG_SAVE);

    c_render_vsync = config_get_bool("c_render_vsync", false, CONFIG_SAVE);
    c_render_scale = config_get_float("c_render_scale", 1.0f, CONFIG_SAVE);

    c_audio_master_volume = config_get_float("c_audio_master_volume", 1.0f, CONFIG_SAVE);
    c_audio_music_volume = config_get_float("c_audio_music_volume", 1.0f, CONFIG_SAVE);
    c_audio_sfx_volume = config_get_float("c_audio_sfx_volume", 1.0f, CONFIG_SAVE);
    c_audio_ambient_volume = config_get_float("c_audio_ambient_volume", 1.0f, CONFIG_SAVE);
    c_audio_voice_volume = config_get_float("c_audio_voice_volume", 1.0f, CONFIG_SAVE);

    void* window_handle = window_init("Open Battlecry");
    renderer_init(window_handle);

    image_init_decoders();

    char title[128];
    uint64_t last_tick = time_now();

    font_t arial_32;
    font_load("../assets/fonts/consolas_20.fnt", &arial_32);

    renderer_add_text(&arial_32, 40, 80, "Yo whatsup nigga!");

    //image_t test_image = {0};
    //image_load("../assets/images/sides/dwarves/ADBX.png", IMAGE_LOAD_TRANSPARENT, &test_image);

    //texture_t test_sprite = renderer_add_sprite(&test_image);

    //assert(test_image.sdl_surface == NULL);

    while (!c_quit->bool_value)
    {
        // Events
        window_handle_events();
        c_handle_input_events();

        // Update

        // Render
        renderer_draw();
        renderer_present();

        // Stats
        uint64_t frame_ticks = time_since(&last_tick);
        sprintf(title, "Open Battlecry - %.2f ms", time_ms(frame_ticks));

        window_set_title(title);
    }

    config_save(config_file_name);

    //renderer_destroy_texture(test_sprite);

    renderer_quit();
    window_quit();

    return 0;
}
