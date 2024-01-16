/*
 * RFC 3533 The Ogg Encapsulation Format Version 0
 * https://datatracker.ietf.org/doc/html/rfc3533
 *
 * RFC 7845 Ogg Encapsulation for the Opus Audio Codec
 * https://datatracker.ietf.org/doc/html/rfc7845.html
 */
#include <algorithm>
#include <array>
#include <numeric>
#include <cmath>
#include <cstdio>
#include <vector>

#include "crc.h"
#include "options.h"
#include "myassert.h"
#include "serialization.h"


constexpr std::array<uint8_t, 4> OGG_MAGIC{ 'O', 'g', 'g', 'S' };
constexpr int OGG_CRC_OFFSET = 22;
constexpr int OGG_PAGE_SEGMENTS_OFFSET = 26;

constexpr std::array<uint8_t, 8> OPUS_MAGIC{ 'O', 'p', 'u', 's', 'H', 'e', 'a', 'd' };
constexpr int OPUS_GAIN_OFFSET = 16;

[[noreturn]] void
exit_with_file_error(FILE * fp, const char * context_msg) {
    if (ferror(fp)) {
        perror(context_msg);
    }
    else {
        fprintf(stderr, "%s failed, end of file\n", context_msg);
    }
    std::exit(EXIT_FAILURE);
}

std::vector<uint8_t>
load_bytes(FILE * fp, int pos, int len, const char * context_msg) {
    if (fseek(fp, pos, SEEK_SET) == 0) {
        std::vector<uint8_t> result;
        result.resize(len);
        int nread = fread(&result[0], 1, len, fp);
        if (nread == len) {
            return result;
        }
    }
    exit_with_file_error(fp, context_msg);
}

uint8_t
load_uint8(FILE * fp, int pos, const char * context_msg) {
    if (fseek(fp, pos, SEEK_SET) == 0) {
        uint8_t result;
        int nread = fread(&result, 1, 1, fp);
        if (nread == 1) {
            return result;
        }
    }
    exit_with_file_error(fp, context_msg);
}

void
store_uint16(FILE * fp, int pos, uint16_t value, const char * context_msg) {
    uint8_t buf[2];
    write_uint16_le(buf, value);
    if (fseek(fp, pos, SEEK_SET) == 0) {
        int nwrite = fwrite(buf, 1, 2, fp);
        if (nwrite == 2) {
            return;
        }
    }
    exit_with_file_error(fp, context_msg);
}

void
store_uint32(FILE * fp, int pos, uint32_t value, const char * context_msg) {
    uint8_t buf[4];
    write_uint32_le(buf, value);
    if (fseek(fp, pos, SEEK_SET) == 0) {
        int nwrite = fwrite(buf, 1, 4, fp);
        if (nwrite == 4) {
            return;
        }
    }
    exit_with_file_error(fp, context_msg);
}

uint16_t
to_q78(double d) {
//    double integral = 0;
//    auto fractional = std::modf(d, &integral);
//    int8_t hi = integral;
//    int8_t lo = std::floor(std::abs(fractional * 256));
//    return static_cast<uint16_t>( (hi << 8) | lo );
    int i = std::floor(d * 256);
    return static_cast<uint16_t>( i );
}

double
from_q78(uint16_t q) {
    auto i = static_cast<int16_t>(q);
    return 1.0 * i / 256;
}

int
main(int argc, char * argv[]) {
    Options opts = parseOptions(argc, argv);
    if (opts.doHelp) {
        help(stdout, argv[0]);
        std::exit(EXIT_SUCCESS);
    }

    FILE * fp = fopen(opts.input.c_str(), "r+");
    if (!fp) {
        perror(opts.input.c_str());
        std::exit(EXIT_FAILURE);
    }

    const auto ogg_magic_in_file = load_bytes(fp, 0, OGG_MAGIC.size(), "load OGG magic string");
    if (!std::equal(begin(OGG_MAGIC), end(OGG_MAGIC), begin(ogg_magic_in_file), end(ogg_magic_in_file))) {
        fprintf(stderr, "Doesn't look like OGG file, wrong magic string.\n");
        std::exit(EXIT_FAILURE);
    }

    const auto num_of_segments = load_uint8(fp, OGG_PAGE_SEGMENTS_OFFSET, "load OGG num of page segments");
    const auto segment_sizes = load_bytes(fp, OGG_PAGE_SEGMENTS_OFFSET+1, num_of_segments, "load OGG segment sizes");
    const auto total_segments_size = std::accumulate(begin(segment_sizes), end(segment_sizes), int{});

    const int ogg_header_size = num_of_segments + 27;
    const int ogg_page_size = ogg_header_size + total_segments_size;

    if (total_segments_size < OPUS_GAIN_OFFSET + 2) {
        fprintf(stderr, "Doesn't look like Opus file, not enough data for ID Header.");
        std::exit(EXIT_FAILURE);
    }

    auto full_ogg_page = load_bytes(fp, 0, ogg_page_size, "load OGG full page");

    const auto crc_in_file = read_uint32_le(&full_ogg_page[OGG_CRC_OFFSET]);
    std::fill_n(begin(full_ogg_page) + OGG_CRC_OFFSET, 4, '\0');
    const auto crc_computed = make_crc(full_ogg_page);

    if (crc_in_file != crc_computed) {
        fprintf(stderr, "Invalid OGG file, CRC in file: 0x%08x, actual CRC of data: 0x%08x\n", crc_in_file, crc_computed);
        std::exit(EXIT_FAILURE);
    }
    // this should be safe becase we verified earlier that full_ogg_page has enough bytes
    if (!std::equal(begin(OPUS_MAGIC), end(OPUS_MAGIC), begin(full_ogg_page)+ogg_header_size)) {
        fprintf(stderr, "Doesn't look like Opus file, wrong magic string.\n");
        std::exit(EXIT_FAILURE);
    }

    const auto cur_gain_in_q78 = read_uint16_le(&full_ogg_page[ogg_header_size + OPUS_GAIN_OFFSET]);
    const auto cur_gain_in_dB = from_q78(cur_gain_in_q78);

    if (opts.editMode == Options::EditMode::None) {
        printf("gain: %g dB\n", cur_gain_in_dB);
        std::exit(EXIT_SUCCESS);
    }

    const auto new_gain_in_dB = [&opts, cur_gain_in_dB]() {
        switch (opts.editMode) {
            case Options::EditMode::Set: return opts.gain;
            case Options::EditMode::Add: return cur_gain_in_dB + opts.gain;
            case Options::EditMode::None: break;
        }
        BOOST_ASSERT_MSG(false, "Unhandled enum.");
    }();

    if (new_gain_in_dB < -128.0 || new_gain_in_dB >= 127.0) {
        fprintf(stderr, "New gain: %g out of range [-128.0, 127.0).\n", new_gain_in_dB);
        std::exit(EXIT_FAILURE);
    }

    printf("changing gain from %g dB to %g dB\n", cur_gain_in_dB, new_gain_in_dB);
    const auto new_gain_in_q78 = to_q78(new_gain_in_dB);

    write_uint16_le(&full_ogg_page[ogg_header_size + OPUS_GAIN_OFFSET], new_gain_in_q78);
    const auto new_crc = make_crc(full_ogg_page);

    store_uint16(fp, ogg_header_size + OPUS_GAIN_OFFSET, new_gain_in_q78, "store OPUS gain");
    store_uint32(fp, OGG_CRC_OFFSET, new_crc, "store OGG crc");
}
