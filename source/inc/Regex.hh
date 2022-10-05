// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot
 * DATE: 2015-03-17
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//

#ifndef TOOLS_REGEX_HH
#define TOOLS_REGEX_HH 1

#include <cstring>
#include <iosfwd>
#include <regex>
#include <string>
#include <vector>

namespace tools
{
class Regex {
public:
    explicit Regex(const std::string& pattern): thePattern(pattern) {
        theRegex.assign(pattern);
    }

    inline int Match(const std::string& str);
    inline const std::vector<std::string>& Split(const std::string& str);
    inline void Substitute(std::string& str, const std::string& replace) {
        str = std::regex_replace(str, theRegex, replace);
    }

    inline void Assign(const char* const pattern) {
        theRegex.assign(pattern);
        Clear();
    }
    inline void Clear() {
        theTokens.clear();
        theResult = std::smatch();
    }
    inline void Reset() {
        Clear();
        theRegex = std::regex();
    }

    inline int Nb() const { return theTokens.size(); }
    inline int NbMatch() const { return theTokens.size(); }
    inline int GetNbMatch() const { return theTokens.size(); }
    inline const std::string& at(uint i) const { return theTokens.at(i); }
    inline const std::string& operator()(uint i) const { return theTokens.at(i); }
    inline const std::string& operator[](uint i) const {
        return theTokens[i];
    }  // no test

    // integrated cast to all numeric types
    inline auto stoi(uint i) -> decltype(std::stoi(std::string())) const {
        return std::stoi(theTokens.at(i), nullptr);
    }
    inline auto stol(uint i) -> decltype(std::stol(std::string())) const {
        return std::stol(theTokens.at(i), nullptr);
    }
    inline auto stoul(uint i) -> decltype(std::stoul(std::string())) const {
        return std::stoul(theTokens.at(i), nullptr);
    }
    inline auto stoll(uint i) -> decltype(std::stoll(std::string())) const {
        return std::stoll(theTokens.at(i), nullptr);
    }
    inline auto stoull(uint i) -> decltype(std::stoull(std::string())) const {
        return std::stoull(theTokens.at(i), nullptr);
    }
    inline auto stof(uint i) -> decltype(std::stof(std::string())) const {
        return std::stof(theTokens.at(i), nullptr);
    }
    inline auto stod(uint i) -> decltype(std::stod(std::string())) const {
        return std::stod(theTokens.at(i), nullptr);
    }
    inline auto stold(uint i) -> decltype(std::stold(std::string())) const {
        return std::stold(theTokens.at(i), nullptr);
    }
    inline const std::vector<std::string>& GetTokens() const { return theTokens; }
    inline const std::vector<std::string>& GetSplitResults() const { return theTokens; }

    inline void PrintTokens(std::ostream& out) const;

private:
    std::string thePattern;
    std::regex theRegex;
    std::smatch theResult;
    std::vector<std::string> theTokens;

public:  // methods with strings
    inline void Assign(const std::string& pattern) {
        theRegex.assign(pattern);
        Clear();
    }
    inline void Substitute(std::string& str, const char* const replace) {
        str = std::regex_replace(str, theRegex, std::string(replace));
    }
    inline void Sub(std::string& str, const char* const replace) {
        Substitute(str, std::string(replace));
    }
    inline void Sub(std::string& str, const std::string& replace) {
        Substitute(str, replace);
    }

    friend std::ostream& operator<<(std::ostream& os, const Regex& r);
};

inline int Regex::Match(const std::string& str) {
    Clear();
    if (std::regex_search(str, theResult, theRegex) == false) {
        return 0;
    }
    for (auto& el : theResult) {
        theTokens.push_back(el);
    }
    while (theTokens.empty() == false and theTokens.back().empty() == true) {
        theTokens.pop_back();
    }
    return theTokens.size();
}

inline const std::vector<std::string>& Regex::Split(const std::string& str) {
    Clear();
    using rti = std::regex_token_iterator<std::string::const_iterator>;
    rti begin(str.begin(), str.end(), theRegex, -1);
    for (rti end; begin != end; ++begin) {
        theTokens.push_back(begin->str());
    }
    return theTokens;
}

inline void Regex::PrintTokens(std::ostream& out) const {
    out << "Nb=" << theTokens.size() << ": \t";
    for (auto& el : theTokens) {
        out << "  [" << el << "]";
    }
    out << "\n";
}

std::ostream& operator<<(std::ostream& os, const Regex& r) {
    os << "Pattern=" << r.thePattern << ": \t";
    r.PrintTokens(os);
    return os;
}

}  // namespace tools

#endif
