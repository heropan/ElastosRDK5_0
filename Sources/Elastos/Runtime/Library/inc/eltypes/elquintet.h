//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __ELQUINTET_H__
#define __ELQUINTET_H__

#include <elatypes.h>
#include <elaobj.h>
#include <eladef.h>
#include <elsharedbuf.h>
#include <elquintettype.h>
#include <elrefbase.h>
#include <elautoptr.h>
#include <elstring.h>

#if defined(_DEBUG) || defined(_ELASTOS_DEBUG)
#include <stdio.h>
#endif

extern "C" {
    _ELASTOS PCarQuintet __cdecl _CarQuintet_Init(_ELASTOS PCarQuintet pCq,
            _ELASTOS PVoid pBuf, _ELASTOS Int32 size, _ELASTOS Int32 used,
            _ELASTOS CarQuintetFlags flags);
    _ELASTOS PCarQuintet __cdecl _CarQuintet_Alloc(_ELASTOS Int32 size);
    void __cdecl _CarQuintet_Free(_ELASTOS PCarQuintet pCq);
    _ELASTOS PCarQuintet __cdecl _CarQuintet_Clone(const _ELASTOS PCarQuintet pCq);
    void __cdecl _CarQuintet_AddRef(const _ELASTOS PCarQuintet pCq);
    _ELASTOS Int32 __cdecl _CarQuintet_Release(_ELASTOS PCarQuintet pCq);

    _ELASTOS Int32 __cdecl _ArrayOf_Copy(_ELASTOS PCarQuintet pcqDst,
            const _ELASTOS CarQuintet* pcqSrc);
    _ELASTOS Int32 __cdecl _ArrayOf_CopyEx(_ELASTOS PCarQuintet pCq,
            const _ELASTOS Byte* p, _ELASTOS Int32 n);
    _ELASTOS PCarQuintet __cdecl _ArrayOf_Alloc(_ELASTOS Int32 size,
            _ELASTOS CarQuintetFlags flags);
    _ELASTOS PCarQuintet __cdecl _ArrayOf_Alloc_Box(_ELASTOS PVoid pBuf,
            _ELASTOS Int32 size, _ELASTOS CarQuintetFlags flags);
    _ELASTOS Int32 __cdecl _ArrayOf_Replace(_ELASTOS PCarQuintet pCq,
            _ELASTOS Int32 offset, const _ELASTOS PByte p, _ELASTOS Int32 n);

    _ELASTOS PCarQuintet __cdecl _CarQuintet_GetNullValue(
            _ELASTOS CarQuintetFlags flag);
}

//
//  Memory structure of ArrayOf:
//
//           ________
//          |  pBuf  |
//          |        v
//  +------------+   +------------------------+
//  | CarQuintet |   |  C Array Data          |
//  +------------+   +------------------------+
//  ^
//  |____ ArrayOf (m_pCq)
//
//

_ELASTOS_NAMESPACE_BEGIN

// NOTE1: MS CL compiler can't support function's template specialization well,
//   so only template class works.
// NOTE2: We shall emit a COMPILE-TIME error if user let ArrayOf
//   to contain a
//   non-automation type. There's an undefined variable in Type2Flag::Flag()'s
//   default implementation, which will emit the error. Also, we must deal
//   with illegal types at runtime.
//
template <class T> struct Type2Flag
{
    static Int32 Flag() {
        if (SUPERSUBCLASS_EX(IInterface*, T)) {
            return CarQuintetFlag_Type_IObject;
        }
        else if (SUPERSUBCLASS_EX(ElRefBase*, T)) {
            return CarQuintetFlag_Type_RefObject;
        }
        else if (SUPERSUBCLASS_EX(ElLightRefBase*, T)) {
            return CarQuintetFlag_Type_LightRefObject;
        }
        else {
            return CarQuintetFlag_Type_Struct;
        }
    }
};

#define DECL_TYPE2FLAG_TMPL(type, flag) \
    template <> struct Type2Flag<type> { static Int32 Flag() \
    { return (flag); } }

DECL_TYPE2FLAG_TMPL(Int8,               CarQuintetFlag_Type_Int8);
DECL_TYPE2FLAG_TMPL(Int16,              CarQuintetFlag_Type_Int16);
DECL_TYPE2FLAG_TMPL(Int32,              CarQuintetFlag_Type_Int32);
DECL_TYPE2FLAG_TMPL(Int64,              CarQuintetFlag_Type_Int64);

DECL_TYPE2FLAG_TMPL(Byte,               CarQuintetFlag_Type_Byte);
DECL_TYPE2FLAG_TMPL(UInt16,             CarQuintetFlag_Type_UInt16);
DECL_TYPE2FLAG_TMPL(UInt32,             CarQuintetFlag_Type_UInt32);
DECL_TYPE2FLAG_TMPL(UInt64,             CarQuintetFlag_Type_UInt64);

DECL_TYPE2FLAG_TMPL(Float,              CarQuintetFlag_Type_Float);
DECL_TYPE2FLAG_TMPL(Double,             CarQuintetFlag_Type_Double);

DECL_TYPE2FLAG_TMPL(String,             CarQuintetFlag_Type_String);

DECL_TYPE2FLAG_TMPL(EMuid,              CarQuintetFlag_Type_EMuid);
DECL_TYPE2FLAG_TMPL(EGuid,              CarQuintetFlag_Type_EGuid);
DECL_TYPE2FLAG_TMPL(IInterface *,          CarQuintetFlag_Type_IObject);
DECL_TYPE2FLAG_TMPL(ElRefBase *,        CarQuintetFlag_Type_RefObject);
DECL_TYPE2FLAG_TMPL(ElLightRefBase *,   CarQuintetFlag_Type_LightRefObject);

#define _MAX_CARQUINTET_SIZE_  (0x7FFFFFFF - sizeof(CarQuintet) - sizeof(SharedBuffer))
#define IS_QUINTENT_FLAG(pcq, flag)     ((pcq) && ((pcq)->m_flags & (flag)))

template <class T>
class ArrayOf;

//---------------QuintetObjectReleaseOp----------------------------------------

template <class T>
struct QuintetObjectReleaseOp
{
    void operator()(void const * buf);
};

template <>
struct QuintetObjectReleaseOp<String>
{
    void operator()(void const * buf);
};

template <class T>
struct QuintetObjectReleaseOp<AutoPtr<T> >
{
    void operator()(void const * buf);
};

//---------------QuintetObjectCopyOp-------------------------------------------

struct QuintetObjectCopyOp
{
    template <class T>
    Int32 operator()(
        ArrayOf<T>* dst, Int32 offset, T const* src, Int32 srcOffset, Int32 count);

    template <class T>
    Int32 operator()(
        ArrayOf<AutoPtr<T> >* dst, Int32 offset,
        AutoPtr<T> const* src, Int32 srcOffset, Int32 count);

    Int32 operator()(
        ArrayOf<String>* dst, Int32 offset,
        String const* src, Int32 srcOffset, Int32 count);
};

//---------------ArrayOf-------------------------------------------------------

/** @addtogroup CARTypesRef
  *   @{
  */
template <class T>
class ArrayOf : public CarQuintet
{
public:
    T* GetPayload() const {
        return (T*)m_pBuf;
    };

    operator PCarQuintet() {
        return this;
    }

    Int32 GetLength() const {
        return m_pBuf ? m_size / sizeof(T) : 0;
    }

    Int32 Copy(T const* pBuf, Int32 n) {
        QuintetObjectCopyOp copyOp;
        return copyOp(this, 0, pBuf, 0, n);
    }

    Int32 Copy(Int32 offset, T const* pBuf, Int32 n) {
        QuintetObjectCopyOp copyOp;
        return copyOp(this, offset, pBuf, 0, n);
    }

    Int32 Copy(const ArrayOf<T> *pSrc) {
        if (this == pSrc) {
            return GetLength();
        }

        QuintetObjectCopyOp copyOp;
        return copyOp(this, 0, (T const*)pSrc->GetPayload(), 0, pSrc->GetLength());
    }

    Int32 Copy(const ArrayOf<T> *pSrc, Int32 count) {
        QuintetObjectCopyOp copyOp;
        return copyOp(this, 0, (T const*)pSrc->GetPayload(), 0, count);
    }

    Int32 Copy(const ArrayOf<T> *pSrc, Int32 srcOffset, Int32 count) {
        QuintetObjectCopyOp copyOp;
        return copyOp(this, 0, (T const*)pSrc->GetPayload(), srcOffset, count);
    }

    Int32 Copy(Int32 offset, const ArrayOf<T> *pSrc) {
        QuintetObjectCopyOp copyOp;
        return copyOp(this, offset, (T const*)pSrc->GetPayload(), 0, pSrc->GetLength());
    }

    Int32 Copy(Int32 offset, const ArrayOf<T> *pSrc, Int32 count) {
        QuintetObjectCopyOp copyOp;
        return copyOp(this, offset, (T const*)pSrc->GetPayload(), 0, count);
    }

    Int32 Copy(Int32 offset, const ArrayOf<T> *pSrc, Int32 srcOffset, Int32 count) {
        QuintetObjectCopyOp copyOp;
        return copyOp(this, offset, (T const*)pSrc->GetPayload(), srcOffset, count);
    }

    ArrayOf<T> *Clone() const {
        PCarQuintet pNewCq = _CarQuintet_Alloc(m_size);
        if (pNewCq) {
            CarQuintetFlags flags = m_flags & CarQuintetFlag_TypeMask;
            flags |= CarQuintetFlag_HeapAlloced;
            _CarQuintet_Init(pNewCq, pNewCq + 1, m_size, m_used, flags);
            if (this->m_pBuf) {
                if (pNewCq) memset(pNewCq->m_pBuf, 0x0, m_size);
                QuintetObjectCopyOp copyOp;
                copyOp((ArrayOf<T> *)pNewCq, 0, (T const*)GetPayload(), 0, this->GetLength());
            }
            else {
                pNewCq->m_pBuf = NULL;
            }
        }

        return (ArrayOf<T> *)pNewCq;
    }

    void AddRef() const {
        _CarQuintet_AddRef((const PCarQuintet)this);
    }

    Int32 GetRefCount() {
        PCarQuintet pCq = (const PCarQuintet)this;
        if (IS_QUINTENT_FLAG(pCq, CarQuintetFlag_HeapAlloced)) {
            SharedBuffer * buf = SharedBuffer::GetBufferFromData(pCq);
            return buf->RefCount();
        }
        return 0;
    }

    Int32 Release() const {
        PCarQuintet pCq = (const PCarQuintet)this;
        if (IS_QUINTENT_FLAG(pCq, CarQuintetFlag_HeapAlloced)) {
            QuintetObjectReleaseOp<T> releaseOp;
            SharedBuffer * buf = SharedBuffer::GetBufferFromData(pCq);
            if (IS_QUINTENT_FLAG(pCq, CarQuintetFlag_AutoRefCounted)) {
                return buf->Release(releaseOp, 0);
            }
            else {
                ELA_ASSERT_WITH_BLOCK(buf->RefCount() == 0) {
                    printf(" >> %s %d\n >> Ref count of share buffer %d isn't zero,"
                            " and CarQuintetFlag_AutoRefCounted isn't set too. m_flags:%08x, m_size:%d\n",
                            __FILE__, __LINE__, buf->RefCount(), m_flags, m_size);
                }

                SharedBuffer::Dealloc(buf, releaseOp);
            }
        }
        return 0;
    }

    void Set(Int32 index, T const other) {
        Copy(index, &other, 1);
    }

    T& operator[](Int32 index) {
        assert(m_pBuf && index >= 0 && index < GetLength());
        return ((T*)(m_pBuf))[index];
    }

    const T& operator[](Int32 index) const {   // nothing
        assert(m_pBuf && index >= 0 && index < GetLength());
        return ((T*)(m_pBuf))[index];
    }

    static ArrayOf<T> *Alloc(Int32 capacity) {
        return (ArrayOf<T> *)_ArrayOf_Alloc(capacity * sizeof(T),
        Type2Flag<T>::Flag());
    }

    static ArrayOf<T> *Alloc(T *pBuf, Int32 capacity) {
        return (ArrayOf<T> *)_ArrayOf_Alloc_Box(
            pBuf, capacity * sizeof(T), Type2Flag<T>::Flag());
    }

    static void Free(ArrayOf<T> *pArray) {
        if (NULL != pArray) {
            pArray->Release();
        }
    }

    Boolean Contains(T const& value) {
        return IndexOf(value) != -1;
    }

    Int32 IndexOf(T const& value) {
        for (Int32 i = 0; i < GetLength(); ++i) {
            if (((T*)(m_pBuf))[i] == value) {
                return i;
            }
        }
        return -1;
    }

    // Notes: Make sure the array is ordered before searching.
    Int32 BinarySearch(T const& value) {
        return BinarySearch(this, value);
    }

    // Notes: Make sure the array is ordered before searching.
    static Int32 BinarySearch(const ArrayOf<T> *pArray, T const& value)
    {
        assert(pArray && pArray->m_pBuf);

        Int32 low = 0;
        Int32 high = pArray->GetLength() - 1;
        Int32 mid;

        while (low <= high) {
            mid = (low + high) >> 1;

            if (value == (*pArray)[mid]) {
                return mid;
            }

            if (value < (*pArray)[mid]) {
                high = mid - 1;
            }
            else {
                low = mid + 1;
            }
        }

        return ~low;
    }

    Boolean Equals(const ArrayOf<T> *other)
    {
        return Equals(this, other);
    }

    static Boolean Equals(const ArrayOf<T> *a, const ArrayOf<T> *b)
    {
        if (a == NULL && b == NULL) {
            return TRUE;
        }

        Int32 length = a->GetLength();
        if (length != b->GetLength()) {
            return FALSE;
        }

        for (Int32 i = 0; i < length; ++i) {
            if ((*a)[i] != (*b)[i]) {
                return FALSE;
            }
        }

        return TRUE;
    }

    ArrayOf(T *pBuf, Int32 capacity) {
        _CarQuintet_Init(this, pBuf, capacity * sizeof(T),
                        capacity * sizeof(T), Type2Flag<T>::Flag());
    }

private:
    // prohibit 'new' operator
    void * operator new(size_t cb);
    ArrayOf() {}
    ArrayOf& operator = (const ArrayOf& buf) {}
    ArrayOf(const ArrayOf& buf) {}
};
/** @} */

typedef AutoPtr<ArrayOf<Byte> >     ByteArray;
typedef AutoPtr<ArrayOf<Char32> >   Char32Array;
typedef AutoPtr<ArrayOf<String> >   StringArray;
typedef AutoPtr<ArrayOf<Int32> >    Int32Array;
typedef AutoPtr<ArrayOf<Int64> >    Int64Array;
typedef AutoPtr<ArrayOf<Float> >    FloatArray;
typedef AutoPtr<ArrayOf<Double> >   DoubleArray;

template <typename T>
class ArrayOf2 {
public:
    typedef AutoPtr<ArrayOf<T> >    ElementType;
    typedef ArrayOf<ElementType>    Type;
};

//---------------QuintetObjectReleaseOp----------------------------------------
template<class OrigT, class TargetT>
void ReleaseFunc(void const * buf)
{
    ArrayOf<OrigT>* pcq = (ArrayOf<OrigT>*)buf;
    Int32 length = pcq->GetLength();
    OrigT* p = (OrigT*)(pcq->m_pBuf);

    for(Int32 i = 0; i < length; ++i) {
        TargetT* prb = (TargetT*)(*p);
        if (prb) {
            prb->Release();
            *p = NULL;
        }
        ++p;
    }
}

template<class T, Boolean isElRefBaseObj, Boolean isElLightRefBaseObj, Boolean isCarObj>
struct ReleaseOpImpl
{
    void operator()(void const * buf)
    {
    }
};

template<class T>
struct ReleaseOpImpl<T, TRUE, TRUE, TRUE>
{
    void operator()(void const * buf)
    {
        ReleaseFunc<T, ElRefBase>(buf);
    }
};

template<class T>
struct ReleaseOpImpl<T, TRUE, TRUE, FALSE>
{
    void operator()(void const * buf)
    {
        ReleaseFunc<T, ElRefBase>(buf);
    }
};

template<class T>
struct ReleaseOpImpl<T, TRUE, FALSE, TRUE>
{
    void operator()(void const * buf)
    {
        ReleaseFunc<T, ElRefBase>(buf);
    }
};

template<class T>
struct ReleaseOpImpl<T, TRUE, FALSE, FALSE>
{
    void operator()(void const * buf)
    {
        ReleaseFunc<T, ElRefBase>(buf);
    }
};

template<class T>
struct ReleaseOpImpl<T, FALSE, TRUE, TRUE>
{
    void operator()(void const * buf)
    {
        ReleaseFunc<T, ElLightRefBase>(buf);
    }
};

template<class T>
struct ReleaseOpImpl<T, FALSE, TRUE, FALSE>
{
    void operator()(void const * buf)
    {
        ReleaseFunc<T, ElLightRefBase>(buf);
    }
};

template<class T>
struct ReleaseOpImpl<T, FALSE, FALSE, TRUE>
{
    void operator()(void const * buf)
    {
        ReleaseFunc<T, IInterface>(buf);
    }
};

template <class T>
inline void QuintetObjectReleaseOp<T>::operator()(void const* buf)
{
    if (NULL != buf) {
        ReleaseOpImpl<T,
            SUPERSUBCLASS_EX(ElRefBase*, T),
            SUPERSUBCLASS_EX(ElLightRefBase*, T),
            SUPERSUBCLASS_EX(IInterface*, T)> impl;
        impl(buf);
    }
}

template <class T>
inline void QuintetObjectReleaseOp<AutoPtr<T> >::operator()(void const* buf)
{
    if (NULL != buf) {
        ArrayOf<AutoPtr<T> >* pcq = (ArrayOf<AutoPtr<T> >*)buf;
        Int32 length = pcq->GetLength();

        for(Int32 i = 0; i < length; ++i) {
            if (NULL != (*pcq)[i]) {
                (*pcq)[i] = NULL;
            }
        }
    }
}

inline void QuintetObjectReleaseOp<String>::operator()(void const* buf)
{
    if (NULL != buf) {
        ArrayOf<String>* pcq = (ArrayOf<String>*)buf;
        Int32 length = pcq->GetLength();

        for(Int32 i = 0; i < length; ++i) {
            if (NULL != (*pcq)[i]) {
                (*pcq)[i] = NULL;
            }
        }
    }
}

//---------------QuintetObjectCopyOp--------------------------------------------
// Notes: ArrayOf::Copy equals System.Copy in java, not memcpy in c/c++.
//
template<class OrigT, class TargetT>
Int32 CopyFunc(ArrayOf<OrigT>* dstArray, Int32 dstOffset, OrigT const* src, Int32 srcOffset, Int32 count)
{
    OrigT* dst = (OrigT*)(dstArray->m_pBuf);
    Int32 copyCount = MIN(count, dstArray->GetLength() - dstOffset);

    // self-copy to the same position.
    Boolean isSelfCopy = (dst == src);
    if ((isSelfCopy && dstOffset == srcOffset) || copyCount == 0) {
        return copyCount;
    }

    TargetT* prb = NULL;
    dst += dstOffset;
    src += srcOffset;

    Boolean isOverlap = (isSelfCopy && (dstOffset > srcOffset) && (dstOffset < srcOffset + copyCount));
    if (isOverlap) {
        for (Int32 i = copyCount - 1; i >= 0; --i) {
            prb = (TargetT*)(*(src + i));
            if (prb) {
                prb->AddRef();
            }

            prb = (TargetT*)(*(dst + i));
            if (prb) {
                prb->Release();
            }

            *(dst + i) = *(src + i);
        }
    }
    else {
        for (Int32 i = 0; i < copyCount; ++i) {
            prb = (TargetT*)(*src);
            if (prb) {
                prb->AddRef();
            }

            prb = (TargetT*)(*dst);
            if (prb) {
                prb->Release();
            }

            *dst++ = *src++;
        }
    }

    return copyCount;
}

template<class T>
static Int32 PlainCopy(T* dst, Int32 dstOffset, T const * src, Int32 srcOffset, Int32 copyCount)
{
    assert(dst != NULL && dstOffset >= 0 && src != NULL && srcOffset >= 0 && copyCount >= 0);

    // self-copy to the same position .
    Boolean isSelfCopy = (dst == src);
    if ((isSelfCopy && dstOffset == srcOffset) || copyCount == 0) {
        return copyCount;
    }

    dst += dstOffset;
    src += srcOffset;

    if (copyCount == 1) {
        *dst = *src;
        return copyCount;
    }

    Boolean isOverlap = (isSelfCopy && (dstOffset > srcOffset) && (dstOffset < srcOffset + copyCount));
    if (isOverlap) {
        for (Int32 i = copyCount - 1; i >= 0; --i) {
            *(dst + i) = *(src + i);
        }
    }
    else {
        for (Int32 i = 0; i < copyCount; ++i) {
            *dst++ = *src++;
        }
    }

    return copyCount;
}

template<class T, Boolean isElRefBaseObj, Boolean isElLightRefBaseObj, Boolean isCarObj>
struct CopyOpImpl
{
    Int32 operator()(ArrayOf<T>* dst, Int32 dstOffset, T const* src, Int32 srcOffset, Int32 count)
    {
        Int32 copyCount = MIN(count, dst->GetLength() - dstOffset);
        return PlainCopy((T*)(dst->m_pBuf), dstOffset, src, srcOffset, copyCount);
    }
};

template<class T>
struct CopyOpImpl<T, TRUE, TRUE, TRUE>
{
    Int32 operator()(ArrayOf<T>* dst, Int32 offset, T const* src, Int32 srcOffset, Int32 count)
    {
        return CopyFunc<T, ElRefBase>(dst, offset, src, srcOffset, count);
    }
};

template<class T>
struct CopyOpImpl<T, TRUE, TRUE, FALSE>
{
    Int32 operator()(ArrayOf<T>* dst, Int32 offset, T const* src, Int32 srcOffset, Int32 count)
    {
        return CopyFunc<T, ElRefBase>(dst, offset, src, srcOffset, count);
    }
};

template<class T>
struct CopyOpImpl<T, TRUE, FALSE, TRUE>
{
    Int32 operator()(ArrayOf<T>* dst, Int32 offset, T const* src, Int32 srcOffset, Int32 count)
    {
        return CopyFunc<T, ElRefBase>(dst, offset, src, srcOffset, count);
    }
};

template<class T>
struct CopyOpImpl<T, TRUE, FALSE, FALSE>
{
    Int32 operator()(ArrayOf<T>* dst, Int32 offset, T const* src, Int32 srcOffset, Int32 count)
    {
        return CopyFunc<T, ElRefBase>(dst, offset, src, srcOffset, count);
    }
};

template<class T>
struct CopyOpImpl<T, FALSE, TRUE, TRUE>
{
    Int32 operator()(ArrayOf<T>* dst, Int32 offset, T const* src, Int32 srcOffset, Int32 count)
    {
        return CopyFunc<T, ElLightRefBase>(dst, offset, src, srcOffset, count);
    }
};

template<class T>
struct CopyOpImpl<T, FALSE, TRUE, FALSE>
{
    Int32 operator()(ArrayOf<T>* dst, Int32 offset, T const* src, Int32 srcOffset, Int32 count)
    {
        return CopyFunc<T, ElLightRefBase>(dst, offset, src, srcOffset, count);
    }
};

template<class T>
struct CopyOpImpl<T, FALSE, FALSE, TRUE>
{
    Int32 operator()(ArrayOf<T>* dst, Int32 offset, T const* src, Int32 srcOffset, Int32 count)
    {
        return CopyFunc<T, IInterface>(dst, offset, src, srcOffset, count);
    }
};

template <class T>
inline Int32 QuintetObjectCopyOp::operator()(
    ArrayOf<T>* dst, Int32 offset, T const* src, Int32 srcOffset, Int32 count)
{
    Int32 realOffset = offset * sizeof(T);
    if (!dst || !dst->m_pBuf || !(src + srcOffset) || count <= 0
            || offset < 0  || dst->m_size < realOffset) {
        return -1;
    }

    CopyOpImpl<T,
        SUPERSUBCLASS_EX(ElRefBase*, T),
        SUPERSUBCLASS_EX(ElLightRefBase*, T),
        SUPERSUBCLASS_EX(IInterface*, T)> impl;
    return impl(dst, offset, src, srcOffset, count);
}

template <class T>
inline Int32 QuintetObjectCopyOp::operator()(
    ArrayOf<AutoPtr<T> >* dst, Int32 dstOffset, AutoPtr<T> const* src, Int32 srcOffset, Int32 count)
{
    if (!dst || !dst->m_pBuf || !(src + srcOffset) || count <= 0
            || dstOffset < 0 || dst->GetLength() < dstOffset) {
        return -1;
    }

    Int32 copyCount = MIN(count, dst->GetLength() - dstOffset);
    return PlainCopy((AutoPtr<T>*)(dst->m_pBuf), dstOffset, src, srcOffset, copyCount);
}

inline Int32 QuintetObjectCopyOp::operator()(
    ArrayOf<String>* dst, Int32 dstOffset, String const* src, Int32 srcOffset, Int32 count)
{
    if (!dst || !dst->m_pBuf || !(src + srcOffset) || count <= 0
            || dstOffset < 0 || dst->GetLength() < dstOffset) {
        return -1;
    }

    Int32 copyCount = MIN(count, dst->GetLength() - dstOffset);
    return PlainCopy((String*)(dst->m_pBuf), dstOffset, src, srcOffset, copyCount);
}

#define NULL_ARRAYOF(type) \
        *(const ArrayOf<type> *)_CarQuintet_GetNullValue( \
            Type2Flag<type>::Flag())

_ELASTOS_NAMESPACE_END

#endif // __ELQUINTET_H__
