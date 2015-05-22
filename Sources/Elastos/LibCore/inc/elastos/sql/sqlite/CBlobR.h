
#ifndef __CBLOBR_H__
#define __CBLOBR_H__

#include "_CBlobR.h"
#include "CBlob.h"
#include <InputStream.h>

using Elastos::IO::InputStream;

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CBlobR), public InputStream
{
public:
    CARAPI Available(
        /* [out] */ Int32* number);

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI Read(
         /* [out] */ Int32* value);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte> * buffer,
        /* [out] */ Int32* number);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte> * b,
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ Int32* number);

    CARAPI Reset();

    CARAPI Close();

    CARAPI Skip(
        /* [in] */ Int64 n,
        /* [out] */ Int64* number);

    CARAPI constructor(
        /* [in] */ Elastos::Sql::SQLite::IBlob* blob);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

private:
    AutoPtr<CBlob> mBlob;
    Int32 pos;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CBLOBR_H__
