#include "compositor.h"

#include <spdlog/spdlog.h>

#include <utility>
compositor::compositor(std::shared_ptr<lvgl_renderer> renderer)
    : renderer(std::move(renderer))
    , socket(std::make_unique<unix_socket>("/run/bifrost_comp_ctl.sock", true))
{
}
void compositor::start()
{
    spdlog::info("Starting bifrost compositor");
    running = true;
    listener_thread = std::thread(&compositor::listener, this);
}
void compositor::stop()
{
    running = false;
    for (const auto& client : clients) {
        client->stop();
    }
    listener_thread.join();
}
void compositor::listener()
{
    while (running) {
        spdlog::debug("Waiting for connection");
        auto connection = socket->accept_connection();
        spdlog::debug("Accepted connection");
        auto client = std::make_shared<compositor_client>(std::move(connection));
        clients.push_back(client);
        client->start();
    }
}