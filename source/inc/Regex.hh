// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot
 * DATE: 2015-03-17
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//

#ifndef REGEX_HH
#define REGEX_HH 1

#include <cstring>
#include <iosfwd>
#include <regex>
#include <string>
#include <vector>

namespace tools
{
class Regex {
public:
    Regex(const char* pattern) { theRegex.assign(pattern); }
    Regex(const std::string pattern) { theRegex.assign(pattern); }

    inline int Match(const char* const str);
    inline const std::vector<std::string>& Split(const char* const str);
    inline void Substitute(const char* str, const char* const replace) {
        str = std::regex_replace(str, theRegex, replace).data();
    }

    inline void Assign(const char* const pattern) {
        theRegex.assign(pattern);
        Clear();
    }
    inline void Clear() {
        theTokens.clear();
        theResult = std::cmatch();
    }
    inline void Reset() {
        Clear();
        theRegex = std::regex();
    }

    inline int Nb() const { return theTokens.size(); }
    inline int NbMatch() const { return theTokens.size(); }
    inline int GetNbMatch() const { return theTokens.size(); }
    inline const std::string& at(uint i) const { return theTokens.at(i); }  // test on i
    inline const std::string& operator()(uint i) const {
        return theTokens.at(i);
    }  // test on i
    inline const std::string& operator[](uint i) const {
        return theTokens[i];
    }  // no test

    // integrated cast to all numeric types
    inline auto stoi(uint i) const {
        return std::stoi(theTokens.at(i), nullptr);
    }  // test on i
    inline auto stol(uint i) const {
        return std::stol(theTokens.at(i), nullptr);
    }  // test on i
    inline auto stoul(uint i) const {
        return std::stoul(theTokens.at(i), nullptr);
    }  // test on i
    inline auto stoll(uint i) const {
        return std::stoll(theTokens.at(i), nullptr);
    }  // test on i
    inline auto stoull(uint i) const {
        return std::stoull(theTokens.at(i), nullptr);
    }  // test on i
    inline auto stof(uint i) const {
        return std::stof(theTokens.at(i), nullptr);
    }  // test on i
    inline auto stod(uint i) const {
        return std::stod(theTokens.at(i), nullptr);
    }  // test on i
    inline auto stold(uint i) const {
        return std::stold(theTokens.at(i), nullptr);
    }  // test on i
    inline const std::vector<std::string>& GetTokens() const { return theTokens; }
    inline const std::vector<std::string>& GetSplitResults() const { return theTokens; }

    inline void PrintTokens(std::ostream& out) const;

private:
    std::regex theRegex;
    std::cmatch theResult;
    std::vector<std::string> theTokens;

public:  // methods with strings
    inline const std::vector<std::string>& Split(const std::string& str);
    inline int Match(const std::string& str) { return Match(str.data()); }
    inline void Assign(const std::string& pattern) {
        theRegex.assign(pattern);
        Clear();
    }
    inline void Substitute(std::string& str, const char* const replace) {
        str = std::regex_replace(str, theRegex, replace);
    }
    inline void Substitute(const char* str, const std::string& replace) {
        Substitute(str, replace.data());
    }
    inline void Substitute(std::string& str, const std::string& replace) {
        Substitute(str, replace.data());
    }
    inline void Sub(const char* str, const char* const replace) {
        Substitute(str, replace);
    }
    inline void Sub(const char* str, const std::string& replace) {
        Substitute(str, replace.data());
    }
    inline void Sub(std::string& str, const char* const replace) {
        Substitute(str, replace);
    }
    inline void Sub(std::string& str, const std::string& replace) {
        Substitute(str, replace.data());
    }
};

inline int Regex::Match(const char* const str) {
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

inline const std::vector<std::string>& Regex::Split(const char* const str) {
    Clear();
    for (std::regex_token_iterator<const char*>
             regit(str, str + strlen(str), theRegex, -1),
         regend;
         regit != regend;
         ++regit)
    {
        theTokens.push_back(regit->str());
    }
    return theTokens;
}

inline const std::vector<std::string>& Regex::Split(const std::string& str) {
    Clear();
    for (std::regex_token_iterator<std::string::const_iterator>
             regit(str.begin(), str.end(), theRegex, -1),
         regend;
         regit != regend;
         ++regit)
    {
        theTokens.push_back(regit->str());
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

}  // namespace tools

#endif
