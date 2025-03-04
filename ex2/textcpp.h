#pragma once

#include <string>

class Text {
    private:
        const std::string _prefix;

    public:
        Text(const std::string& prefix);
        ~Text();

        void print(const std::string& t);
};

