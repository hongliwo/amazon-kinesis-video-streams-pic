/*
 * Copyright 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include <stdlib.h>

/* Public headers */
#include "pool_allocator.h"

/* Internal headers */
#include "allocator.h"

#define USE_POOL_FORCE 1

void *myMalloc(size_t bytes)
{
	printf("=======> malloc:%d, %s:%d\n", bytes, __func__, __LINE__);
#ifdef USE_POOL_FORCE
    return poolAllocatorMalloc(bytes);
#else
	return malloc(bytes);
#endif
}

void *myRealloc(void *ptr, size_t bytes)
{
	printf("=======> realloc:%d, %s:%d\n", bytes, __func__, __LINE__);
#ifdef USE_POOL_FORCE
    return poolAllocatorRealloc(ptr, bytes);
#else
	return realloc(ptr, bytes);
#endif

}

void *myCalloc(size_t num, size_t bytes)
{
	printf("=======> realloc:%d, %s:%d\n", bytes, __func__, __LINE__);
#ifdef USE_POOL_FORCE
    return poolAllocatorCalloc(num, bytes);
#else
    return calloc(num, bytes);
#endif

	//void *p = calloc(num, bytes);
	//printf("=======> calloc:%p, sizeof(p):%p, %d:%d, %s:%d\n", p, sizeof(p), num, bytes, __func__, __LINE__);
	//return p;
}

void myFree(void *ptr)
{
	printf("=======> free:%s:%d\n", __func__, __LINE__);
#ifdef USE_POOL_FORCE
    poolAllocatorFree(ptr);
#else
	free(ptr);
#endif
}

/**
 * Wrapper of KVS malloc that use pool allocator malloc.
 *
 * @param[in] bytes Memory size
 * @return New allocated address on success, NULL otherwise
 */
void *__wrap_myMalloc(size_t bytes)
{
	printf("=======> poolAllocatorMalloc:%d, %s:%d\n", bytes, __func__, __LINE__);
    return poolAllocatorMalloc(bytes);
}

/**
 * Wrapper of KVS realloc that use pool allocator realloc.
 *
 * @param[in] ptr Pointer to be re-allocated
 * @param[in] bytes New memory size
 * @return New allocated address on success, NULL otherwise
 */
void *__wrap_myRealloc(void *ptr, size_t bytes)
{
	printf("=======> poolAllocatorRealloc:%d, %s:%d\n", bytes, __func__, __LINE__);
    return poolAllocatorRealloc(ptr, bytes);
}

/**
 * Wrapper of KVS calloc that use pool allocator calloc.
 *
 * @param[in] num Number of elements
 * @param[in] bytes Element size
 * @return Newly allocated address on success, NULL otherwise
 */
void *__wrap_myCalloc(size_t num, size_t bytes)
{
	printf("=======> poolAllocatorCalloc:%d:%d, %s:%d\n", num, bytes, __func__, __LINE__);
    return poolAllocatorCalloc(num, bytes);
}

/**
 * Wrapper of KVS free that use pool allocator free.
 *
 * @param[in] ptr Memory pointer to be freed
 */
void __wrap_myFree(void *ptr)
{
	printf("=======> poolAllocatorFree:%s:%d\n", __func__, __LINE__);
    poolAllocatorFree(ptr);
}
