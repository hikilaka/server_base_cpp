#ifndef SYSD_BUFFER_HPP
#define SYSD_BUFFER_HPP

#pragma once

#include <cstdint>
#include <vector>

#include <boost/endian/conversion.hpp>

namespace sysd {
    class buffer {
    public:
        static const std::size_t DEFAULT_CAPACITY = 0x10;

        buffer(const std::size_t capacity = DEFAULT_CAPACITY)
            : write_pos(0)
        {
            payload.reserve(capacity);
        }

        buffer& operator<<(std::uint8_t value) {
            append<std::uint8_t, 1>(value);
            return *this;
        }
        
        buffer& operator<<(std::uint16_t value) {
            append<std::uint16_t, 2>(value);
            return *this;
        }

        buffer& operator<<(std::uint32_t value) {
            append<std::uint32_t, 4>(value);
            return *this;
        }

        buffer& operator<<(std::uint64_t value) {
            append<std::uint64_t, 8>(value);
            return *this;
        }

        buffer& operator<<(std::int8_t value) {
            append<std::int8_t, 1>(value);
            return *this;
        }
        
        buffer& operator<<(std::int16_t value) {
            append<std::int16_t, 2>(value);
            return *this;
        }

        buffer& operator<<(std::int32_t value) {
            append<std::int32_t, 4>(value);
            return *this;
        }

        buffer& operator<<(std::int64_t value) {
            append<std::int64_t, 8>(value);
            return *this;
        }

       template <typename T, std::size_t S>
        void append(T value) {
            value = boost::endian::native_to_big(value);
            append(reinterpret_cast<std::uint8_t *>(&value), S);
        }


        template <typename T>
        void append(const T *src, std::size_t count) {
            append(reinterpret_cast<const std::uint8_t *>(src), count);
        }
        
        void append(const std::uint8_t *src, std::size_t count) {
            if (count == 0) 
                return;

            std::cout << "APPENDING: count=" << count << ", data={"; 
            for (std::size_t i = 0; i < count; i++)
                std::cout << static_cast<std::uint32_t>(src[i]) << ", ";
            std::cout << "\b\b}" << std::endl;

            payload.insert(payload.end(), src, src+count);
            write_pos += count;
        }

        void print() {
            std::cout << "buffer(payload={";
            if (payload.size() > 0) {
                for (auto &i : payload) {
                    std::cout << static_cast<std::uint32_t>(i) << ", ";
                }
                std::cout << "\b\b";
            }
            std::cout << "}, size=" << payload.size();
            std::cout << ", write_pos=" << write_pos << ")" << std::endl; 
        }

        const std::vector<std::uint8_t> data() { return payload; }
    private:
        std::size_t write_pos;
        std::vector<std::uint8_t> payload;
    };
}

#endif

