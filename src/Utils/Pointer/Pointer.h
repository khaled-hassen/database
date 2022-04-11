#pragma once

#include <iostream>

// a simple unique smart pointer implementation
template<typename T>
class Pointer
{
private:
    T* m_Ptr = nullptr;

private:
    // free memory allocated by the pointer
    void Destroy() { delete m_Ptr; }

public:
    // create an empty Pointer<T> object
    static Pointer<T> GetNull() { return Pointer(); }

    Pointer() : m_Ptr(nullptr) { }

    explicit Pointer(T* ptr) : m_Ptr(ptr) { }

    ~Pointer() { Destroy(); }

    T& operator*() const { return *m_Ptr; }

    T* operator->() const { return m_Ptr; }

    bool operator==(T* _ptr) const { return m_Ptr == _ptr; }

    Pointer(const Pointer<T>& obj) = delete; // copy constructor is deleted
    Pointer<T>& operator=(const Pointer<T>& obj) = delete; // copy assignment is deleted

    // move assignment
    Pointer<T>& operator=(Pointer<T>&& obj) noexcept
    {
        Destroy(); // cleanup any existing data
        // Transfer ownership of the memory from obj to this object
        m_Ptr = obj.m_Ptr;
        obj.m_Ptr = nullptr;
        return *this;
    }
};