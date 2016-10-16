#ifndef RSC_NET_PACKET_HPP
#define RSC_NET_PACKET_HPP

#pragma once

#include "sysd/net/buffer.hpp"

namespace rsc::net {
    class packet : public sysd::net::buffer {
        packet() = default;
    };
}

#endif // RSC_NET_PACKET_HPP
