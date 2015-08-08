//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <eltypes.h>

extern "C" {

_ELASTOS_NAMESPACE_USING

PCarQuintet __cdecl _CarQuintet_Init(
    /* [in] */ PCarQuintet cq,
    /* [in] */ PVoid buf,
    /* [in] */ Int32 size,
    /* [in] */ Int32 used,
    /* [in] */ CarQuintetFlags flags)
{
    ELA_ASSERT_WITH_BLOCK(used <= size) {
        printf("_CarQuintet_Init with invalid arguments. size: %d, used: %d, flags: %08x\n",
                size, used, flags);
    }

    if (used > size) used = size;

    if (cq) {
        cq->m_flags = flags;
        cq->m_reserve = 0;
        if (buf) {
            assert(size >=0 && used >= 0);
            cq->m_size = size;
            cq->m_used = used;
        }
        else {
            assert(size ==0 && used == 0);
            cq->m_size = 0;
            cq->m_used = 0;
        }
        cq->m_pBuf = buf;
    }

    return cq;
}

PCarQuintet __cdecl _CarQuintet_Alloc(
    /* [in] */ Int32 size)
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

void __cdecl _CarQuintet_Free(
    /* [in] */ PCarQuintet cq)
{
    if (!cq) return;

    if (IS_QUINTENT_FLAG(cq, CarQuintetFlag_HeapAlloced)) {
        SharedBuffer::GetBufferFromData(cq)->Release();
    }
}

PCarQuintet __cdecl _CarQuintet_Clone(
    /* [in] */ const PCarQuintet cq)
{
    if (!cq) return NULL;

    PCarQuintet newCq = _CarQuintet_Alloc(cq->m_size);
    if (newCq) {
        CarQuintetFlags flags = cq->m_flags & CarQuintetFlag_TypeMask;
        flags |= CarQuintetFlag_HeapAlloced;

        _CarQuintet_Init(newCq, newCq + 1, cq->m_size, cq->m_used, flags);
        if (cq->m_pBuf) {
            memcpy(newCq->m_pBuf, cq->m_pBuf, cq->m_size);
        }
        else {
            newCq->m_pBuf = NULL;
        }
    }

    return newCq;
}

void __cdecl _CarQuintet_AddRef(
    /* [in] */ const PCarQuintet cq)
{
    if (!cq) return;

    if (IS_QUINTENT_FLAG(cq, CarQuintetFlag_HeapAlloced)) {
        SharedBuffer::GetBufferFromData(cq)->Acquire();
        cq->m_flags |= CarQuintetFlag_AutoRefCounted;
    }
}

Int32  __cdecl _CarQuintet_Release(
    /* [in] */ PCarQuintet cq)
{
    if (!cq) return 0;

    if (IS_QUINTENT_FLAG(cq, CarQuintetFlag_HeapAlloced)) {
        SharedBuffer * buf = SharedBuffer::GetBufferFromData(cq);
        if (IS_QUINTENT_FLAG(cq, CarQuintetFlag_AutoRefCounted)) {
            return buf->Release();
        }
        else {
            ELA_ASSERT_WITH_BLOCK(buf->RefCount() == 0) {
                printf(" >> %s %d\n >> Ref count of share buffer %d isn't zero,"
                        " and CarQuintetFlag_AutoRefCounted isn't set too. m_flags:%08x, m_size:%d\n",
                        __FILE__, __LINE__, buf->RefCount(), cq->m_flags, cq->m_size);
            }

            SharedBuffer::Dealloc(buf);
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------

PCarQuintet __cdecl _ArrayOf_Alloc(
    /* [in] */ Int32 size,
    /* [in] */ CarQuintetFlags flags)
{
    PCarQuintet cq = _CarQuintet_Alloc(size);
    flags |= CarQuintetFlag_HeapAlloced;
    _CarQuintet_Init(cq, cq + 1, size, size, flags);
    if (cq) memset(cq->m_pBuf, 0x0, size);

    return cq;
}

PCarQuintet __cdecl _ArrayOf_Alloc_Box(
    /* [in] */ PVoid buf,
    /* [in] */ Int32 size,
    /* [in] */ CarQuintetFlags flags)
{
    if (size < 0) return NULL;

    PCarQuintet cq = _CarQuintet_Alloc(0);
    flags |= CarQuintetFlag_HeapAlloced;
    return _CarQuintet_Init(cq, buf, size, size, flags);
}

Int32 __cdecl _ArrayOf_Replace(
    /* [in] */ PCarQuintet cq,
    /* [in] */ Int32 offset,
    /* [in] */ const Byte* p,
    /* [in] */ Int32 n)
{
    if (!cq || !cq->m_pBuf || offset < 0 || !p || n < 0 || cq->m_size < offset) {
        return -1;
    }

    Int32 nReplace = MIN(n, cq->m_size - offset);
    memcpy((Byte*)cq->m_pBuf + offset, p, nReplace);

    return nReplace;
}

Int32 __cdecl _ArrayOf_Copy(
    /* [in] */ PCarQuintet destCq,
    /* [in] */ const CarQuintet* srcCq)
{
    if (!destCq || !destCq->m_pBuf || !srcCq || !srcCq->m_pBuf) {
        return -1;
    }

    Int32 n = MIN(srcCq->m_used, destCq->m_size);
    memcpy(destCq->m_pBuf, srcCq->m_pBuf, n);
    return n;
}

Int32 __cdecl _ArrayOf_CopyEx(
    /* [in] */ PCarQuintet cq,
    /* [in] */ const Byte* p,
    /* [in] */ MemorySize n)
{
    if (!cq || !cq->m_pBuf || !p || n < 0) {
        return -1;
    }

    n = MIN(n, cq->m_size);
    memcpy(cq->m_pBuf, p, n);
    return n;
}

}
