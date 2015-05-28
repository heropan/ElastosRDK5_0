
#include "coredef.h"
#include "FilterReader.h"

namespace Elastos {
namespace IO {

FilterReader::FilterReader()
{
}

FilterReader::~FilterReader()
{
}

ECode FilterReader::Init(
    /* [in] */ IReader* in)
{
    VALIDATE_NOT_NULL(in);

    AutoPtr<IInterface> obj;
    in->GetLock((IInterface**)&obj);
    LockObject* lockobj = (LockObject*)obj.Get();
    Reader::Init(lockobj);

    mIn = in;

    return NOERROR;
}

ECode FilterReader::Close()
{
    assert(mLock != NULL);
    Object::Autolock lock(mLock);

    return ICloseable::Probe(mIn)->Close();
}

ECode FilterReader::Mark(
    /* [in] */ Int32 readLimit)
{
    assert(mLock != NULL);
    Object::Autolock lock(mLock);

    return mIn->Mark(readLimit);
}

ECode FilterReader::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    assert(supported != NULL);
    assert(mLock != NULL);
    Object::Autolock lock(mLock);

    return mIn->IsMarkSupported(supported);
}

ECode FilterReader::Read(
    /* [out] */ Int32* value)
{
    assert(value != NULL);
    assert(mLock != NULL);
    Object::Autolock lock(mLock);


    return mIn->Read(value);
}

ECode FilterReader::ReadChars(
    /* [out] */ ArrayOf<Char32>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    assert(buffer != NULL);
    assert(number != NULL);
    assert(mLock != NULL);
    Object::Autolock lock(mLock);

    return mIn->ReadChars(buffer, offset, count, number);
}

ECode FilterReader::IsReady(
    /* [out] */ Boolean* ready)
{
    assert(ready != NULL);
    assert(mLock != NULL);
    Object::Autolock lock(mLock);

    return mIn->IsReady(ready);
}

ECode FilterReader::Reset()
{
    assert(mLock != NULL);
    Object::Autolock lock(mLock);

    return mIn->Reset();
}

ECode FilterReader:: Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    assert(number != NULL);
    assert(mLock != NULL);
    Object::Autolock lock(mLock);

    return mIn->Skip(count, number);
}

} // namespace IO
} // namespace Elastos
