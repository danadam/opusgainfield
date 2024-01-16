#ifndef OPUS_GAIN_FIELD_CRC_H
#define OPUS_GAIN_FIELD_CRC_H

#include <boost/crc.hpp>
constexpr uint32_t crc_polynomial = 0x04C11DB7;
constexpr uint32_t crc_init_val = 0;
constexpr uint32_t crc_final_xor = 0;
constexpr bool crc_reflect_in = false;
constexpr bool crc_reflect_rem = false;
using crc_maker = boost::crc_optimal<32, crc_polynomial, crc_init_val, crc_final_xor, crc_reflect_in, crc_reflect_rem>;

inline
uint32_t
make_crc(const std::vector<uint8_t> & data) {
    crc_maker c;
    c.process_bytes(&data[0], data.size());
    return c.checksum();
}

#endif
