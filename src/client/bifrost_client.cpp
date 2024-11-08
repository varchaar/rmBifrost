#include "bifrost/bifrost_client.h"
#include "bifrost_client_impl.h"

bifrost_client::bifrost_client(std::string application_name, std::string window_title, bool prefer_full_screen, uint32_t swapchain_image_count)
    : impl(std::make_shared<bifrost_client_impl>(application_name, window_title, prefer_full_screen, swapchain_image_count))
{
}