
#ifndef __CSSLSOCKETOUTPUTSTREAM_H__
#define __CSSLSOCKETOUTPUTSTREAM_H__

#include "_CSSLSocketOutputStream.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CSSLSocketOutputStream)
{
public:
    CARAPI Close();

    CARAPI Write(
        /* [in] */ Int32 oneByte);

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte> & buffer);

    CARAPI WriteBytesEx(
        /* [in] */ const ArrayOf<Byte> & buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI CheckError(
        /* [out] */ Boolean * pHasError);

    CARAPI GetLock(
        /* [out] */ IInterface ** ppLockObj);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CSSLSOCKETOUTPUTSTREAM_H__
