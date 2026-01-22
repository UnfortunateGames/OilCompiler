#pragma once
#include <optional>
#include <cstdio>
#include "../misc/data.hh"

std::optional<OilError>
raw_write(FILE* out, OilData* raw, bool revert, bool debug);

std::optional<OilError>
huff_write(FILE* out, OilData* huff, OilData* raw, bool debug);

std::optional<OilError>
huff_run_checks(FILE* out, OilData* huff, OilData* raw, bool debug);

std::optional<OilError>
raw_run_checks(FILE* out, OilData* huff, OilData* raw, int chr, bool debug);
