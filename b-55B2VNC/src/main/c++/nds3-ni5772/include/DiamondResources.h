#ifndef DIAMOND_RESOURCES_H
#define DIAMOND_RESOURCES_H

#include <cstdint>

namespace diamond {

// Resource identifiers come exclusively from the matching exported NI header.
enum class Kind {
    ControlBool, ControlI16, ControlI32, ControlU8, ControlU16, ControlU32,
    IndicatorBool, IndicatorI32, IndicatorU32, IndicatorU64,
    FifoI16, FifoI64, FifoU32, FifoU64
};

struct Resource {
    const char* name;
    Kind kind;
    std::uint32_t id;
};

} // namespace diamond

#endif
