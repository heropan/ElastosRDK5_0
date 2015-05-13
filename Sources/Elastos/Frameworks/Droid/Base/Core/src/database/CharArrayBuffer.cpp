#include "database/CharArrayBuffer.h"

namespace Elastos {
namespace Droid {
namespace Database {

CharArrayBuffer::CharArrayBuffer()
    : mSizeCopied(0)
{}

void CharArrayBuffer::Init(
    /* [in] */ Int32 size)
{
    mData = ArrayOf<Char32>::Alloc(size);
}

void CharArrayBuffer::Init(
    /* [in] */ ArrayOf<Char32>* buf)
{
    mData = buf;
}

ECode CharArrayBuffer::GetData(
    /* [out,callee] */ ArrayOf<Char32>** data)
{
    VALIDATE_NOT_NULL(data)
    *data = mData;
    ARRAYOF_ADDREF(*data);
    return NOERROR;
}

ECode CharArrayBuffer::SetData(
    /* [in] */ ArrayOf<Char32>* data)
{
    mData = data;
    return NOERROR;
}

ECode CharArrayBuffer::GetSizeCopied(
    /* [out] */ Int32* sizeCopied)
{
    VALIDATE_NOT_NULL(sizeCopied)
    *sizeCopied = mSizeCopied;
    return NOERROR;
}

ECode CharArrayBuffer::SetSizeCopied(
    /* [in] */ Int32 sizeCopied)
{
    mSizeCopied = sizeCopied;
    return NOERROR;
}


} //Database
} //Droid
} //Elastos
