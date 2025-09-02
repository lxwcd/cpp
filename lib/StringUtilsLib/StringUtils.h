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

// ȥ���ַ������˵Ŀհ��ַ�
void trim(std::string &str);
void trimLeft(std::string &str);
void trimRight(std::string &str);

// ���ַ������ָ����ָ�
std::vector<std::string> split(const std::string &str, const std::string &delimiter);

// ����ַ����Ƿ����ض��Ӵ���ͷ
bool startsWith(const std::string &str, const std::string &prefix);

// ����ַ����Ƿ����ض��Ӵ���β
bool endsWith(const std::string &str, const std::string &suffix);

// ���ַ���ת��ΪСд
std::string toLower(const std::string &str);

// ���ַ���ת��Ϊ��д
std::string toUpper(const std::string &str);

// ������ֵת��Ϊ�ַ���
std::string boolToString(bool value);

// ����ַ����Ƿ�Ϊ�ջ�������հ��ַ�
bool isBlank(const std::string &str);

// ���ַ����е��Ӵ��滻Ϊ��һ���Ӵ�
std::string replace(const std::string &str, const std::string &from, const std::string &to);

}  // namespace StringUtils
