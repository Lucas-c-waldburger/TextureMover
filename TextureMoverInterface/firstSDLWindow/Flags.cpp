#include "Flags.h"

Flags::Flags() : flagValue(0)
{}

bool Flags::isSet(uint8_t flag) { return (flagValue & flag); }

void Flags::setFlag(uint8_t flag) { flagValue |= flag; }

void Flags::unsetFlag(uint8_t flag) { flagValue &= (~flag); }