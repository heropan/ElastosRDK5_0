
#ifndef __ELASTOS_DROID_PROVIDER_CDOWNLOADS_H__
#define __ELASTOS_DROID_PROVIDER_CDOWNLOADS_H__

#include "_CDownloads.h"

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CDownloads)
{
public:
    CARAPI constructor();

    /**
     * Delete all the downloads for a package/class pair.
     */
    CARAPI RemoveAllDownloadsByPackage(
        /* [in] */ IContext* context,
        /* [in] */ const String& notification_package,
        /* [in] */ const String& notification_class);

private:
    /**
     * Query where clause for general querying.
     */
    static const String QUERY_WHERE_CLAUSE;
};

} //namespace Provider
} //namespace Droid
} //namespace Elastos

#endif //__ELASTOS_DROID_PROVIDER_CDOWNLOADS_H__
