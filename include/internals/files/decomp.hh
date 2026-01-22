#pragma once
#include <optional>
#include <cstdio>
#include <expected>
#include "../misc/data.hh"

std::optional<Version>
validate_header_then_move(FILE* in, bool debug);

std::expected<std::string, OilError>
oil_read(FILE* in, bool debug);
