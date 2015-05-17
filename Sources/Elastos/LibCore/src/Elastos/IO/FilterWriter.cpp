
#include "cmdef.h"
#include "FilterWriter.h"

namespace Elastos {
namespace IO {

FilterWriter::FilterWriter()
{
}

FilterWriter::~FilterWriter()
{
}

ECode FilterWriter::Init(
    /* [in] */ IWriter* out)
{
    AutoPtr<IInterface> obj;
    out->GetLock((IInterface**)&obj);
    LockObject* lockObj = (LockObject*)obj.Get();
    Writer::Init(lockObj);

    mOut = out;

    return NOERROR;
}

ECode FilterWriter::Close()
{
    assert(mLock != NULL);
    Object::Autolock lock(mLock);

    return ICloseable::Probe(mOut)->Close();
}

ECode FilterWriter::Flush()
{
    assert(mLock != NULL);
    Object::Autolock lock(mLock);

    return IFlushable::Probe(mOut)->Flush();
}

ECode FilterWriter::WriteCharsEx(
    /* [in] */ const ArrayOf<Char32>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    assert(mLock != NULL);
    Object::Autolock lock(mLock);

    return mOut->WriteCharsEx(buffer, offset, count);
}

ECode FilterWriter::Write(
    /* [in] */ Int32 oneChar32)
{
    assert(mLock != NULL);
    Object::Autolock lock(mLock);

    return mOut->Write(oneChar32);
}

ECode FilterWriter::WriteStringEx(
    /* [in] */ const String& str,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    assert(mLock != NULL);
    Object::Autolock lock(mLock);

    return mOut->WriteStringEx(str, offset, count);
}

} // namespace IO
} // namespace Elastos
