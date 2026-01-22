#include <vector>
#include <string>
#include <internals/misc/debug.hh>

std::string show_char(int chr) {
    if (chr == '\n') return "\\n";
    if (chr == '\r') return "\\r";
    if (chr == '\t') return "\\t";
    std::string out = "";
    return out += chr;
}
