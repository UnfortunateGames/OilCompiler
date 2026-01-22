#include <optional>
#include <cstdio>
#include <iostream>
#include <internals/files/file.hh>
#include <internals/files/comp.hh>
#include <internals/files/decomp.hh>
#include <internals/misc/data.hh>
#include <internals/misc/debug.hh>

std::optional<OilError>
oil_comp(FILE* in, FILE* out, bool debug) {
    fwrite(
        header,
        HEADER_SIZE,
        1, out
    );
    int previous = EOF;
    int current;
    OilData raw_buffer{
        false,
        0,
        {}
    };
    OilData huff_buffer{
        true,
        0,
        {}
    };
    while (
        (current = fgetc(in)) != EOF
    ) {
        // We always want to see the buffer immediently.
        // So std::endl is used to flush it.
        if (previous == EOF) {
            if (debug)
                std::cout
                    << "First Loop : "
                    << (char) current
                    << std::endl;
            huff_buffer.reset(current);
            raw_buffer.push(previous = current);
        } if (previous == current) {
            if (debug)
                std::cout
                    << "Huff : "
                    << (int) huff_buffer.get_count()
                    << " | Char : "
                    << show_char(current)
                    << std::endl;
            huff_run_checks(
                out,
                &huff_buffer,
                &raw_buffer,
                debug
            );
            previous = current;
            continue;
        }
        raw_run_checks(
            out,
            &huff_buffer,
            &raw_buffer,
            current,
            debug
        );
        previous = current;
    }
    return std::nullopt;
}

std::optional<OilError>
oil_decomp(FILE* in, FILE* out, bool debug) {
    if (
        const auto result
            = validate_header_then_move(in, debug);
        result.has_value()
    ) switch (result.value()) {
        case Version::Future:
            return OilError::FutureOilFile;
        case Version::Old:
            return OilError::OldOilFile;
        case Version::Valid:
            break;
    }
    else return OilError::EOFReached;
    while (true) {
        const auto read = oil_read(in, debug);
        if (read.has_value()) {
            fwrite(
                read.value().c_str(),
                read.value().size(),
                1, out
            );
            continue;
        } if (read.error() == OilError::EOFReached) break;
        return read.error();
    }
    if (debug)
        std::cout
            << "Exitted loop"
            << std::endl;
    return std::nullopt;
}
