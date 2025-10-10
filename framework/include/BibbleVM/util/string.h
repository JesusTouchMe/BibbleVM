// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_UTIL_STRING_H
#define BIBBLEVM_UTIL_STRING_H 1

#include <string>

namespace bibble::util {
    struct StringHash {
        using is_transparent = void;

        size_t operator()(const std::string& s) const noexcept {
            return std::hash<std::string>()(s);
        }

        size_t operator()(std::string_view s) const noexcept {
            return std::hash<std::string_view>{}(s);
        }
    };

    struct StringEq {
        using is_transparent = void;

        bool operator()(const std::string& a, const std::string& b) const noexcept {
            return a == b;
        }

        bool operator()(std::string_view a, std::string_view b) const noexcept {
            return a == b;
        }

        bool operator()(const std::string& a, std::string_view b) const noexcept {
            return a == b;
        }

        bool operator()(std::string_view a, const std::string& b) const noexcept {
            return a == b;
        }
    };
}

#endif // BIBBLEVM_UTIL_STRING_H
