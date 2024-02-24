#pragma once
#include "global.h"

enum class MessageType : char
{
    invalid,
    ping,
    name,
    disconnect,
    gameStart
};
