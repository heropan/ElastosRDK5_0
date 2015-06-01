
#ifndef __ELASTOS_IO_CSEQUENCEINPUTSTREAM_H__
#define __ELASTOS_IO_CSEQUENCEINPUTSTREAM_H__

#include "_Elastos_IO_CSequenceInputStream.h"
#include "SequenceInputStream.h"

namespace Elastos {
namespace IO {

CarClass(CSequenceInputStream), public SequenceInputStream
{
public:
    CARAPI constructor(
        /* [in] */ IInputStream* s1,
        /* [in] */ IInputStream* s2);

    CARAPI constructor(
        /* [in] */ IObjectEnumerator* e);

    CARAPI Available(
        /* [out] */ Int32* number);

    CARAPI Close();

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* number);

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

    CARAPI Reset();

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

} // namespace IO
} // namespace Elastos

#endif //__ELASTOS_IO_CSEQUENCEINPUTSTREAM_H__
