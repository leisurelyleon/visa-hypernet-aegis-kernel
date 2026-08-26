#pragma once
#include <atomic>
#include <cstddef>
#include <vector>
#include <stdexcept>

namespace visa::aegis {

template<typename T, size_t Capacity>
class LockFreeQueue {
private:
    static_assert((Capacity != 0) && ((Capacity & (Capacity - 1)) == 0), "Capacity must be a power of 2");

    alignas(64) T buffer_[Capacity];
    alignas(64) std::atomic<size_t> head_{0};
    alignas(64) std::atomic<size_t> tail_{0};

    // Mask for fast modulo arithmetic
    static constexpr size_t Mask = Capacity - 1;

public:
    LockFreeQueue() = default;

    // Called by the Producer (e.g., Network Ingress Thread)
    bool Push(const T& item) {
        size_t current_tail = tail_.load(std::memory_order_relaxed);
        size_t next_tail = (current_tail + 1) & Mask;

        if (next_tail == head_.load(std::memory_order_acquire)) {
            return false; // Queue is full
        }

        buffer_[current_tail] = item;
        tail_.store(next_tail, std::memory_order_release);
        return true;
    }

    // Called by the Consumer (e.g., Routing Logic Thread)
    bool Pop(T& out_item) {
        size_t current_head = head_.load(std::memory_order_relaxed);

        if (current_head == tail_.load(std::memory_order_acquire)) {
            return false; // Queue is empty
        }

        out_item = buffer_[current_head];
        head_.store((current_head + 1) & Mask, std::memory_order_release);
        return true;
    }
};

} // namespace visa::aegis
