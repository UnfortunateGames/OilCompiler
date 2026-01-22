#include <optional>
#include <cstdio>
#include <expected>
#include <string>
#include <iostream>
#include <internals/files/decomp.hh>
#include <internals/misc/data.hh>
#include <internals/misc/debug.hh>

std::optional<Version>
validate_header_then_move(FILE* in, bool debug) {
    if (debug) std::cout << "Validating header" << std::endl;
    int16_t file_read = fgetc(in);
    // Signature check
    // No debug, since main already gives us the error.
    // No need for sanity checks
    if (
        // EOF is -1, therefore if file_read is EOF, EOF != 7
        file_read != 7
        || (file_read = fgetc(in)) != 16
    ) return std::nullopt;
    // Continue to version data
    if (
        (file_read = fgetc(in)) == EOF
    ) return std::nullopt;
    uint8_t version = file_read;
    if (
        (file_read = fgetc(in)) == EOF
    ) return std::nullopt;
    fseek(in, file_read, SEEK_SET);
    if (file_read < header[VERSION_INDEX])
        return Version::Old;
    if (file_read > header[VERSION_INDEX])
        return Version::Future;
    return Version::Valid;
}

std::expected<std::string, OilError>
oil_read(FILE* in, bool debug) {
    if (debug) std::cout << "Reading byte";
    int16_t file_read = fgetc(in);
    if (debug)
        std::cout
            << "Huff Raw byte : "
            << (int) file_read;
    if (file_read == EOF)
        return std::unexpected(
            OilError::EOFReached
        );
    if (file_read > 1)
        return std::unexpected(
            OilError::InvalidSection
        );
    bool huff_raw = file_read == 1;
    if ((file_read = fgetc(in)) == EOF)
        return std::unexpected(
            OilError::EOFReached
        );
    if (debug)
        std::cout
            << "Count : "
            << (int) file_read
            << std::endl;
    if (file_read == 0)
        return std::unexpected(
            OilError::InvalidSection
        );
    uint8_t amount = (uint8_t) file_read;
    std::string out{""};
    if (huff_raw) {
        if (
            (file_read = fgetc(in)) == EOF
        ) return std::unexpected(
            OilError::EOFReached
        );
        if (debug)
            std::cout
                << "Huff Character : "
                << (char) file_read;
        for (uint8_t _{0u}; _ < amount; _++) {
            out += (char) file_read;
        }
        return out;
    }
    if (debug) std::cout << "Raw stream : ";
    for (uint8_t _{0u}; _ < amount; _++) {
        if ((file_read = fgetc(in)) == EOF)
            return std::unexpected(
                OilError::EOFReached
            );
        if (debug) std::cout << show_char(file_read);
        out += show_char(file_read);
    }
    std::cout << std::endl;
    return out;
}
