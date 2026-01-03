
#include "exchange/util/MonitorResource.h"
#include <memory_resource>
namespace exchange::util {

MonitorResource::MonitorResource(std::pmr::memory_resource *parent)
    : parent_(parent) {}

int32_t MonitorResource::getbytesalloc() { return bytesAlloc_; }

void *MonitorResource::do_allocate(size_t bytes, size_t align) {
  bytesAlloc_ += bytes;
  return parent_->allocate(bytes, align);
}
void MonitorResource::do_deallocate(void *p, size_t bytes, size_t align) {
  bytesAlloc_ -= bytes;
  return parent_->deallocate(p, bytes, align);
}
bool MonitorResource::do_is_equal(
    std::pmr::memory_resource const &other) const noexcept {
  return parent_->is_equal(other);
}
}; // namespace exchange::util
