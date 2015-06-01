
#include "coredef.h"
#include "StringWriter.h"
#include <elastos/core/Character.h>

using Elastos::Core::Character;

namespace Elastos {
namespace IO {

StringWriter::StringWriter()
{
    mBufLock = new Object();
}

StringWriter::~StringWriter()
{
}

ECode StringWriter::Init()
{
    mBuf = new StringBuffer(16);
    if (mBuf == NULL)
        return E_OUT_OF_MEMORY_ERROR;

    mLock = mBufLock;

    return NOERROR;
}

ECode StringWriter::Init(
    /* [in] */ Int32 initialSize)
{
    if (initialSize < 0) {
//      throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mBuf = new StringBuffer(initialSize);
    if (mBuf == NULL)
        return E_OUT_OF_MEMORY_ERROR;

    mLock = mBufLock;

    return NOERROR;
}

ECode StringWriter::Close()
{
    return NOERROR;
}

ECode StringWriter::Flush()
{
    return NOERROR;
}

ECode StringWriter::GetBuffer(
   /* [out] */ IStringBuffer** buf)
{
    VALIDATE_NOT_NULL(buf)

    *buf = mBuf;
    REFCOUNT_ADD(*buf)

    return NOERROR;
}

ECode StringWriter::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    mBuf->ToString(str);
    return NOERROR;
}

ECode StringWriter::Write(
    /* [in] */ Int32 oneChar8)
{
    mBuf->AppendChar(oneChar8);

    return NOERROR;
}

ECode StringWriter::Write(
    /* [in] */ ArrayOf<Char32>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    // avoid int overflow
    // BEGIN android-changed
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // removed redundant check, added null check, used (offset | count) < 0
    // instead of (offset < 0) || (count < 0) to safe one operation
    if ((offset | count) < 0 || count > buffer->GetLength() - offset) {
//      throw new IndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed
    if (count == 0) {
        return NOERROR;
    }

    AutoPtr<ArrayOf<Char8> > dst;
    Int32 dstOffset = 0;
    FAIL_RETURN(Character::ToChars(*buffer, offset, count, (ArrayOf<Char8>**)&dst, &dstOffset));

    mBuf->Append(String(dst->GetPayload()));

    return NOERROR;
}

ECode StringWriter::WriteString(
    /* [in] */ const String& str)
{
    return mBuf->Append(str);
}

ECode StringWriter::WriteString(
    /* [in] */ const String& str,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return mBuf->Append(str.Substring(offset, offset + count));
}

ECode StringWriter::AppendChar(
    /* [in] */ Char32 c)
{
    return Write(c);
}

ECode StringWriter::AppendCharSequence(
    /* [in] */ ICharSequence* csq)
{
    String str("NULL");
    if (csq != NULL) {
        csq->ToString(&str);
    }

    return WriteString(str);
}

ECode StringWriter::AppendCharSequence(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    String str("NULL");
    if (csq != NULL) {
        csq->ToString(&str);
    }

    return WriteString(str, start, end - start);
}

} // namespace IO
} // namespace Elastos
