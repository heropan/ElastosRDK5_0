
#ifndef __CBLOB_H__
#define __CBLOB_H__

#include "_Elastos_Sql_SQLite_CBlob.h"
#include <elastos/core/Object.h>


using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CBlob)
    , public Object
    , public IBlob
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CBlob();

    CARAPI GetInputStream(
        /* [out] */ IInputStream** instream);

    CARAPI GetOutputStream(
        /* [out] */ IOutputStream** outstream);

    CARAPI Close();

public:

    CARAPI Write(
        /* [in] */ ArrayOf<Byte>* b,
        /* [in] */ Int32 off,
        /* [in] */ Int32 pos,
        /* [in] */ Int32 len,
        /* [out] */ Int32 * count);

    CARAPI Read(
        /* [in] */ ArrayOf<Byte>* b,
        /* [in] */ Int32 off,
        /* [in] */ Int32 pos,
        /* [in] */ Int32 len,
        /* [out] */ Int32 * count);
public:
    Int32 size;

private:
    static Boolean Internal_init();

    static Boolean is_init;
    Int64 mHandle;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CBLOB_H__