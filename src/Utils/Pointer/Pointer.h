#pragma once

#include <iostream>

// a simple smart pointer implementation
template<typename T>
class Pointer
{
private:
    T* ptr = nullptr;

private:
    // free memory allocated by the pointer
    void Destroy() { delete ptr; }

public:
    // create an empty Pointer<T> object
    static Pointer<T> GetNull() { return Pointer(); }

    Pointer() : ptr(nullptr) { }

    explicit Pointer(T* ptr) : ptr(ptr) { }

    ~Pointer() { Destroy(); }

    T& operator*() const { return *ptr; }

    T* operator->() const { return ptr; }

    bool operator==(T* _ptr) const { return ptr == _ptr; }

    Pointer(const Pointer<T>& obj) = delete; // copy constructor is deleted
    Pointer<T>& operator=(const Pointer<T>& obj) = delete; // copy assignment is deleted

    // move assignment
    Pointer<T>& operator=(Pointer<T>&& obj) noexcept
    {
        Destroy(); // cleanup any existing data
        // Transfer ownership of the memory from obj to this object
        ptr = obj.ptr;
        obj.ptr = nullptr;
        return *this;
    }
};