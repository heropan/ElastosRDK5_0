
#ifndef __CDATATRUNCATION_H__
#define __CDATATRUNCATION_H__

#include "_Elastos_Sql_CDataTruncation.h"

namespace Elastos {
namespace Sql {

CarClass(CDataTruncation)
    , public Object
    , public IDataTruncation
    , public ISQLWarning
    , public ISerializable
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CARAPI  constructor(
        /* [in] */ Int32 index,
        /* [in] */ Boolean parameter,
        /* [in] */ Boolean read,
        /* [in] */ Int32 dataSize,
        /* [in] */ Int32 transferSize);

    // CARAPI  constructor(
    //     /* [in] */ Int32 index,
    //     /* [in] */ Boolean parameter,
    //     /* [in] */ Boolean read,
    //     /* [in] */ Int32 dataSize,
    //     /* [in] */ Int32 transferSize,
    //     /* [in] */ IThrowable * cause);

    CARAPI  GetDataSize(
        /* [out] */ Int32* size);

    CARAPI  GetIndex(
        /* [out] */ Int32* index);

    CARAPI  GetParameter(
        /* [out] */ Boolean* isPara);

    CARAPI  GetRead(
        /* [out] */ Boolean* isRead);

    CARAPI  GetTransferSize(
        /* [out] */ Int32* size);

    CARAPI GetNextWarning(
        /* [out] */ ISQLWarning ** sqlwar);

    CARAPI SetNextWarning(
        /* [in] */ ISQLWarning * w);

private:

    Int32 index;

    Boolean parameter;

    Boolean read;

    Int32 dataSize;

    Int32 transferSize;

    const static String THE_REASON;

    const static String THE_SQLSTATE_READ;

    const static String THE_SQLSTATE_WRITE;

    const static Int32 THE_ERROR_CODE = 0;

    const static Int64 serialVersionUID = 6464298989504059473L;
};

} // namespace Sql
} // namespace Elastos

#endif // __CDATATRUNCATION_H__
