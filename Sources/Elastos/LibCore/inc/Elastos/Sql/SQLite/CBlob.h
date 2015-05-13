
#ifndef __CBLOB_H__
#define __CBLOB_H__

#include "_CBlob.h"

using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CBlob)
{
public:
    CBlob();
    CARAPI GetInputStream(
        /* [out] */ IInputStream** instream);

    CARAPI GetOutputStream(
        /* [out] */ IOutputStream** outstream);

    CARAPI Close();

public:

    CARAPI_(Int32) Write(
        /* [in] */ ArrayOf<Byte>* b ,
        /* [in] */ Int32 off ,
        /* [in] */ Int32 pos ,
        /* [in] */ Int32 len );

    CARAPI_(Int32) Read(
        /* [in] */ ArrayOf<Byte>* b ,
        /* [in] */ Int32 off ,
        /* [in] */ Int32 pos ,
        /* [in] */ Int32 len );
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