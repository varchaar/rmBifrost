
#ifndef COMPOSITOR_CLIENT_H
#define COMPOSITOR_CLIENT_H
#include "../utils/data_structs.h"
#include "../utils/shm_channel.h"
#include "../utils/unix_socket.h"
#include "packets/begin_session_response.h"
#include "packets/packet.h"
#include "packets/submit_frame_packet.h"

#include <atomic>
#include <thread>
#include <mutex>
#include <queue>
#include <optional>

class compositor_client {
public:
    struct compositor_client_config {
        uint32_t title_bar_height;
    };

    enum class client_state {
        CONNECTED = 0,
        SESSION_STARTED = 1,
        DISCONNECTED = 2
    };

    explicit compositor_client(std::unique_ptr<unix_socket::connection> conn, compositor_client_config cfg);
    void start();
    void stop();
    std::optional<std::tuple<uint32_t, submit_frame_packet, rect, uint64_t>> get_swapchain_image();
    void release_swapchain_image(uint32_t frame_id);

    std::string application_name;
    std::string window_title;
    bool prefer_full_screen;
    client_state state = client_state::CONNECTED;
private:
    void handle_packet(const std::shared_ptr<packet>& packet);
    void send_packet(const std::shared_ptr<packet>& resp);
    std::vector<uint64_t> create_swapchain_images();

    compositor_client_config cfg;
    std::thread client_thread;
    std::unique_ptr<unix_socket::connection> conn;
    std::atomic<bool> running;

    uint32_t swapchain_image_count;
    extent swapchain_extent;
    rect composite_region;

    std::string session_name;
    size_t aligned_image_size;
    std::unique_ptr<shm_channel> shared_memory;
    std::vector<bool> framebuffer_in_flight;
    std::vector<submit_frame_packet> submission_info;
    std::queue<uint32_t> submitted_frame_ids;

    std::mutex packet_write_mutex;
};

#endif // COMPOSITOR_CLIENT_H
