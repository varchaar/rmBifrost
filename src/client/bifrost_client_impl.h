
#ifndef BIFROST_CLIENT_IMPL_H
#define BIFROST_CLIENT_IMPL_H
#include <cstdint>
#include <string>
#include <memory>

#include "../utils/unix_socket.h"
#include "../compositor/packets/packet.h"
#include "../utils/shm_channel.h"

class bifrost_client_impl {
public:
    bifrost_client_impl(std::string application_name, std::string window_title, bool prefer_full_screen, uint32_t swapchain_image_count);
private:
    std::unique_ptr<unix_socket> socket;
    std::unique_ptr<shm_channel> channel;

    void initialize(std::string application_name, std::string window_title, bool prefer_full_screen, uint32_t swapchain_image_count);
    void send_packet(const std::shared_ptr<packet>& packet) const;
    std::shared_ptr<packet> receive_packet() const;
    void create_shm_channel();

    std::string session_name;
    uint32_t swapchain_image_count;
};



#endif //BIFROST_CLIENT_IMPL_H
