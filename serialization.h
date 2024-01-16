#ifndef DANADAM_SERIALIZATION_GUARD_H
#define DANADAM_SERIALIZATION_GUARD_H

#include <stdint.h>
#include <string.h>

#include <stdio.h>

inline void read_chars(const uint8_t * buf, char * result, int resultSize) {
    memcpy(result, buf, (size_t)resultSize);
}
inline void write_chars(uint8_t * buf, const char * value, int valueSize) {
    memcpy(buf, value, (size_t)valueSize);
}

inline uint8_t read_uint8(const uint8_t * buf) {
    return buf[0];
}

inline uint16_t read_uint16_le(const uint8_t * buf) {
    return (uint16_t)(
              ((uint16_t)buf[0] << 0)
            | ((uint16_t)buf[1] << 8)
        );
}
inline uint32_t read_uint32_le(const uint8_t * buf) {
    return (uint32_t)(
              ((uint32_t)buf[0] << 0)
            | ((uint32_t)buf[1] << 8)
            | ((uint32_t)buf[2] << 16)
            | ((uint32_t)buf[3] << 24)
        );
}
inline uint64_t read_uint64_le(const uint8_t * buf) {
    return (uint64_t)(
              ((uint64_t)buf[0] << 0)
            | ((uint64_t)buf[1] << 8)
            | ((uint64_t)buf[2] << 16)
            | ((uint64_t)buf[3] << 24)
            | ((uint64_t)buf[4] << 32)
            | ((uint64_t)buf[5] << 40)
            | ((uint64_t)buf[6] << 48)
            | ((uint64_t)buf[7] << 56)
        );
}

inline uint16_t read_uint16_be(const uint8_t * buf) {
    return (uint16_t)(
              ((uint16_t)buf[0] << 8)
            | ((uint16_t)buf[1] << 0)
        );
}
inline uint32_t read_uint32_be(const uint8_t * buf) {
    return (uint32_t)(
              ((uint32_t)buf[0] << 24)
            | ((uint32_t)buf[1] << 16)
            | ((uint32_t)buf[2] << 8)
            | ((uint32_t)buf[3] << 0)
        );
}
inline uint64_t read_uint64_be(const uint8_t * buf) {
    return (uint64_t)(
              ((uint64_t)buf[0] << 56)
            | ((uint64_t)buf[1] << 48)
            | ((uint64_t)buf[2] << 40)
            | ((uint64_t)buf[3] << 32)
            | ((uint64_t)buf[4] << 24)
            | ((uint64_t)buf[5] << 16)
            | ((uint64_t)buf[6] << 8)
            | ((uint64_t)buf[7] << 0)
        );
}

inline int8_t read_int8(const uint8_t * buf) {
    return (int8_t)read_uint8(buf);
}

inline int16_t read_int16_le(const uint8_t * buf) {
    return (int16_t)read_uint16_le(buf);
}
inline int32_t read_int32_le(const uint8_t * buf) {
    return (int32_t)read_uint32_le(buf);
}
inline int64_t read_int64_le(const uint8_t * buf) {
    return (int64_t)read_uint64_le(buf);
}

inline int16_t read_int16_be(const uint8_t * buf) {
    return (int16_t)read_uint16_be(buf);
}
inline int32_t read_int32_be(const uint8_t * buf) {
    return (int32_t)read_uint32_be(buf);
}
inline int64_t read_int64_be(const uint8_t * buf) {
    return (int64_t)read_uint64_be(buf);
}

inline void write_uint8(uint8_t * buf, uint8_t value) {
    buf[0] = value;
}

inline void write_uint16_le(uint8_t * buf, uint16_t value) {
    uint8_t b[] = {
        (uint8_t)(value >>  0),
        (uint8_t)(value >>  8)
    };
    memcpy(buf, b, 2);
}
inline void write_uint32_le(uint8_t * buf, uint32_t value) {
    uint8_t b[] = {
        (uint8_t)(value >>  0),
        (uint8_t)(value >>  8),
        (uint8_t)(value >> 16),
        (uint8_t)(value >> 24)
    };
    memcpy(buf, b, 4);
}
inline void write_uint64_le(uint8_t * buf, uint64_t value) {
    uint8_t b[] = {
        (uint8_t)(value >>  0),
        (uint8_t)(value >>  8),
        (uint8_t)(value >> 16),
        (uint8_t)(value >> 24),
        (uint8_t)(value >> 32),
        (uint8_t)(value >> 40),
        (uint8_t)(value >> 48),
        (uint8_t)(value >> 56)
    };
    memcpy(buf, b, 8);
}

inline void write_uint16_be(uint8_t * buf, uint16_t value) {
    uint8_t b[] = {
        (uint8_t)(value >>  8),
        (uint8_t)(value >>  0)
    };
    memcpy(buf, b, 2);
}
inline void write_uint32_be(uint8_t * buf, uint32_t value) {
    uint8_t b[] = {
        (uint8_t)(value >> 24),
        (uint8_t)(value >> 16),
        (uint8_t)(value >>  8),
        (uint8_t)(value >>  0)
    };
    memcpy(buf, b, 4);
}
inline void write_uint64_be(uint8_t * buf, uint64_t value) {
    uint8_t b[] = {
        (uint8_t)(value >> 56),
        (uint8_t)(value >> 48),
        (uint8_t)(value >> 40),
        (uint8_t)(value >> 32),
        (uint8_t)(value >> 24),
        (uint8_t)(value >> 16),
        (uint8_t)(value >>  8),
        (uint8_t)(value >>  0)
    };
    memcpy(buf, b, 8);
}

inline void write_int8(uint8_t * buf, int8_t value) {
    write_uint8(buf, (uint8_t)value);
}

inline void write_int16_le(uint8_t * buf, int16_t value) {
    write_uint16_le(buf, (uint16_t)value);
}
inline void write_int32_le(uint8_t * buf, int32_t value) {
    write_uint32_le(buf, (uint32_t)value);
}
inline void write_int64_le(uint8_t * buf, int64_t value) {
    write_uint64_le(buf, (uint64_t)value);
}

inline void write_int16_be(uint8_t * buf, int16_t value) {
    write_uint16_be(buf, (uint16_t)value);
}
inline void write_int32_be(uint8_t * buf, int32_t value) {
    write_uint32_be(buf, (uint32_t)value);
}
inline void write_int64_be(uint8_t * buf, int64_t value) {
    write_uint64_be(buf, (uint64_t)value);
}

#endif
