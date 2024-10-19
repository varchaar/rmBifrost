#include "bifrost_impl.h"

#include <InlineHook.h>
#include <QImage>
#include <QMetaEnum>
#include <QPainter>
#include <spdlog/spdlog.h>

#include "hook_typedefs.h"

std::weak_ptr<bifrost_impl> bifrost_impl::instance;

void bifrost_impl::initialize()
{
    instance = shared_from_this();
    initialize_hooks();
}

QObject* bifrost_impl::create_ep_fb()
{
    spdlog::debug("create_ep_fb hook called");
    auto epfb_inst = create_ep_fb_original();
    start_bifrost(epfb_inst);
    return epfb_inst;
}

void bifrost_impl::start_bifrost(QObject* epfb_inst)
{
    QImage* fb = *reinterpret_cast<QImage**>(reinterpret_cast<void*>(epfb_inst) + 0xc0);
    spdlog::debug("fb: {}x{}x{}", fb->width(), fb->height(), fb->depth());
    spdlog::debug("fb format: {}", QMetaEnum::fromType<QImage::Format>().valueToKey(fb->format()));

    auto screen_update_func = reinterpret_cast<ScreenUpdateFunc>(screen_update_func_addr);
    lvgl_app_inst = std::make_shared<lvgl_app>(display_config { fb, epfb_inst, screen_update_func });
    lvgl_app_inst->initialize();

    int freq = 1000 / 85;
    long last_tick = 0;
    while (true) {
        lvgl_app_inst->tick();
        long now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        long sleep_time = freq - (now - last_tick);
        if (sleep_time > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        }
        last_tick = now;
    }
}

void bifrost_impl::initialize_hooks()
{
    BWSR_InlineHook(
        create_ep_fb_addr, reinterpret_cast<void*>(create_ep_fb_hook),
        reinterpret_cast<void**>(&create_ep_fb_original), nullptr, nullptr);
}
