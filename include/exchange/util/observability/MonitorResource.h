

#include <cstddef>
#include <cstdint>
#include <memory_resource>
#ifndef EXCHANGE_UTIL_OBSERVABILITY_MONITORRESOURCE_H
#define EXCHANGE_UTIL_OBSERVABILITY_MONITORRESOURCE_H
namespace exchange::util::observability {
class MonitorResource : public std::pmr::memory_resource {
public:
  MonitorResource(
      std::pmr::memory_resource *parent = std::pmr::get_default_resource());

  int32_t getbytesalloc() const;

private:
  int32_t bytesAlloc_{};
  std::pmr::memory_resource *parent_;
  void *do_allocate(size_t bytes, size_t align) override;
  void do_deallocate(void *p, size_t bytes, size_t align) override;
  bool
  do_is_equal(std::pmr::memory_resource const &other) const noexcept override;
};
} // namespace exchange::util::observability
#endif // EXCHANGE_UTIL_OBSERVABILITY_MONITORRESOURCE_H
