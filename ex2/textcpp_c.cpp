#include "textcpp.h"

extern "C" {
    void* init_text(const char* prefix) {
        if (!prefix)
            return NULL;

        return static_cast<void*>(new Text(prefix));
    }

    int finalize_text(void* pText) {
        Text* text = reinterpret_cast<Text*>(pText);
        if (!text)
            return -1;

        delete text;
        return 0;
    }

    int print(void* pText, const char* t) {
        Text* text = reinterpret_cast<Text*>(pText);
        if (!text)
            return -1;

        if (!t)
            return -1;

        text->print(t);
        return 0;
    }
}

