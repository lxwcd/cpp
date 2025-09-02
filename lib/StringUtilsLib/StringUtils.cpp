#include "StringUtils.h"

#include <cctype>

namespace StringUtils
{

void trimLeft(std::string &str)
{
    auto it =
        std::find_if(str.begin(), str.end(), [](char c)
                     { return !std::isspace(static_cast<unsigned char>(c)); });

    str.erase(str.begin(), it);
}

void trimRight(std::string &str)
{
    auto it =
        std::find_if(str.rbegin(), str.rend(), [](char c)
                     { return !std::isspace(static_cast<unsigned char>(c)); })
            .base();

    str.erase(it, str.end());
}

void trim(std::string &str)
{
    trimLeft(str);
    trimRight(str);
}

std::vector<std::string> split(const std::string &str,
                               const std::string &delimiter)
{
    std::vector<std::string> tokens = {};
    if (tokens.empty()) return tokens;

    size_t start = 0;
    size_t end   = str.find(delimiter);

    while (end != std::string::npos)
    {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end   = str.find(delimiter, start);
    }

    tokens.push_back(str.substr(start));
    return tokens;
}

bool startsWith(const std::string &str, const std::string &prefix)
{
    if (prefix.length() > str.length()) return false;
    return str.compare(0, prefix.length(), prefix) == 0;
}

bool endsWith(const std::string &str, const std::string &suffix)
{
    if (suffix.length() > str.length()) return false;
    return str.compare(str.length() - suffix.length(), suffix.length(),
                       suffix) == 0;
}

std::string toLower(const std::string &str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::string toUpper(const std::string &str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return result;
}

std::string boolToString(bool value) { return value ? "true" : "false"; }

bool isBlank(const std::string &str)
{
    std::string s = str;
    trim(s);

    return s.empty();
}

std::string replace(const std::string &str, const std::string &from,
                    const std::string &to)
{
    if (from.empty()) return str;

    std::string result = str;
    size_t start_pos   = 0;
    while ((start_pos = result.find(from, start_pos)) != std::string::npos)
    {
        result.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return result;
}

}  // namespace StringUtils