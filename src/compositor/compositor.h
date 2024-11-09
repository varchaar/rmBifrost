
#ifndef COMPOSITOR_H
#define COMPOSITOR_H
#include "../gui/lvgl_renderer.h"
#include "../utils/shm_channel.h"
#include "../utils/unix_socket.h"
#include "compositor_client.h"

#include <memory>
#include <thread>

class compositor {
public:
    struct display_config {
        QImage* fb;
        QObject* epfb_inst;
        ScreenUpdateFunc screen_update_func;
    };

    explicit compositor(display_config cfg);

    void start();
    void stop();
    void request_refresh(rect update_region, refresh_type type);
private:
    std::atomic<bool> running = false;

    display_config cfg;
    std::shared_ptr<lvgl_renderer> renderer;
    std::unique_ptr<unix_socket> socket;
    std::thread listener_thread;
    std::thread render_thread;
    std::mutex fb_mutex;

    std::vector<std::shared_ptr<compositor_client>> clients;

    std::vector<std::pair<rect, refresh_type>> pending_refresh_requests;

    void listener();
    void refresh(point p1, point p2, refresh_type type);
    void render_clients();
};



#endif //COMPOSITOR_H
