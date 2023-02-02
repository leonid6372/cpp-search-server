#pragma once

#include <string>
#include <vector>
#include <set>
#include <ostream>
#include "document.h"
#include "paginator.h"

std::vector<std::string> SplitIntoWords(const std::string& text);

template <typename StringContainer>
std::set<std::string> MakeUniqueNonEmptyStrings(const StringContainer& strings) {
    std::set<std::string> non_empty_strings;
    for (const std::string& str : strings) {
        if (!str.empty()) {
            non_empty_strings.insert(str);
        }
    }
    return non_empty_strings;
}

std::ostream& operator<<(std::ostream& os, const Document& document);

template <typename Iterator>
std::ostream& operator<<(std::ostream& os, const IteratorRange<Iterator>& page) {
    for(const auto& document : page){
        os << document;
    }
    return os;
}