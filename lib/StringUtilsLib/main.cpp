#include <iostream>
#include "StringUtils.h"

void testIsNumber() {
    std::cout << "Testing isDouble:" << std::endl;

    std::string testCases[] = {
        "+", ".123", "-45.67 ", " +0.5", "1", "12.34.56", "12a.34", " 2"
    };

    for (const auto& test : testCases) {
        std::cout << "'" << test << "' -> "
            << (StringUtils::isNumber<double>(test, true) ? "true" : "false") << std::endl;
    }
    std::cout << std::endl;
}

void testOtherFunctions() {
    std::cout << "Testing other functions:" << std::endl;

    // Test trim
    std::string str = "     Hello World  ";
    std::cout << "before trim: " << str << std::endl;
    StringUtils::trim(str);
    std::cout << "after trim: " << str << std::endl;

    // Test split
    std::string toSplit = "apple,banana,cherry";
    //std::string toSplit = "";
    auto parts = StringUtils::split(toSplit, ",");
    std::cout << "Split: '" << toSplit << "' -> ";
    for (const auto& part : parts) {
        std::cout << "'" << part << "' ";
    }
    std::cout << std::endl;

    // Test startsWith and endsWith
    std::string testStr = "Hello World";
    std::cout << "startsWith 'Hello': " << StringUtils::startsWith(testStr, "Hello") << std::endl;
    std::cout << "endsWith 'World': " << StringUtils::endsWith(testStr, "World") << std::endl;

    // Test case conversion
    std::cout << "toLower: '" << testStr << "' -> '" << StringUtils::toLower(testStr) << "'" << std::endl;
    std::cout << "toUpper: '" << testStr << "' -> '" << StringUtils::toUpper(testStr) << "'" << std::endl;

    // Test replace
    std::string replaceStr = "I like apples and apples are good";
    std::cout << "Replace: '" << replaceStr << "' -> '"
        << StringUtils::replace(replaceStr, "apples", "oranges") << "'" << std::endl;
}

int main() {
    testIsNumber();
    testOtherFunctions();
    return 0;
}