#pragma once
#ifndef _NONCOPYABLE_12_06_23__
#define _NONCOPYABLE_12_06_23__
namespace non_copyable_
{
    class noncopyable
    {
    public:
        noncopyable(noncopyable const&) = delete;
        noncopyable& operator=(noncopyable const&) = delete;

    protected:
        constexpr noncopyable() = default;
        ~noncopyable() = default;
    };
} // namespace non_copyable_

using noncopyable = non_copyable_::noncopyable;
#endif // _NONCOPYABLE_12_06_23__