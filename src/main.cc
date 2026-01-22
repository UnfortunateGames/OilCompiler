#include <iostream>
#include <cstdio>
#include <cstring>
#include <oil.hh>

int main(int argc, char *argv[]) {
    FILE* in = nullptr;
    FILE* out = nullptr;
    bool compress = true;
    bool debug = false;
    std::optional<std::string>
        input_name = std::nullopt;
    std::optional<std::string>
        output_name = std::nullopt;

    for (int i{1u}; i < argc; i++) {
        std::string argument = argv[i];
        if (
            argument == "-o"
            || argument == "--output"
        ) {
            if (i + 1 >= argc) {
                std::cerr << "Expected output after -o/--output.";
                return 2;
            }
            if (!output_name)
                output_name = argv[++i];
            else {
                std::cerr << "2 outputs given, please give only one.";
                return 2;
            }
            continue;
        } if (
            argument == "-d"
            || argument == "--decompress"
        ) {
            compress = false;
            continue;
        } if (
            argument == "-D"
            || argument == "--debug"
        ) {
            debug = true;
            continue;
        } if (input_name) {
            std::cerr << "2 input files given, please give only one.";
            return 2;
        }
        input_name = argument;
    }

    if (!input_name) {
        std::cerr << "Input file not given.";
        return 2;
    } if (!output_name) {
        output_name = input_name.value();
        if (compress) output_name.value() += ".oil";
        else output_name.value() += ".out";
    }
    out = fopen(
        output_name
            .value()
            .c_str(),
        "wb"
    );
    in = fopen(
        input_name
            .value()
            .c_str(),
        "rb"
    );

    if (!in) {
        std::cerr << "Input file given does not exist.";
        return 1;
    }

    const std::optional<OilError> result
        = compress ?
            oil_comp(in, out, debug)
            : oil_decomp(in, out, debug);

    if (result.has_value()) {
        switch (result.value()) {
            case OilError::EOFReached:
                break;
            case OilError::InvalidSection:
                std::cerr
                    << "File may be corrupt. Check the .oil file, or .out file if needed.";
                break;
            case OilError::InvalidOilData:
                std::cerr
                    << "Program failed to parse OilData. Please report this bug to us.";
                break;
            case OilError::InvalidHeader:
                std::cerr
                    << "File may not be an actual .oil file.";
                break;
            case OilError::OldOilFile:
                // For future implementation
                std::cerr
                    << "Oil file is old.\n"
                    << "Try -F/--force to force new decompression\n"
                    << "Or preferrable -L/--legacy for legacy decompression.";
                break;
            case OilError::FutureOilFile:
                std::cerr
                    << "Oil file is from the future.\n"
                    << "Try -F/--force to force this version's decompression.";
                break;
        }
    }

    if (in) fclose(in);
    if (out) fclose(out);
    in = nullptr;
    out = nullptr;
    return 0;
}
