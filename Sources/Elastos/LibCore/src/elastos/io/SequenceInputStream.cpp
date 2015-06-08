
#include "coredef.h"
#include "SequenceInputStream.h"
#include "CObjectContainer.h"

using Elastos::Core::CObjectContainer;

namespace Elastos {
namespace IO {

CAR_INTERFACE_IMPL(SequenceInputStream, InputStream, ISequenceInputStream)

ECode SequenceInputStream::constructor(
    /* [in] */ IInputStream* s1,
    /* [in] */ IInputStream* s2)
{
    if (s1 == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    CObjectContainer::New((IObjectContainer**)&mContainer);
    mContainer->Add(s2);
    mContainer->GetObjectEnumerator((IObjectEnumerator**)&mEnum);
    mIn = s1;
    return NOERROR;
}

ECode SequenceInputStream::constructor(
    /* [in] */ IObjectEnumerator* e)
{
    mEnum = e;
    Boolean succeeded = FALSE;
    e->MoveNext(&succeeded);
    if (succeeded) {
        e->Current((IInterface**)(&mIn));
        if (mIn == NULL) {
            return E_NULL_POINTER_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode SequenceInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number)
    if (mEnum != NULL && mIn != NULL) {
        return mIn->Available(number);
    }
    *number = 0;

    return NOERROR;
}

ECode SequenceInputStream::Close()
{
    while (mIn != NULL) {
        NextStream();
    }
    mEnum = NULL;
    mContainer = NULL;
    return NOERROR;
}

ECode SequenceInputStream::NextStream()
{
    if (mIn != NULL) {
        ICloseable::Probe(mIn)->Close();
        mIn = NULL;
    }
    Boolean succeeded = FALSE;
    mEnum->MoveNext(&succeeded);
    if (succeeded) {
        mEnum->Current((IInterface**)(&mIn));
        if (mIn == NULL) {
            return E_NULL_POINTER_EXCEPTION;
        }
    }
    else {
        mIn = NULL;
    }

    return NOERROR;
}

ECode SequenceInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    while (mIn != NULL) {
        mIn->Read(value);
        if ((*value) >= 0) {
            return NOERROR;
        }
        NextStream();
    }
    return -1;
}

ECode SequenceInputStream::Read(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer)
    VALIDATE_NOT_NULL(number)
    if (mIn == NULL) {
        *number = -1;
        return NOERROR;
    }
    if (buffer == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    if ((byteOffset | byteCount) < 0 || byteOffset > buffer->GetLength() - byteCount) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    while (mIn != NULL) {
        mIn->Read(buffer, byteOffset, byteCount, number);
        if ((*number) >= 0) {
            return NOERROR;
        }
        NextStream();
    }
    *number = -1;

    return NOERROR;
}

} // namespace IO
} // namespace Elastos
