
#include "bifrost_client_impl.h"

#include <sys/socket.h>
#include <cereal/archives/binary.hpp>
#include <sys/socket.h>
#include <spdlog/spdlog.h>
#include "../compositor/packets/begin_session_request.h"
#include "../compositor/packets/begin_session_response.h"
#include "../compositor/packets/release_frame.h"
#include "../compositor/packets/submit_frame.h"

#include <sys/un.h>

bifrost_client_impl::bifrost_client_impl(std::string application_name, std::string window_title, bool prefer_full_screen, uint32_t swapchain_image_count)
    : socket(std::make_unique<unix_socket>("/run/bifrost_comp_ctl.sock", false))
{
    initialize(application_name, window_title, prefer_full_screen, swapchain_image_count);
    create_shm_channel();
}

void bifrost_client_impl::initialize(std::string application_name, std::string window_title, bool prefer_full_screen, uint32_t swapchain_image_count)
{
    auto request = std::make_shared<begin_session_request>();
    request->application_name = application_name;
    request->window_title = window_title;
    request->prefer_full_screen = prefer_full_screen;
    request->swapchain_image_count = swapchain_image_count;
    send_packet(request);
}

void bifrost_client_impl::create_shm_channel()
{
    auto pkt = receive_packet();
    if (std::dynamic_pointer_cast<begin_session_response>(pkt) == nullptr) {
        throw std::runtime_error("Received invalid response from server");
    }
    auto response = std::static_pointer_cast<begin_session_response>(pkt);
    spdlog::info("Session name: {}", response->session_name);
    session_name = response->session_name;
    swapchain_image_count = response->swapchain_image_count;
    channel = std::make_unique<shm_channel>(session_name, -1, false);
    spdlog::info("Created shared memory channel");
}

void bifrost_client_impl::send_packet(const std::shared_ptr<packet>& packet) const
{
    std::ostringstream oss;
    cereal::BinaryOutputArchive archive(oss);
    archive(packet);
    spdlog::debug("Serialized packet size: {}", oss.str().size());
    const std::string serialized_packet = oss.str();
    size_t size = serialized_packet.size();
    socket->get_connection()->write(reinterpret_cast<const char*>(&size), sizeof(size));
    socket->get_connection()->write(serialized_packet.c_str(), size);
}

std::shared_ptr<packet> bifrost_client_impl::receive_packet() const
{
    size_t size;
    socket->get_connection()->read(reinterpret_cast<char*>(&size), sizeof(size));
    std::string serialized_packet;
    socket->get_connection()->read(serialized_packet.data(), size);
    std::istringstream iss(serialized_packet);
    cereal::BinaryInputArchive archive(iss);
    std::shared_ptr<packet> packet;
    archive(packet);
    return packet;
}
