#include "lvgl_app.h"

#include "../constants.h"

#include <spdlog/spdlog.h>

std::weak_ptr<lvgl_app> lvgl_app::instance;

uint32_t lv_tick_cb()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void lvgl_app::lv_display_flush(lv_display_t* disp, const lv_area_t* area, uint8_t* color_p)
{
    int fb_width = cfg.fb->width();
    int fb_height = cfg.fb->height();
    int fb_depth = cfg.fb->depth() / 8;

    spdlog::debug("flushing area: {}x{}-{}x{}", area->x1, area->y1, area->x2, area->y2);
    // copy the area from the lvgl buffer to the QImage buffer
    for (int y = area->y1; y <= area->y2; y++) {
        QRgb* line = reinterpret_cast<QRgb*>(cfg.fb->scanLine(y));
        int lvgl_idx = (y * fb_width + area->x1) * fb_depth;
        for (int x = area->x1; x <= area->x2; x++) {
            if (fb_depth == 4) {
                line[x] = qRgba(color_p[lvgl_idx + 1], color_p[lvgl_idx + 2], color_p[lvgl_idx + 3], 255);
            } else {
                line[x] = qRgb(color_p[lvgl_idx + 1], color_p[lvgl_idx + 2], color_p[lvgl_idx + 3]);
            }
            lvgl_idx += 4;
        }
    }

    spdlog::debug("addr {} {}", (void*)compose_buffer, (void*) color_p);


    cfg.screen_update_func(cfg.epfb_inst, { area->x1, area->y1 }, { area->x2, area->y2 }, 1, 0, 0);
    lv_display_flush_ready(disp);

}

void lvgl_app::lv_display_flush_cb(lv_display_t* disp, const lv_area_t* area, uint8_t* color_p)
{
    auto instance = lvgl_app::instance.lock();
    if (!instance)
        return;

    instance->lv_display_flush(disp, area, color_p);
}

void lvgl_app::start_launcher()
{
    lv_obj_t * lottie = lv_lottie_create(lv_screen_active());
    lv_lottie_set_src_data(lottie, WELCOME_JSON, strlen(WELCOME_JSON));

    static uint8_t buf[856 * 246 * 4];
    lv_lottie_set_buffer(lottie, 856, 246, buf);

    lv_obj_center(lottie);
}

void lvgl_app::initialize()
{
    instance = shared_from_this();

    lv_init();
    lv_tick_set_cb(lv_tick_cb);
    display = lv_display_create(cfg.fb->width(), cfg.fb->height());

    lv_indev_t *touch = lv_evdev_create(LV_INDEV_TYPE_POINTER, "/dev/input/event3");
    lv_evdev_set_calibration(touch, 0, 0, 2160, 2880);
    lv_indev_set_display(touch, display);

    auto buf_size = cfg.fb->width() * cfg.fb->height() * cfg.fb->depth() / 8;
    compose_buffer = new uint8_t[buf_size];
    lv_display_set_buffers(display, compose_buffer, nullptr, buf_size, LV_DISPLAY_RENDER_MODE_DIRECT);
    lv_display_set_flush_cb(display, lv_display_flush_cb);
    spdlog::info("lvgl_app initialized");

    start_launcher();
}

void lvgl_app::tick()
{
    lv_timer_handler();
}
