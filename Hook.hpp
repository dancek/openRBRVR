#pragma once

#include <MinHook.h>
#include <stdexcept>

// RAII wrapper for MinHook
template <typename T>
struct Hook {
    T call;
    T src;

    explicit Hook()
        : call(nullptr)
        , src(nullptr)
    {
    }

    explicit Hook(T src, T tgt)
        : src(src)
    {
        if (MH_CreateHook(reinterpret_cast<void*>(src), reinterpret_cast<void*>(tgt), reinterpret_cast<void**>(&call)) != MH_OK) {
            throw std::exception("Could not hook");
        }
        Enable();
    }
    void Enable()
    {
        if (MH_EnableHook(src) != MH_OK) {
            throw std::exception("Could not disable hook");
        }
    }
    void Disable()
    {
        if (MH_DisableHook(src) != MH_OK) {
            throw std::exception("Could not disable hook");
        }
    }
    Hook(const Hook&) = delete;
    Hook(Hook&& rhs)
    {
        call = rhs.call;
        src = rhs.src;
        rhs.call = nullptr;
        rhs.src = nullptr;
        return *this;
    }
    Hook& operator=(const Hook&) = delete;
    Hook& operator=(Hook&& rhs) noexcept
    {
        call = rhs.call;
        src = rhs.src;
        rhs.call = nullptr;
        rhs.src = nullptr;
        return *this;
    }
    ~Hook()
    {
        if (src)
            MH_DisableHook(src);
    }
};

template <typename T, typename F>
T* GetVtable(F* obj)
{
    return (T*)(*(uintptr_t*)obj);
}
