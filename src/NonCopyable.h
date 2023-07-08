#pragma once

#ifndef NON_COPYABLE_H_INCLUDED
#define NON_COPYABLE_H_INCLUDED

class NonCopyable {

protected:
    NonCopyable() = default;
    ~NonCopyable() = default;

public:
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable &operator=(const NonCopyable &) = delete;
};

#endif
