#pragma once
#include <string>
#include <vector>
#include <utility>
#include <cstdint>

static constexpr uint8_t RAW_BASE = 2;
static constexpr uint8_t HUFF_SIZE = 3;
static constexpr uint8_t VERSION_INDEX = 2;
static constexpr uint8_t HEADER_SIZE = 8;

static const uint8_t header[8]{
    0x07, // Signature No. 1
    0x10, // Signature No. 2 ( 07 10 Oil backwards )
    0x03, // Version No. ( V3 )
    0x08, // Start Position (The end of the header)
    0x00, // Future flags ...
    0x00,
    0x00,
    0x00,
};

enum OilError {
    EOFReached,     // A success, rather than an error.
    InvalidHeader,  // Corruption check first
    InvalidSection, // Another corruption check
    InvalidOilData, // The program's fault, or mine
    OldOilFile,     // Old programs may not have future optimizations or flags
    FutureOilFile,  // Future files may have flags unknown or optimizations.
};

// Header check to return

enum Version {
    Old, Valid, Future
};

/*
 * OilData is a staging object.
 *
 * huff_raw determines compilation method:
 * true - Huffman compressing
 * (AAAABB - 4A2B)
 * false -> Raw Data
 * (ABCDEF - ABCDEF unchanged)
 * 
 * Much like the old Pokemon sprite compressing
 * Thus the name 'OIL' representing their black and white pallete.
 * 
 * OilData is staging object that is passed over functions to mutate.
 * Therefore it is a move only class object.
 * Empty/Flushed -> Staging -> Completed -> Flushed
 */

class OilData {
    public:
        explicit OilData(
            const bool huff_raw,
            const uint8_t amount,
            const std::vector<char> chr
        )
            : huff_raw(huff_raw),
            count(amount),
            buffer(chr) {}
        ~OilData() = default;
        OilData(OilData&& other) noexcept
            : huff_raw(other.huff_raw),
            count(
                std::exchange(other.count, 0)
            ),
            buffer(
                std::move(other.buffer)
            ) {}
        // Precautions
        OilData(const OilData&) = delete;
        OilData& operator=(const OilData&) = delete;
        OilData& operator=(OilData&&) = delete;
        // Setters
        // Raw Buffer Control
        void push(const char in) {
            this->buffer.push_back(in);
            this->count++;
        }
        void revert() {
            for (uint8_t _{0u}; _ < 3; _++) {
                this->buffer.pop_back();
            }
            this->count -= 3;
        }
        void clear() {
            this->buffer.clear();
            this->count = 0;
        }
        // Huff buffer control
        void incr() {
            this->count++;
        }
        void reset(const char in) {
            this->clear();
            this->push(in);
        }
        // Getters
        bool validate() {
            if (
                this->count != buffer.size()
                || this->count == 0
            ) return false;
            return true;
        }
        bool get_huff_raw() {
            return this->huff_raw;
        }
        uint8_t get_count() {
            return this->count;
        }
        std::vector<char> get_buffer() {
            return this->buffer;
        }
        // Writing to file
        std::vector<uint8_t> data() {
            std::vector<uint8_t> out;
            if (this->huff_raw) {
                out.push_back(1u);
                out.push_back(this->count);
                out.push_back((uint8_t) this->buffer[0]);
                return out;
            }
            out.push_back(0u);
            out.push_back(this->count);
            for (const char iter : this->buffer) {
                out.push_back((uint8_t) iter);
            }
            return out;
        }
    private:
        const bool huff_raw;
        uint8_t count = 0;
        std::vector<char> buffer{};
};
