
#ifndef __LIBCORE_NET_URI_FILEURLCONNECTION_H__
#define __LIBCORE_NET_URI_FILEURLCONNECTION_H__

#include "URLConnection.h"

using Elastos::Net::IURLConnection;
using Elastos::Net::IURL;
using Elastos::Net::URLConnection;
using Elastos::IO::IInputStream;
using Elastos::IO::IFilePermission;
using Elastos::IO::IFile;

namespace Libcore {
namespace Net {
namespace Url {

class FileURLConnection
    : public URLConnection
    , public IFileURLConnection
{
public:
    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ IURL* url);

    CARAPI Connect();

    CARAPI GetHeaderField(
        /* [in] */ const String& key,
        /* [out] */ String* value);

    CARAPI GetHeaderFieldKey(
        /* [in] */ Int32 posn,
        /* [out] */ String* key);

    CARAPI GetHeaderField(
        /* [in] */ Int32 pos,
        /* [out] */ String* value);

    virtual CARAPI GetHeaderFields(
        /* [out] */ IMap** headerFields);

    CARAPI GetContentLength(
        /* [out] */ Int32* length);

    CARAPI GetContentType(
        /* [out] */ String* type);

    CARAPI GetInputStream(
        /* [out] */ IInputStream** is);

    // public java.security.Permission getPermission();

private:
    CARAPI_(AutoPtr<IInputStream>) GetDirectoryListing(
        /* [in] */ IFile* f);

    CARAPI_(String) GetContentTypeForPlainFiles();

private:
    String mFilename;

    AutoPtr<IInputStream> mIs;

    Int32 mLength; // = -1;

    Boolean mIsDir;

    AutoPtr<IFilePermission> mPermission;
};

} // namespace Url
} // namespace Net
} // namespace Libcore

#endif //__LIBCORE_NET_URI_FILEURLCONNECTION_H__
