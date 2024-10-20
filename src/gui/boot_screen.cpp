#include "boot_screen.h"

#include <thread>
#include <spdlog/spdlog.h>

#include "../constants.h"
#include <src/core/lv_obj_pos.h>
#include <src/display/lv_display.h>
#include <src/misc/lv_anim.h>
#include <src/widgets/lottie/lv_lottie.h>

void boot_screen::start()
{
    // setup_animation();
    //
    // std::this_thread::sleep_for(std::chrono::milliseconds(4000));

    setup_boot_selection();
    std::this_thread::sleep_for(std::chrono::milliseconds(350000000));
}

boot_screen::~boot_screen() {
    for (auto obj : deletion_queue) {
        lv_obj_delete(obj);
    }
}

void boot_screen::setup_animation()
{
    welcome_json = get_resource_file("animations/hello.json");

    auto lottie_obj = lv_lottie_create(lv_screen_active());
    lv_lottie_set_src_data(lottie_obj, welcome_json.data(), welcome_json.size());
    lv_anim_set_repeat_count(lv_lottie_get_anim(lottie_obj), 1);

    auto width = SCREEN_WIDTH / 2;
    auto height = SCREEN_WIDTH / 2 / 3;
    lottie_buf.resize(width * height * 4);
    lv_lottie_set_buffer(lottie_obj, width, height, lottie_buf.data());

    lv_obj_center(lottie_obj);

    deletion_queue.push_back(lottie_obj);
}

void boot_screen::setup_boot_selection() {
    auto btn_group_rm = lv_obj_create(lv_screen_active());
    lv_obj_set_size(btn_group_rm, LV_PCT(60), LV_SIZE_CONTENT);
    lv_obj_set_style_border_color(btn_group_rm, lv_color_black(), 0);
    lv_obj_set_style_border_width(btn_group_rm, 7, 0);
    lv_obj_set_style_bg_color(btn_group_rm, lv_color_white(), 0);
    lv_obj_set_style_radius(btn_group_rm, 0, 0);
    lv_obj_align(btn_group_rm, LV_ALIGN_BOTTOM_MID, 0, -200);
    lv_obj_set_flex_flow(btn_group_rm, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(btn_group_rm, 40, 0);

    // Create the title label
    auto title_label = lv_label_create(btn_group_rm);
    lv_label_set_text(title_label, "reMarkable OS");
    lv_obj_set_style_text_font(title_label, &ebgaramond_48, 0);
    lv_obj_set_size(title_label, LV_PCT(100), LV_SIZE_CONTENT);

    // Create the subtitle label
    auto subtitle_label = lv_label_create(btn_group_rm);
    lv_label_set_text(subtitle_label, "Tap to enter");
    lv_obj_set_size(subtitle_label, LV_PCT(100), LV_SIZE_CONTENT);

    // Add the box to the deletion queue
    deletion_queue.push_back(title_label);
    deletion_queue.push_back(subtitle_label);
    deletion_queue.push_back(btn_group_rm);
}