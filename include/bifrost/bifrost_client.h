
#ifndef BIFROST_CLIENT_H
#define BIFROST_CLIENT_H

#include <string>
#include <cstdint>
#include <memory>
class bifrost_client_impl;

class bifrost_client {
public:
    explicit bifrost_client(std::string application_name, std::string window_title, bool prefer_full_screen, uint32_t swapchain_image_count);
private:
    std::shared_ptr<bifrost_client_impl> impl;
    std::string application_name;
    std::string window_title;
    bool prefer_full_screen{};
    uint32_t swapchain_image_count{};
};

#endif // BIFROST_CLIENT_H
