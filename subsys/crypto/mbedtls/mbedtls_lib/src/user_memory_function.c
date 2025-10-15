#include "user_memory_function.h"

void *(*mbedtls_malloc)(uint32_t size) = malloc;
void (*mbedtls_free)(void *ptr) = free;
