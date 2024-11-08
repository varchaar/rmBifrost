
#ifndef COMPOSITOR_H
#define COMPOSITOR_H
#include "../gui/lvgl_renderer.h"
#include "../utils/shm_channel.h"
#include "../utils/unix_socket.h"
#include "compositor_client.h"

#include <memory>
#include <thread>

class packet;
class compositor {
public:
    explicit compositor(std::shared_ptr<lvgl_renderer> renderer);

    void start();
    void stop();

private:
    std::atomic<bool> running = false;

    std::shared_ptr<lvgl_renderer> renderer;
    std::unique_ptr<unix_socket> socket;
    std::thread listener_thread;

    std::vector<std::shared_ptr<compositor_client>> clients;

    void listener();
};



#endif //COMPOSITOR_H
