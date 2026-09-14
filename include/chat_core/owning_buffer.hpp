#pragma once

#include <cstddef>
#include <cstring>
#include <utility>

// OwningBuffer — учебный класс, владеющий сырым буфером.
// Реализует правило пяти: деструктор, копирующий конструктор,
// копирующее присваивание (через copy-and-swap), перемещающий
// конструктор и перемещающее присваивание. Перемещающие функции
// помечены noexcept.
//
// Copy-and-swap автоматически решает проблему самоприсваивания:
// если this == &other, swap меняет объект сам с собой, а временная
// копия корректно удалится в деструкторе.
namespace chat {


class OwningBuffer {
public:
    OwningBuffer() : data_(nullptr), size_(0) {}

    explicit OwningBuffer(std::size_t size)
        : data_(size > 0 ? new char[size]() : nullptr), size_(size) {}

    // Копирующий конструктор
    OwningBuffer(const OwningBuffer& other)
        : data_(other.size_ > 0 ? new char[other.size_] : nullptr),
          size_(other.size_) {
        if (size_ > 0) {
            std::memcpy(data_, other.data_, size_);
        }
    }

    // Copy-and-swap: аргумент по значению — работает и для copy, и для move
    OwningBuffer& operator=(OwningBuffer other) noexcept {
        swap(*this, other);
        return *this;
    }

    // Перемещающий конструктор
    OwningBuffer(OwningBuffer&& other) noexcept
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    ~OwningBuffer() {
        delete[] data_;
    }

    friend void swap(OwningBuffer& a, OwningBuffer& b) noexcept {
        using std::swap;
        swap(a.data_, b.data_);
        swap(a.size_, b.size_);
    }

    char* data() { return data_; }
    const char* data() const { return data_; }
    std::size_t size() const { return size_; }

private:
    char* data_;
    std::size_t size_;
};

} 