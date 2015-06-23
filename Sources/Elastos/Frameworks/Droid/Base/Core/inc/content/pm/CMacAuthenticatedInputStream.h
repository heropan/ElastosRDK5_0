
#ifndef __CMACAUTHENTICATEDINPUTSTREAM_H__
#define __CMACAUTHENTICATEDINPUTSTREAM_H__

#include "_CMacAuthenticatedInputStream.h"
#include "elastos/io/FilterInputStream.h"

using Elastos::Crypto::IMac;
using Elastos::IO::IInputStream;
using Elastos::IO::FilterInputStream;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

/**
 * An input stream filter that applies a MAC to the data passing through it. At
 * the end of the data that should be authenticated, the tag can be calculated.
 * After that, the stream should not be used.
 *
 * @hide
 */
CarClass(CMacAuthenticatedInputStream), public FilterInputStream
{
public:
    CMacAuthenticatedInputStream();

    ~CMacAuthenticatedInputStream();

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IInputStream* inStream,
        /* [in] */ IMac* mac);

    CARAPI IsTagEqual(
        /* [in] */ ArrayOf<Byte>* tag,
        /* [out] */ Boolean* result);

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [out] */ Int32* number);

    CARAPI Close();

    CARAPI Available(
        /* [out] */ Int32* number);

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 byteCount,
        /* [out] */ Int64* number);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

private:
    AutoPtr<IMac> mMac;
};

}
}
}
}

#endif // __CMACAUTHENTICATEDINPUTSTREAM_H__
