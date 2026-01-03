

#include <cstddef>
#include <cstdint>
#include <memory_resource>
#ifndef EXCHANGE_SNAPSHOTTRIGGER_H
#define EXCHANGE_SNAPSHOTTRIGGER_H
class MonitorResource : public std::pmr::memory_resource {
public:
  MonitorResource(
      std::pmr::memory_resource *parent = std::pmr::get_default_resource())
      : parent_(parent) {}

  auto getbytesalloc() { return bytesAlloc_; }

private:
  int32_t bytesAlloc_{};
  std::pmr::memory_resource *parent_;
  void *do_allocate(size_t bytes, size_t align) override {
    bytesAlloc_ += bytes;
    return parent_->allocate(bytes, align);
  }
  void do_deallocate(void *p, size_t bytes, size_t align) override {
    bytesAlloc_ -= bytes;
    return parent_->deallocate(p, bytes, align);
  }
  bool
  do_is_equal(std::pmr::memory_resource const &other) const noexcept override {
    return parent_->is_equal(other);
  }
};
#endif //
