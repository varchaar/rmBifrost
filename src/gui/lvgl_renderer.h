
#ifndef LVGL_APP_H
#define LVGL_APP_H

#include "../hook_typedefs.h"
#include "lv_conf.h"

#include <QImage>
#include <condition_variable>
#include <lvgl.h>
#include <memory>

struct display_config {
    QImage* fb;
    QObject* epfb_inst;
    ScreenUpdateFunc screen_update_func;
};

class lvgl_renderer : public std::enable_shared_from_this<lvgl_renderer> {
public:
    explicit lvgl_renderer(const display_config& cfg)
        : cfg(cfg)
    {
    }
    void initialize();
    void start();
private:
    static std::weak_ptr<lvgl_renderer> instance;
    display_config cfg;
    lv_display_t* display;
    uint8_t* compose_buffer;
    long last_full_refresh_time = 0;

    void tick();

    void lv_display_flush(lv_display_t* disp, const lv_area_t* area, uint8_t* color_p);
    static void lv_display_flush_cb(lv_display_t* disp, const lv_area_t* area, uint8_t* color_p);
};

#endif // LVGL_APP_H
