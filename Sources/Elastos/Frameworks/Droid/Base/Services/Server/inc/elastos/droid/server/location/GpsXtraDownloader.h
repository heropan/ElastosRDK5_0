
#ifndef __ELASTOS_DROID_SERVER_LOCATION_GPSXTRADOWNLOADER_H__
#define __ELASTOS_DROID_SERVER_LOCATION_GPSXTRADOWNLOADER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "_Elastos.Droid.Server.h"
#include "Elastos.Droid.Content.h"
#include "Elastos.CoreLibrary.Utility.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Content::IContext;
using Elastos::Utility::IProperties;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Location {

/**
 * A class for downloading GPS XTRA data.
 *
 * {@hide}
 */
class GpsXtraDownloader
    : public Object
{
public:
    GpsXtraDownloader(
        /* [in] */ IContext* context,
        /* [in] */ IProperties* properties);

    CARAPI_(AutoPtr<ArrayOf<Byte> >) DownloadXtraData();

protected:
    static CARAPI_(AutoPtr<ArrayOf<Byte> >) DoDownload(
        /* [in] */ const String& url,
        /* [in] */ Boolean isProxySet,
        /* [in] */ const String& proxyHost,
        /* [in] */ Int32 proxyPort);

private:
    static const String TAG;
    static const Boolean DEBUG;
    static const String DEFAULT_USER_AGENT;

    AutoPtr<IContext> mContext;
    AutoPtr<ArrayOf<String> > mXtraServers;
    // to load balance our server requests
    Int32 mNextServerIndex;
    String mUserAgent;
};

} // namespace Location
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_LOCATION_GPSXTRADOWNLOADER_H__
