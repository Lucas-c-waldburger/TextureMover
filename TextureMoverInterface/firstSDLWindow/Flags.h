#pragma once
#include <iostream>

struct Flags
{
    Flags();

    uint8_t flagValue;

    bool isSet(uint8_t flag);
    void setFlag(uint8_t flag);
    void unsetFlag(uint8_t flag);
};

enum GAME_FLAGS
{
    QUIT = 1 << 0
};