//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <eltypes.h>

extern "C" {

_ELASTOS_NAMESPACE_USING

static CarQuintet s_nullCq[] = {
    {0, 0, 0, 0, NULL},
    {1, 0, 0, 0, NULL},
    {2, 0, 0, 0, NULL},
    {3, 0, 0, 0, NULL},
    {4, 0, 0, 0, NULL},
    {5, 0, 0, 0, NULL},
    {6, 0, 0, 0, NULL},
    {7, 0, 0, 0, NULL},
    {8, 0, 0, 0, NULL},
    {9, 0, 0, 0, NULL},
    {10, 0, 0, 0, NULL},
    {11, 0, 0, 0, NULL},
    {12, 0, 0, 0, NULL},
    {13, 0, 0, 0, NULL},
    {14, 0, 0, 0, NULL},
    {15, 0, 0, 0, NULL},
    {16, 0, 0, 0, NULL},
    {17, 0, 0, 0, NULL},
    {18, 0, 0, 0, NULL},
};

PCarQuintet __cdecl _CarQuintet_GetNullValue(CarQuintetFlags flag)
{
    flag = flag & CarQuintetFlag_TypeMask;
    if (flag < 1 || flag > 18) {
        flag = 0;
    }

    return &s_nullCq[flag];
}

PCarQuintet __cdecl _CarQuintet_Init(PCarQuintet pCq,
    PVoid pBuf, Int32 size, Int32 used, CarQuintetFlags flags)
{
    ELA_ASSERT_WITH_BLOCK(used <= size) {
        printf("_CarQuintet_Init with invalid arguments. size: %d, used: %d, flags: %08x\n",
                size, used, flags);
    }

    if (used > size) used = size;

    if (pCq) {
        pCq->m_flags = flags;
        pCq->m_reserve = 0;
        if (pBuf) {
            assert(size >=0 && used >= 0);
            pCq->m_size = size;
            pCq->m_used = used;
        }
        else {
            assert(size ==0 && used == 0);
            pCq->m_size = 0;
            pCq->m_used = 0;
        }
        pCq->m_pBuf = pBuf;
    }

    return pCq;
}

PCarQuintet __cdecl _CarQuintet_Alloc(Int32 size)
{
    if (size < 0 || size > (Int32)_MAX_CARQUINTET_SIZE_) {
        return NULL;
    }

    Int32 bufSize = sizeof(CarQuintet) + (size ? size : 1);
    SharedBuffer* buf = SharedBuffer::Alloc(bufSize, FALSE);
    if (buf) {
        return (PCarQuintet)(buf->GetData());
    }
    return NULL;
}

void __cdecl _CarQuintet_Free(PCarQuintet pCq)
{
    if (!pCq) return;

    if (IS_QUINTENT_FLAG(pCq, CarQuintetFlag_HeapAlloced)) {
        SharedBuffer::GetBufferFromData(pCq)->Release();
    }
}

PCarQuintet __cdecl _CarQuintet_Clone(const PCarQuintet pCq)
{
    if (!pCq) return NULL;

    PCarQuintet pNewCq = _CarQuintet_Alloc(pCq->m_size);
    if (pNewCq) {
        CarQuintetFlags flags = pCq->m_flags & CarQuintetFlag_TypeMask;
        flags |= CarQuintetFlag_HeapAlloced;

        _CarQuintet_Init(pNewCq, pNewCq + 1, pCq->m_size, pCq->m_used, flags);
        if (pCq->m_pBuf) {
            memcpy(pNewCq->m_pBuf, pCq->m_pBuf, pCq->m_size);
        }
        else {
            pNewCq->m_pBuf = NULL;
        }
    }

    return pNewCq;
}

void __cdecl _CarQuintet_AddRef(const PCarQuintet pCq)
{
    if (!pCq) return;

    if (IS_QUINTENT_FLAG(pCq, CarQuintetFlag_HeapAlloced)) {
        SharedBuffer::GetBufferFromData(pCq)->Acquire();
        pCq->m_flags |= CarQuintetFlag_AutoRefCounted;
    }
}

Int32  __cdecl _CarQuintet_Release(PCarQuintet pCq)
{
    if (!pCq) return 0;

    if (IS_QUINTENT_FLAG(pCq, CarQuintetFlag_HeapAlloced)) {
        SharedBuffer * buf = SharedBuffer::GetBufferFromData(pCq);
        if (IS_QUINTENT_FLAG(pCq, CarQuintetFlag_AutoRefCounted)) {
            return buf->Release();
        }
        else {
            ELA_ASSERT_WITH_BLOCK(buf->RefCount() == 0) {
                printf(" >> %s %d\n >> Ref count of share buffer %d isn't zero,"
                        " and CarQuintetFlag_AutoRefCounted isn't set too. m_flags:%08x, m_size:%d\n",
                        __FILE__, __LINE__, buf->RefCount(), pCq->m_flags, pCq->m_size);
            }

            SharedBuffer::Dealloc(buf);
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------

PCarQuintet __cdecl _ArrayOf_Alloc(Int32 size, CarQuintetFlags flags)
{
    PCarQuintet pCq = _CarQuintet_Alloc(size);
    flags |= CarQuintetFlag_HeapAlloced;
    _CarQuintet_Init(pCq, pCq + 1, size, size, flags);
    if (pCq) memset(pCq->m_pBuf, 0x0, size);

    return pCq;
}

PCarQuintet __cdecl _ArrayOf_Alloc_Box(PVoid pBuf, Int32 size, CarQuintetFlags flags)
{
    if (size < 0) return NULL;

    PCarQuintet pCq = _CarQuintet_Alloc(0);
    flags |= CarQuintetFlag_HeapAlloced;
    return _CarQuintet_Init(pCq, pBuf, size, size, flags);
}

Int32 __cdecl _ArrayOf_Replace(PCarQuintet pCq, Int32 offset, const PByte p, Int32 n)
{
    if (!pCq || !pCq->m_pBuf || offset < 0 || !p || n < 0 || pCq->m_size < offset) {
        return -1;
    }

    Int32 nReplace = MIN(n, pCq->m_size - offset);
    memcpy((PByte)pCq->m_pBuf + offset, p, nReplace);

    return nReplace;
}

Int32 __cdecl _ArrayOf_Copy(PCarQuintet pcqDst, const CarQuintet* pcqSrc)
{
    if (!pcqDst || !pcqDst->m_pBuf || !pcqSrc || !pcqSrc->m_pBuf) {
        return -1;
    }

    Int32 n = MIN(pcqSrc->m_used, pcqDst->m_size);
    memcpy(pcqDst->m_pBuf, pcqSrc->m_pBuf, n);
    return n;
}

Int32 __cdecl _ArrayOf_CopyEx(PCarQuintet pCq, const Byte* p, MemorySize n)
{
    if (!pCq || !pCq->m_pBuf || !p || n < 0) {
        return -1;
    }

    n = MIN(n, pCq->m_size);
    memcpy(pCq->m_pBuf, p, n);
    return n;
}

}
