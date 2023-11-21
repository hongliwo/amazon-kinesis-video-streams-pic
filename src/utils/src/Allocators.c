#include "Include_i.h"
#include "pool_allocator.h"
//
// Default allocator functions
//
PVOID defaultMemAlloc(SIZE_T size)
{
#ifdef USE_POOL_ALLOCATOR
	return poolAllocatorMalloc(size);
#else
    return malloc(size);
#endif
}

PVOID defaultMemAlignAlloc(SIZE_T size, SIZE_T alignment)
{
#if defined(__MACH__)
    // On Mac allocations are 16 byte aligned. There is hardly an equivalent anyway
    UNUSED_PARAM(alignment);
    return malloc(size);
#elif defined(_MSC_VER) || defined(__MINGW64__) || defined(__MINGW32__)
    return _aligned_malloc(size, alignment);
#else
    return memalign(size, alignment);
#endif
}

PVOID defaultMemCalloc(SIZE_T num, SIZE_T size)
{
#ifdef USE_POOL_ALLOCATOR
	return poolAllocatorCalloc(num, size);
#else
    return calloc(num, size);
#endif
}

PVOID defaultMemRealloc(PVOID ptr, SIZE_T size)
{
#ifdef USE_POOL_ALLOCATOR
	return poolAllocatorRealloc(ptr, size);
#else
    return realloc(ptr, size);
#endif
}

VOID defaultMemFree(VOID* ptr)
{
#ifdef USE_POOL_ALLOCATOR
	poolAllocatorFree(ptr);
#else
    free(ptr);
#endif
}

memAlloc globalMemAlloc = defaultMemAlloc;
memAlignAlloc globalMemAlignAlloc = defaultMemAlignAlloc;
memCalloc globalMemCalloc = defaultMemCalloc;
memRealloc globalMemRealloc = defaultMemRealloc;
memFree globalMemFree = defaultMemFree;

VOID dumpMemoryHex(PVOID pMem, UINT32 size)
{
#ifdef LOG_STREAMING
    DLOGS("============================================");
    DLOGS("Dumping memory: %p, size: %d", pMem, size);
    DLOGS("++++++++++++++++++++++++++++++++++++++++++++");

    CHAR buf[256];
    PCHAR pCur = buf;
    PBYTE pByte = (PBYTE) pMem;
    for (UINT32 i = 0; i < size; i++) {
        SPRINTF(pCur, "%02x ", *pByte++);
        pCur += 3;
        if ((i + 1) % 16 == 0) {
            DLOGS("%s", buf);
            buf[0] = '\0';
            pCur = buf;
        }
    }

    DLOGS("++++++++++++++++++++++++++++++++++++++++++++");
    DLOGS("Dumping memory done!");
    DLOGS("============================================");
#else
    UNUSED_PARAM(pMem);
    UNUSED_PARAM(size);
#endif
}

BOOL checkBufferValues(PVOID ptr, BYTE val, SIZE_T size)
{
    SIZE_T i;
    PBYTE pBuf = (PBYTE) ptr;

    if (pBuf == NULL) {
        return FALSE;
    }

    for (i = 0; i < size; pBuf++, i++) {
        if (*pBuf != val) {
            return FALSE;
        }
    }

    return TRUE;
}

memChk globalMemChk = checkBufferValues;
