
#ifndef __CFILTEROUTPUTSTREAM_H__
#define __CFILTEROUTPUTSTREAM_H__

#include "_CFilterOutputStream.h"
#include "FilterOutputStream.h"

namespace Elastos {
namespace IO {

CarClass(CFilterOutputStream)
    , public FilterOutputStream
{
public:
    CFilterOutputStream();

    ~CFilterOutputStream();

    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Int32 oneByte);

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI WriteBytesEx(
        /* [in] */ const ArrayOf<Byte> & buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI constructor(
        /* [in] */ IOutputStream* out);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);
};

} // namespace IO
} // namespace Elastos

#endif // __CFILTEROUTPUTSTREAM_H__
