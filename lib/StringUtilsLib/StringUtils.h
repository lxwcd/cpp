#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

namespace StringUtils
{

// check if the string is number
template <typename T>
bool isNumber(const std::string &str, bool isStrict = false)
{
    if (str.empty()) return false;

    if (isStrict &&
        std::any_of(str.begin(), str.end(), [](char c) { return std::isspace(static_cast<unsigned char>(c)); }))
    {
        return false;
    }

    std::istringstream iss(str);
    T num;

    return (iss >> num) && iss.eof();
}

// 去除字符串两端的空白字符
void trim(std::string &str);
void trimLeft(std::string &str);
void trimRight(std::string &str);

// 将字符串按分隔符分割
std::vector<std::string> split(const std::string &str, const std::string &delimiter);

// 检查字符串是否以特定子串开头
bool startsWith(const std::string &str, const std::string &prefix);

// 检查字符串是否以特定子串结尾
bool endsWith(const std::string &str, const std::string &suffix);

// 将字符串转换为小写
std::string toLower(const std::string &str);

// 将字符串转换为大写
std::string toUpper(const std::string &str);

// 将布尔值转换为字符串
std::string boolToString(bool value);

// 检查字符串是否为空或仅包含空白字符
bool isBlank(const std::string &str);

// 将字符串中的子串替换为另一个子串
std::string replace(const std::string &str, const std::string &from, const std::string &to);

}  // namespace StringUtils
