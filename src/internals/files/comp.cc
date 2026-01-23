#include <optional>
#include <iostream>
#include <internals/files/comp.hh>
#include <internals/misc/data.hh>
#include <internals/misc/debug.hh>

std::optional<OilError>
raw_write(
    FILE* out, // Checked for in main.cc.
    OilData* raw, // raw's huff_raw must be false.
    bool revert,
    bool debug
) {
    // We always want the debug print to flush immediently.
    // As if a bug airs, without a std::endl it wont appear.
    if (debug)
        std::cout
            << "Raw flush called : "
            << (int) raw->get_count()
            << "\nValidating raw buffer"
            << std::endl;
    if (!raw->validate()) {
        // No need to assess the error, as main already tells us.
        // Debug, or not.
        if (debug)
            std::cout
                << "Count : "
                << (int) raw->get_count()
                << " | Actual Size : "
                << raw->get_buffer().size()
                << std::endl;
        return OilError::InvalidOilData;
    }
    if (revert) raw->revert();
    fwrite(
        raw->data().data(),
        RAW_BASE + raw->get_count(),
        1, out
    );
    raw->clear();
    if (debug) std::cout << "Raw buffer flushed" << std::endl;
    return std::nullopt;
}

std::optional<OilError>
huff_write(
    FILE* out,
    OilData* huff,
    OilData* raw, // If it is not empty
    bool debug
) {
    if (debug)
        std::cout
            << "Huff flush called : "
            << (int) huff->get_count()
            << " | Char : "
            << huff->get_buffer()[0]
            << std::endl;
    if (raw->get_count() > 3) {
        if (
            const auto result
                = raw_write(
                    out,
                    raw,
                    true,
                    debug
                );
            result.has_value()
        ) return result.value();
    } else raw->clear();
    fwrite(
        huff->data().data(),
        HUFF_SIZE,
        1, out
    );
    return std::nullopt;
}

std::optional<OilError>
huff_run_checks(
    FILE* out,
    OilData* huff,
    OilData* raw,
    bool debug
) {
    if (
        huff->get_count() >= HUFF_SIZE
        && raw->get_count() > 3
    ) if (
        const auto result
            = raw_write(
                out,
                raw,
                true,
                debug
            );
        result.has_value()
    ) return result.value();
    if (huff->get_count() < HUFF_SIZE) {
        if (raw->get_count() == 255) if (
            const auto result
                = raw_write(
                    out,
                    raw,
                    false,
                    debug
                );
            result.has_value()
        ) return result.value();
        raw->push(huff->get_buffer()[0]);
    } else raw->clear();
    huff->incr();
    return std::nullopt;
}

std::optional<OilError>
raw_run_checks(
    FILE* out,
    OilData* huff,
    OilData* raw,
    int chr,
    bool debug
) {
    if (debug)
        std::cout
            << "Raw : "
            << (int) raw->get_count()
            << " | Char : "
            << show_char(chr)
            << std::endl;
    if (huff->get_count() >= HUFF_SIZE) if (
        const auto result
            = huff_write(
                out,
                huff,
                raw,
                debug
            );
        result.has_value()
    ) return result.value();
    if (raw->get_count() == 255) if (
        const auto result
            = raw_write(
                out,
                raw,
                false,
                debug
            );
        result.has_value()
    ) return result.value();
    raw->push(chr);
    huff->reset(chr);
    return std::nullopt;
}
