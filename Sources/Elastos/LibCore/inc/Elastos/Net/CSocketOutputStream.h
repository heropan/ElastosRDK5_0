
#ifndef __CSOCKETOUTPUTSTREAM_H__
#define __CSOCKETOUTPUTSTREAM_H__

#include "_CSocketOutputStream.h"
#include "PlainSocketImpl.h"
#include <OutputStream.h>

using Elastos::IO::OutputStream;

namespace Elastos {
namespace Net {

CarClass(CSocketOutputStream), public OutputStream
{
public:
    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Int32 oneByte);

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte> & buffer);

    CARAPI WriteBytesEx(
        /* [in] */ const ArrayOf<Byte> & buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI constructor(
        /* [in] */ ISocketImpl * pImpl);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

private:
    PlainSocketImpl* mSocket;
};

} // namespace Net
} // namespace Elastos

#endif // __CSOCKETOUTPUTSTREAM_H__
