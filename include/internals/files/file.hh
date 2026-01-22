#pragma once
#include <optional>
#include "../misc/data.hh"

std::optional<OilError>
oil_comp(FILE* in, FILE* out, bool debug);

std::optional<OilError>
oil_decomp(FILE* in, FILE* out, bool debug);
