#ifndef HALOGEN_LIBRARIES_COMMON_CLASSMACROS_HPP
#define HALOGEN_LIBRARIES_COMMON_CLASSMACROS_HPP

#include <memory>  // IWYU pragma: keep

#define HALOGEN_SMART_POINTERS(CLASSNAME)                                                                         \
    using uPtr = std::unique_ptr<CLASSNAME>;                                                                      \
    using sPtr = std::shared_ptr<CLASSNAME>

#define HALOGEN_CLASS_COPY_DEFAULT(CLASSNAME)                                                                     \
    CLASSNAME(const CLASSNAME&) = default;                                                                        \
    CLASSNAME& operator=(const CLASSNAME&) = default

#define HALOGEN_CLASS_CONSTEXPR_COPY_DEFAULT(CLASSNAME)                                                           \
    constexpr CLASSNAME(const CLASSNAME&) = default;                                                              \
    constexpr CLASSNAME& operator=(const CLASSNAME&) = default

#define HALOGEN_CLASS_MOVE_DEFAULT(CLASSNAME)                                                                     \
    CLASSNAME(CLASSNAME&&) = default;                                                                             \
    CLASSNAME& operator=(CLASSNAME&&) = default

#define HALOGEN_CLASS_CONSTEXPR_MOVE_DEFAULT(CLASSNAME)                                                           \
    constexpr CLASSNAME(CLASSNAME&&) = default;                                                                   \
    constexpr CLASSNAME& operator=(CLASSNAME&&) = default

#define HALOGEN_CLASS_COPY_DELETE(CLASSNAME)                                                                      \
    CLASSNAME(const CLASSNAME&) = delete;                                                                         \
    CLASSNAME& operator=(const CLASSNAME&) = delete

#define HALOGEN_CLASS_MOVE_DELETE(CLASSNAME)                                                                      \
    CLASSNAME(CLASSNAME&&) = delete;                                                                              \
    CLASSNAME& operator=(CLASSNAME&&) = delete

#endif  // HALOGEN_LIBRARIES_COMMON_CLASSMACROS_HPP