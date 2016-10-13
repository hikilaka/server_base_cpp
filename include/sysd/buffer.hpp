#ifndef SYSD_BUFFER_HPP
#define SYSD_BUFFER_HPP

#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include <boost/endian/conversion.hpp>

namespace sysd {
    struct bits {
        bits(std::uint32_t value, std::size_t nbits)
            : value(value), nbits(nbits) {  }

        std::uint32_t value;
        std::size_t nbits;
    };

    class buffer {
    public:
        static const std::size_t DEFAULT_CAPACITY = 0x10;

        buffer(const std::size_t capacity = DEFAULT_CAPACITY)
            : write_pos(0), read_pos(0), bit_pos(0)
        {
            payload.reserve(capacity);
        }

        buffer& operator<<(bool value) {
            append<std::uint8_t, 1>(value ? 1 : 0);
            return *this;
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

        buffer& operator<<(const char *str) {
            if (std::size_t length = std::strlen(str); length > 0) {
                append(reinterpret_cast<const std::uint8_t *>(str), length);
            }
            append<std::uint8_t, 1>(0);
            return *this;
        }

        buffer& operator<<(std::string str) {
            if (str.length() > 0) {
                append(reinterpret_cast<const std::uint8_t *>(str.c_str()),
                       str.length());
            }
            append<std::uint8_t, 1>(0);
            return *this;
        }

        buffer& operator<<(sysd::bits bits) {
            static const std::uint32_t bitmasks[] = {
                0, 0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f,
                0x7f, 0xff, 0x1ff, 0x3ff, 0x7ff, 0xfff, 0x1fff, 0x3fff, 0x7fff,
                0xffff, 0x1ffff, 0x3ffff, 0x7ffff, 0xfffff, 0x1fffff, 0x3fffff,
                0x7fffff, 0xffffff, 0x1ffffff, 0x3ffffff, 0x7ffffff, 0xfffffff,
                0x1fffffff, 0x3fffffff, 0x7fffffff, 0xffffffff
            };

            // no need to swap endianness if less than 2 bytes, i believe.
            // i'll have to do further testing
            // possible solution to a possible bug:
            //
            // if (bits.nbits >= 16)
            //     bits.value = boost::endian::native_to_big(bits.value);
            
            std::size_t byte_pos = bit_pos >> 3;
            std::size_t bit_offset = 8 - (bit_pos & 7);

            bit_pos += bits.nbits;
            write_pos = (bit_pos + 7) / 8;
            payload.reserve(write_pos);

            // Note: we have to populate the internal container
            // before we can begin writing to it, even though
            // we have already allocated the space for our writes!
            while (payload.size() < write_pos) {
                payload.emplace_back(0);
            }

            for (; bits.nbits > bit_offset; bit_offset = 8) {
                payload[byte_pos] &= ~bitmasks[bit_offset];
                payload[byte_pos++] |= (bits.value >> (bits.nbits - bit_offset))
                                            & bitmasks[bit_offset];
                bits.nbits -= bit_offset;
            }
            if (bits.nbits == bit_offset) {
                payload[byte_pos] &= ~bitmasks[bit_offset];
                payload[byte_pos] |= bits.value & bitmasks[bit_offset];
            } else {
                payload[byte_pos] &= ~(bitmasks[bits.nbits]
                                            << (bit_offset - bits.nbits));
                payload[byte_pos] |= (bits.value & bitmasks[bits.nbits])
                                            << (bit_offset - bits.nbits);
            }
            return *this;
        }

        buffer& operator>>(bool& value) {
            value = consume<std::uint8_t, 1>() == 1 ? true : false;
            return *this;
        }
        
        buffer& operator>>(std::uint8_t& value) {
            value = consume<std::uint8_t, 1>();
            return *this;
        }
        
        buffer& operator>>(std::uint16_t& value) {
            value = consume<std::uint16_t, 2>();
            return *this;
        }
        
        buffer& operator>>(std::uint32_t& value) {
            value = consume<std::uint32_t, 4>();
            return *this;
        }
        
        buffer& operator>>(std::uint64_t& value) {
            value = consume<std::uint64_t, 8>();
            return *this;
        }
        buffer& operator>>(std::int8_t& value) {
            value = consume<std::int8_t, 1>();
            return *this;
        }

        buffer& operator>>(std::int16_t& value) {
            value = consume<std::int16_t, 2>();
            return *this;
        }

        buffer& operator>>(std::int32_t& value) {
            value = consume<std::int32_t, 4>();
            return *this;
        }

        buffer& operator>>(std::int64_t& value) {
            value = consume<std::int64_t, 8>();
            return *this;
        }

        buffer& operator>>(std::string& value) {
            return *this;
        }

        friend std::ostream& operator<<(std::ostream &o, const buffer &b) {
            o << "buffer(payload={";
            if (b.payload.size() > 0) {
                for (auto &i : b.payload) {
                    o << static_cast<std::uint32_t>(i) << ", ";
                }
                o << "\b\b";
            }
            o << "}, size=" << b.payload.size();
            o << ", write_pos=" << b.write_pos;
            o << ", read_pos=" << b.read_pos;
            o << ", bit_pos=" << b.bit_pos << ")";
            return o;
        }

        const std::vector<std::uint8_t>& data() { return payload; }
   private:
       template <typename T, std::size_t S>
        void append(T value) {
            value = boost::endian::native_to_big(value);
            append(reinterpret_cast<const std::uint8_t *>(&value), S);
        }


        template <typename T>
        void append(const T *src, std::size_t count) {
            append(reinterpret_cast<const std::uint8_t *>(src), count);
        }
        
        void append(const std::uint8_t *src, std::size_t count) {
            if (count == 0) 
                return;

            payload.insert(payload.end(), src, src+count);
            write_pos += count;
        }

        template <typename T, std::size_t S>
        T consume() {
            T value = *((T*) &payload[read_pos]);
            read_pos += S;
            return boost::endian::big_to_native(value);
        }

        std::size_t write_pos, read_pos, bit_pos;
        std::vector<std::uint8_t> payload;
    };
}

#endif // SYSD_BUFFER_HPP

