
#include "Writer.h"
#include "elastos/Character.h"

using Elastos::Core::Character;

namespace Elastos {
namespace IO {

Writer::Writer()
{
    mLock = new LockObject();
}

Writer::Writer(
    /* [in] */ LockObject* lock)
{
    assert(lock != NULL);
    mLock = lock;
}

Writer::~Writer()
{
    mLock = NULL;
}

AutoPtr<IInterface> Writer::GetLock()
{
    AutoPtr<IInterface> obj = (IInterface*)mLock.Get();
    return obj;
}

ECode Writer::Write(
    /* [in] */ Int32 oneChar32)
{
    assert(mLock != NULL);
    Object::Autolock lock(mLock);

    ArrayOf_<Char32, 1> buf;
    buf[0] = oneChar32;

    return WriteChars(buf);
}

ECode Writer::WriteChars(
    /* [in] */ const ArrayOf<Char32>& buffer)
{
    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note

    return WriteCharsEx(buffer, 0, buffer.GetLength());
}

ECode Writer::WriteString(
    /* [in] */ const String& str)
{
    return WriteStringEx(str, 0, str.GetLength());
}

ECode Writer::WriteStringEx(
    /* [in] */ const String& str,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    if ((offset | count) < 0 || offset > str.GetLength() - count) {
//      throw new StringIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    assert(mLock != NULL);
    Object::Autolock lock(mLock);

    AutoPtr<ArrayOf<Char32> > buf = str.GetChars(offset, offset + count);
    return WriteCharsEx(*buf, 0, buf->GetLength());
}

ECode Writer::AppendChar(
    /* [in] */ Char32 c)
{
    return Write(c);
}

ECode Writer::AppendCharSequence(
    /* [in] */ ICharSequence* csq)
{
    String tmp("NULL");
    if (NULL != csq) {
        csq->ToString(&tmp);
    }

    return WriteString(tmp);
}

ECode Writer::AppendCharSequenceEx(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    String tmp("NULL");
    if (NULL != csq) {
        csq->ToString(&tmp);
    }

    return WriteString(tmp.Substring(start, end));
}

ECode Writer::CheckError(
    /* [out] */ Boolean* hasError)
{
    assert(hasError != NULL);

    *hasError = FALSE;
    return NOERROR;
}

ECode Writer::Init(
    /* [in] */ LockObject* lock)
{
    assert(lock != NULL);
    mLock = lock;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
