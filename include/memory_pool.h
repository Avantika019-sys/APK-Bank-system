#pragma once
#include <memory_resource>
#include <vector>
#include <unordered_map>
#include <iostream>

namespace banking::pmr {
    
// Custom memory resource for tracking allocations
class TrackingMemoryResource : public std::pmr::memory_resource {
private:
    std::pmr::memory_resource* upstream_;
    size_t allocated_ = 0;
    size_t deallocated_ = 0;
    
public:
    explicit TrackingMemoryResource(std::pmr::memory_resource* upstream = 
        std::pmr::get_default_resource()) 
        : upstream_(upstream) {}
    
    size_t get_allocated() const noexcept { return allocated_; }
    size_t get_deallocated() const noexcept { return deallocated_; }
    size_t get_net_allocations() const noexcept { return allocated_ - deallocated_; }

protected:
    void* do_allocate(size_t bytes, size_t alignment) override {
        void* ptr = upstream_->allocate(bytes, alignment);
        allocated_ += bytes;
        std::cout << "[PMR] Allocated " << bytes << " bytes, total: " << allocated_ << "\n";
        return ptr;
    }
    
    void do_deallocate(void* ptr, size_t bytes, size_t alignment) override {
        upstream_->deallocate(ptr, bytes, alignment);
        deallocated_ += bytes;
        std::cout << "[PMR] Deallocated " << bytes << " bytes\n";
    }
    
    bool do_is_equal(const memory_resource& other) const noexcept override {
        return this == &other;
    }
};

// Memory pool singleton
class MemoryPool {
private:
    inline static std::pmr::unsynchronized_pool_resource pool_{};
    inline static TrackingMemoryResource tracker_{&pool_};
    
public:
    static std::pmr::memory_resource* get_pool() noexcept {
        return &tracker_;
    }
    
    static void print_stats() {
        std::cout << "=== Memory Pool Stats ===\n";
        std::cout << "Net allocations: " << tracker_.get_net_allocations() << " bytes\n";
    }
};

} // namespace banking::pmr