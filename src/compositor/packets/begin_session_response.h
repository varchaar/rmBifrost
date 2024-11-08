
#ifndef BEGIN_SESSION_RESPONSE_H
#define BEGIN_SESSION_RESPONSE_H

#include "packet.h"

#include <cereal/types/polymorphic.hpp>

class begin_session_response : public packet {
public:
    uint32_t swapchain_image_count;
    std::string session_name;

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(swapchain_image_count, session_name);
    }

    void poly() override {};
};

#include <cereal/archives/binary.hpp>

CEREAL_REGISTER_TYPE(begin_session_response);

CEREAL_REGISTER_POLYMORPHIC_RELATION(packet, begin_session_response);

#endif //BEGIN_SESSION_RESPONSE_H
