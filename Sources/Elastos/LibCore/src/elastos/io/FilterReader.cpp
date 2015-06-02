
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
    VALIDATE_NOT_NULL(supported)
    assert(mLock != NULL);
    Object::Autolock lock(mLock);

    return mIn->IsMarkSupported(supported);
}

ECode FilterReader::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
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
    VALIDATE_NOT_NULL(buffer)
    VALIDATE_NOT_NULL(number)
    assert(mLock != NULL);
    Object::Autolock lock(mLock);

    return mIn->ReadChars(buffer, offset, count, number);
}

ECode FilterReader::IsReady(
    /* [out] */ Boolean* ready)
{
    VALIDATE_NOT_NULL(ready)
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
    VALIDATE_NOT_NULL(number)
    assert(mLock != NULL);
    Object::Autolock lock(mLock);

    return mIn->Skip(count, number);
}

} // namespace IO
} // namespace Elastos
