//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#if !defined(__EZALLOCA_H__)
#define __EZALLOCA_H__

#define HEAP_ALLOC_THRESHOLD 1024

extern "C" {
    _ELASTOS PCarQuintet __cdecl _CarQuintet_Init(_ELASTOS PCarQuintet pCq,
            _ELASTOS PVoid pBuf, _ELASTOS Int32 size, _ELASTOS Int32 used,
            _ELASTOS CarQuintetFlags flags);
}

_ELASTOS_NAMESPACE_BEGIN

#define CARARRAY_SIZE(type, n) \
    (sizeof(_ELASTOS CarQuintet) + sizeof(type) * (n))

CAR_INLINE PCarQuintet _Auto_Init(PVoid pv, Int32 size, Int32 used,
        CarQuintetFlags flags)
{
    PCarQuintet pCq = (PCarQuintet)pv;

    if (pCq) {
        _CarQuintet_Init(pCq, pCq + 1, size, used, flags);
    }

    return pCq;
}

#ifdef _RELEASE

#define AUTO_ARRAYOF(type, n) (ArrayOf<type> *)(\
    (UInt32)n <= HEAP_ALLOC_THRESHOLD / sizeof(type) \
    ? _Auto_Init(_alloca(CARARRAY_SIZE(type, n)), \
        sizeof(type) * (n), sizeof(type) * (n), Type2Flag<type>::Flag()) \
    : _Auto_Init(malloc(CARARRAY_SIZE(type, n)), \
        sizeof(type) * (n), sizeof(type) * (n), \
        Type2Flag<type>::Flag() | CarQuintetFlag_HeapAlloced))

#else //_RELEASE

#define AUTO_ARRAYOF(type, n) (ArrayOf<type> *)(\
    _Auto_Init(malloc(CARARRAY_SIZE(type, n)), \
        sizeof(type) * (n), sizeof(type) * (n), \
        Type2Flag<type>::Flag() | CarQuintetFlag_HeapAlloced))

#endif //_RELEASE

_ELASTOS_NAMESPACE_END

#endif //__EZALLOCA_H__
