#include "ext/frameworkdef.h"
#include "database/CCharArrayBuffer.h"

namespace Elastos {
namespace Droid {
namespace Database {

ECode CCharArrayBuffer::GetData(
    /* [out, callee] */ ArrayOf<Char32>** data)
{
    VALIDATE_NOT_NULL(data)
    return CharArrayBuffer::GetData(data);
}

ECode CCharArrayBuffer::SetData(
    /* [in] */ ArrayOf<Char32>* data)
{
    return CharArrayBuffer::SetData(data);
}

ECode CCharArrayBuffer::GetSizeCopied(
    /* [out] */ Int32* sizeCopied)
{
    VALIDATE_NOT_NULL(sizeCopied)
    return CharArrayBuffer::GetSizeCopied(sizeCopied);
}

ECode CCharArrayBuffer::SetSizeCopied(
    /* [in] */ Int32 sizeCopied)
{
    return CharArrayBuffer::SetSizeCopied(sizeCopied);
}

ECode CCharArrayBuffer::constructor(
    /* [in] */ Int32 size)
{
    Init(size);
    return NOERROR;
}

ECode CCharArrayBuffer::constructor(
    /* [in] */ ArrayOf<Char32>* buf)
{
    Init(buf);
    return NOERROR;
}

} //Database
} //Droid
} //Elastos