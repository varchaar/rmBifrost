#ifndef BOOT_SCREEN_H
#define BOOT_SCREEN_H
#include <condition_variable>
#include <vector>
#include <cstdint>

#include "lvgl_renderer.h"

enum boot_screen_state {
    BOOT_SCREEN_IN_FLIGHT,
    RELINQUISH_CONTROL,
};


class boot_screen {
public:
    std::condition_variable cv;

    std::mutex cv_m;

    boot_screen_state state = BOOT_SCREEN_IN_FLIGHT;
    std::vector<uint8_t> font_buf;

    void start();


    explicit boot_screen(const std::shared_ptr<lvgl_renderer> &shared) : lvgl_renderer_inst(shared) {
    }

    ~boot_screen();

private:
    std::vector<uint8_t> lottie_buf;
    std::vector<uint8_t> welcome_json;

    std::shared_ptr<lvgl_renderer> lvgl_renderer_inst;

    std::vector<lv_obj_t *> deletion_queue;

    void setup_animation();

    void setup_boot_selection();
};


#endif //BOOT_SCREEN_H
