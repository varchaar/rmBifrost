
#ifndef COMPOSITOR_CLIENT_H
#define COMPOSITOR_CLIENT_H
#include "../utils/unix_socket.h"
#include "packets/begin_session_response.h"
#include "packets/packet.h"
#include "packets/submit_frame.h"
#include "../utils/shm_channel.h"

#include <atomic>
#include <thread>
#include <mutex>
#include <queue>

class compositor_client {
public:
    enum class client_state {
        CONNECTED,
        SESSION_STARTED,
        DISCONNECTED
    };

    explicit compositor_client(std::unique_ptr<unix_socket::connection> conn);
    void start();
    void stop();

    client_state state;
private:
    void handle_packet(const std::shared_ptr<packet>& packet);
    void send_packet(const std::shared_ptr<packet>& resp);

    std::thread client_thread;
    std::unique_ptr<unix_socket::connection> conn;
    std::atomic<bool> running;

    std::string application_name;
    std::string window_title;
    bool prefer_full_screen;
    uint32_t swapchain_image_count;
    std::string session_name;
    std::unique_ptr<shm_channel> shared_memory;
    std::vector<bool> framebuffer_in_flight;
    std::vector<submit_frame> submission_info;
    std::queue<uint32_t> pending_frame_ids;

    std::mutex packet_write_mutex;
};

#endif // COMPOSITOR_CLIENT_H
