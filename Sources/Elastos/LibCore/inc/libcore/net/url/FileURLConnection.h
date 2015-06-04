
#ifndef __LIBCORE_NET_URI_FILEURLCONNECTION_H__
#define __LIBCORE_NET_URI_FILEURLCONNECTION_H__

#include "Elastos.CoreLibrary_server.h"
#include "URLConnection.h"

using Elastos::Net::IURLConnection;
using Elastos::Net::IURL;
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
