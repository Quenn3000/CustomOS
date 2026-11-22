#include <memory_management.hpp>

#include "test_suite.hpp"

void test_memory() {
    test_begin("memory/allocator");

    MemoryManager& manager = MemoryManager::Instance();

    void* first = manager.malloc();
    void* second = manager.malloc();

    CHECK(first != NULL);
    CHECK(second != NULL);
    CHECK((uint32_t)second - (uint32_t)first == MEMORY_PROCESS_BLOCK_SIZE);
    CHECK(manager.free(first));

    void* reused = manager.malloc();
    CHECK_INT((uint32_t)reused, (uint32_t)first);

    manager.free(second);
    manager.free(reused);

    test_end();
}
