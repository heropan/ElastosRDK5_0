
#ifndef __ELASTOS_DROID_SERVER_NOTIFICATION_RANKINGHELPER_H__
#define __ELASTOS_DROID_SERVER_NOTIFICATION_RANKINGHELPER_H__

#include <Elastos.CoreLibrary.External.h>
#include <_Elastos.Droid.Server.h>
#include "elastos/droid/server/notification/ManagedServices.h"
#include "elastos/droid/server/notification/GlobalSortKeyComparator.h"
#include "elastos/droid/server/notification/NotificationComparator.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IArrayMap;
using Elastos::Core::ICharSequence;
using Elastos::IO::IPrintWriter;
using Org::Xmlpull::V1::IXmlSerializer;
using Org::Xmlpull::V1::IXmlPullParser;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Notification {

class NotificationRecord;

class RankingHelper
    : public Object
    , public IRankingConfig
{
public:
    CAR_INTERFACE_DECL();

    RankingHelper(
        /* [in] */ IContext* context,
        /* [in] */ IHandler* rankingHandler,
        /* [in] */ ArrayOf<String>* extractorNames);

    ~RankingHelper();

    CARAPI_(AutoPtr<INotificationSignalExtractor>) FindExtractor(
        /* [in] */ const InterfaceID& extractorClass);

    CARAPI ExtractSignals(
        /* [in] */ NotificationRecord* r);

    CARAPI ReadXml(
        /* [in] */ IXmlPullParser* parser);

    CARAPI WriteXml(
        /* [in] */ IXmlSerializer* out);

    CARAPI Sort(
        /* [in] */ IArrayList* notificationList);

    CARAPI_(Int32) IndexOf(
        /* [in] */ IArrayList* notificationList,
        /* [in] */ NotificationRecord* target);

    // @Override
    CARAPI GetPackagePriority(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid,
        /* [out] */ Int32* priority);

    // @Override
    CARAPI SetPackagePriority(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 priority);

    // @Override
    CARAPI GetPackageVisibilityOverride(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid,
        /* [out] */ Int32* visibility);

    // @Override
    CARAPI SetPackageVisibilityOverride(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 visibility);

    CARAPI_(void) Dump(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& prefix,
        /* [in] */ DumpFilter* filter);

private:
    CARAPI_(void) UpdateConfig();

    static CARAPI SafeInt32(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ const String& att,
        /* [in] */ Int32 defValue,
        /* [out] */ Int32* result);

    static CARAPI TryParseInt32(
        /* [in] */ const String& value,
        /* [in] */ Int32 defValue,
        /* [out] */ Int32* result);

private:
    static const String TAG;
    static const Boolean DEBUG;

    static const Int32 XML_VERSION;

    static const String TAG_RANKING;
    static const String TAG_PACKAGE;
    static const String ATT_VERSION;

    static const String ATT_NAME;
    static const String ATT_UID;
    static const String ATT_PRIORITY;
    static const String ATT_VISIBILITY;

    AutoPtr< ArrayOf<INotificationSignalExtractor*> > mSignalExtractors;
    AutoPtr<NotificationComparator> mPreliminaryComparator;// = new NotificationComparator();
    AutoPtr<GlobalSortKeyComparator> mFinalComparator;// = new GlobalSortKeyComparator();

    // Package name to uid, to priority. Would be better as Table<String, Int, Int>
    /* ArrayMap<String, SparseIntArray> mPackagePriorities;*/
    AutoPtr<IArrayMap> mPackagePriorities;
    /* ArrayMap<String, SparseIntArray> mPackageVisibilities;*/
    AutoPtr<IArrayMap> mPackageVisibilities;
    /* ArrayMap<String, NotificationRecord> mProxyByGroupTmp;*/
    AutoPtr<IArrayMap> mProxyByGroupTmp;

    AutoPtr<IContext> mContext;
    AutoPtr<IHandler> mRankingHandler;
};

} // Notification
} // Server
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_SERVER_NOTIFICATION_RANKINGHELPER_H__
