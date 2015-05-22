
#ifndef __FILEURLCONNECTION_H__
#define __FILEURLCONNECTION_H__

#include "Elastos.CoreLibrary_server.h"
#include "URLConnection.h"

using Elastos::Net::IURLConnection;
using Elastos::Net::IURL;
using Elastos::IO::IInputStream;
using Elastos::IO::IFilePermission;
using Elastos::IO::IFile;

namespace Elastos {
namespace Net {
namespace Url {

class FileURLConnection : public URLConnection
{
public:
    CARAPI Init(
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
} // namespace Elastos

#endif //__FILEURLCONNECTION_H__
