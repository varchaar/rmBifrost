#include "bifrost_impl.h"

#include <BSWR/InlineHook.h>
#include <spdlog/spdlog.h>

#include "hook_typedefs.h"

std::weak_ptr<bifrost_impl> bifrost_impl::instance;

void bifrost_impl::initialize() {
    instance = shared_from_this();
    initialize_hooks();
}

QObject *bifrost_impl::create_ep_fb() {
    spdlog::debug("create_ep_fb hook called");
    return create_ep_fb_original();
}

void bifrost_impl::initialize_hooks() {
    BWSR_InlineHook(
        create_ep_fb_addr, reinterpret_cast<void *>(create_ep_fb_hook),
        reinterpret_cast<void **>(&create_ep_fb_original), nullptr, nullptr);
}
