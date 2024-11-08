
#ifndef SUBMIT_FRAME_H
#define SUBMIT_FRAME_H

#include "packet.h"
#include "../../constants.h"

#include <cereal/types/polymorphic.hpp>
#include <cstdint>

struct submit_frame : packet {
    struct rect {
        uint32_t x1;
        uint32_t y1;
        uint32_t x2;
        uint32_t y2;

        template<class Archive>
        void serialize(Archive& archive) {
            archive(x1, y1, x2, y2);
        }
    };
    uint8_t framebuffer_id;
    rect dirty_rect;
    refresh_type preferred_refresh_type;

    template<class Archive>
    void serialize(Archive& archive) {
        archive(framebuffer_id, dirty_rect, preferred_refresh_type);
    }

    void poly() override {};
};

#include <cereal/archives/binary.hpp>

CEREAL_REGISTER_TYPE(submit_frame);

CEREAL_REGISTER_POLYMORPHIC_RELATION(packet, submit_frame);

#endif //SUBMIT_FRAME_H
