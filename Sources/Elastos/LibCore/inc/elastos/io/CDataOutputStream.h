
#ifndef __ELASTOS_IO_CDATAOUTPUTSTREAM_H__
#define __ELASTOS_IO_CDATAOUTPUTSTREAM_H__

#include "_Elastos_IO_CDataOutputStream.h"
#include "DataOutputStream.h"

namespace Elastos {
namespace IO {

CarClass(CDataOutputStream)
    , public DataOutputStream
{
public:
    CDataOutputStream();

    ~CDataOutputStream();

    CARAPI constructor(
        /* [in] */ IOutputStream* out);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI Close();

    CARAPI Flush();

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI Write(
        /* [in] */ Int32 oneByte);

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte> & buffer);

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte> & buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI WriteBoolean(
        /* [in] */ Boolean val);

    CARAPI WriteByte(
        /* [in] */ Int32 val);

    CARAPI WriteBytesFromString(
        /* [in] */ const String& str);

    CARAPI WriteChar(
        /* [in] */ Int32 val);

    CARAPI Write(
        /* [in] */ const String& str);

    CARAPI WriteDouble(
        /* [in] */ Double val);

    CARAPI WriteFloat(
        /* [in] */ Float val);

    CARAPI WriteInt32(
        /* [in] */ Int32 val);

    CARAPI WriteInt64(
        /* [in] */ Int64 val);

    CARAPI WriteInt16(
        /* [in] */ Int32 val);

    CARAPI WriteUTF(
        /* [in] */ const String& str);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);
};

} // namespace IO
} // namespace Elastos

#endif
