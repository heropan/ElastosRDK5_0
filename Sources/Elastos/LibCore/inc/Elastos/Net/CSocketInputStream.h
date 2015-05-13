
#ifndef __CSOCKETINPUTSTREAM_H__
#define __CSOCKETINPUTSTREAM_H__

#include "_CSocketInputStream.h"
#include "CPlainSocketImpl.h"
#include <InputStream.h>

using Elastos::IO::InputStream;

namespace Elastos {
namespace Net {

CarClass(CSocketInputStream), public InputStream
{
public:
    CARAPI Available(
        /* [out] */ Int32* number);

    CARAPI Close();

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* pSupported);

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI ReadBytesEx(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* number);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

    CARAPI constructor(
        /* [in] */ ISocketImpl* impl);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

private:
    PlainSocketImpl* mSocket;
};

} // namespace Net
} // namespace Elastos

#endif // __CSOCKETINPUTSTREAM_H__
