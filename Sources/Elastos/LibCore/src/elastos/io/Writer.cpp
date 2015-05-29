
#include "Writer.h"
#include "elastos/core/Character.h"

using Elastos::Core::Character;
using Elastos::Core::EIID_IAppendable;

namespace Elastos {
namespace IO {

CAR_INTERFACE_IMPL_2(Writer, Object, IAppendable, IWriter)

Writer::Writer()
{
    mLock = this->Probe(EIID_IObject);
    mLock->AddRef();
}

Writer::Writer(
    /* [in] */ IObject* lock)
{
    assert(lock != NULL);
    mLock = lock;
    mLock->AddRef();
}

Writer::~Writer()
{
    if(mLock != this->Probe(EIID_IObject))
    {
        mLock->Release();
        mLock = NULL;
    }
}

ECode Writer::Write(
    /* [in] */ Int32 oneChar32)
{
    assert(mLock != NULL);
    Object::Autolock lock(mLock);

    ArrayOf_<Char32, 1> buf;
    buf[0] = oneChar32;

    return Write(buf);
}

ECode Writer::Write(
    /* [in] */ const ArrayOf<Char32>& buffer)
{
    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note

    return Write(buffer, 0, buffer.GetLength());
}

ECode Writer::Write(
    /* [in] */ const String& str)
{
    return Write(str, 0, str.GetLength());
}

ECode Writer::Write(
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
    return Write(*buf, 0, buf->GetLength());
}

ECode Writer::AppendChar(
    /* [in] */ Char32 c)
{
    return Write(c);
}

ECode Writer::Append(
    /* [in] */ ICharSequence* csq)
{
    String tmp("NULL");
    if (NULL != csq) {
        csq->ToString(&tmp);
    }

    return Write(tmp);
}

ECode Writer::Append(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    String tmp("NULL");
    if (NULL != csq) {
        csq->ToString(&tmp);
    }

    return Write(tmp.Substring(start, end));
}

ECode Writer::CheckError(
    /* [out] */ Boolean* hasError)
{
    assert(hasError != NULL);

    *hasError = FALSE;
    return NOERROR;
}

ECode Writer::constructor(
    /* [in] */ IObject* lock)
{
    assert(lock != NULL);
    mLock = lock;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
