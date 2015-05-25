
#include "cmdef.h"
#include "CBlob.h"
#include "CBlobR.h"
#include "CBlobW.h"

namespace Elastos {
namespace Sql {
namespace SQLite {

CBlob::CBlob()
{
    mHandle = 0;
    size = 0;
    is_init = FALSE;
}

Boolean CBlob::Internal_init()
{
    if(!is_init)
    {
        is_init = TRUE;
    }

    return is_init;
}

Boolean CBlob::is_init = Internal_init();

ECode CBlob::GetInputStream(
    /* [out] */ IInputStream** instream)
{
    AutoPtr<IBlobR> tmp;
    FAIL_RETURN(CBlobR::New(this, (IBlobR** )&tmp));
    *instream = (IInputStream *)tmp;
    INTERFACE_ADDREF(*instream);
    return NOERROR;
}

ECode CBlob::GetOutputStream(
    /* [out] */ IOutputStream ** outstream)
{
    AutoPtr<IBlobW> tmp;
    FAIL_RETURN(CBlobW::New(this, (IBlobW** )&tmp));
    *outstream = tmp;
    INTERFACE_ADDREF(*outstream);
    return NOERROR;
}

ECode CBlob::Close()
{
#if HAVE_SQLITE3 && HAVE_SQLITE3_INCRBLOBIO
    hbl *bl = mHandle;

    if (bl) {
    if (bl->h) {
        handle *h = bl->h;
        hbl *blc, **blp;

        blp = &h->blobs;
        blc = *blp;
        while (blc) {
        if (blc == bl) {
            *blp = blc->next;
            break;
        }
        blp = &blc->next;
        blc = *blp;
        }
    }
    if (bl->blob) {
        sqlite3_blob_close(bl->blob);
    }
    bl->blob = 0;
    free(bl);
    mHandle = 0;
    size = 0;
    }
#endif
    return NOERROR;
}

CARAPI_(Int32) CBlob::Write(
    /* [in] */ ArrayOf<Byte>* b ,
    /* [in] */ Int32 off ,
    /* [in] */ Int32 pos ,
    /* [in] */ Int32 len )
{
#if HAVE_SQLITE3 && HAVE_SQLITE3_INCRBLOBIO
    hbl *bl = mHandle;

    if (bl && bl->h && bl->blob) {
    Int32 ret;

    if (len <= 0) {
        return NOERROR;
    }

    ret = sqlite3_blob_write(bl->blob, b, len, pos);

    if (ret != SQLITE_OK) {

        return NOERROR;
    }
    return len;
    }
#else
   return E_SQL_SQLITE_THROWEX_EXCEPTION;
#endif

    return NOERROR;
}

CARAPI_(Int32) CBlob::Read(
    /* [in] */ ArrayOf<Byte>* b ,
    /* [in] */ Int32 off ,
    /* [in] */ Int32 pos ,
    /* [in] */ Int32 len )
{
#if HAVE_SQLITE3 && HAVE_SQLITE3_INCRBLOBIO
    hbl *bl = mHandle;

    if (bl && bl->h && bl->blob) {
    Int32 ret;

    if (len <= 0) {
        return NOERROR;
    }
    ret = sqlite3_blob_read(bl->blob, b, len, pos);
    if (ret != SQLITE_OK) {
        return NOERROR;
    }
    return len;
    }
    return E_SQL_SQLITE_THROWEX_EXCEPTION;
#else
    return E_SQL_SQLITE_THROWEX_EXCEPTION;
#endif
    return NOERROR;
}

} // namespace SQLite
} // namespace Sql
} // namespace Elastos
