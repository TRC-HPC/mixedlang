#include "textcpp.h"
#include <iostream>

Text::Text(const std::string& prefix) :
    _prefix(prefix)
{
    std::cout << "In c'tor." << std::endl;
}

Text::~Text() {
    std::cout << "In d'tor." << std::endl;
}

void Text::print(const std::string& t) {
    std::cout << _prefix << ": " << t << std::endl;
}

