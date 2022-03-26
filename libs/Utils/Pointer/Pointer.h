#pragma once

// a simple smart pointer implementation
template<typename T>
class Pointer
{
private:
    T* ptr = nullptr;

public:
    explicit Pointer(T* ptr) : ptr(ptr) { }

    ~Pointer() { delete ptr; }

    T& operator*() const { return *ptr; }

    T* operator->() const { return ptr; }
};