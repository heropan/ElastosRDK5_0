#ifndef __CCLIPBOARDSERVICE_H__
#define __CCLIPBOARDSERVICE_H__

#include "_CClipboardService.h"
#include "ext/frameworkext.h"
#include "content/BroadcastReceiver.h"
#include <elastos/utility/etl/HashSet.h>
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashSet;
using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IRemoteCallbackList;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::App::IIActivityManager;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::IClipData;
using Elastos::Droid::Content::IClipDataItem;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IClipDescription;
using Elastos::Droid::Content::IOnPrimaryClipChangedListener;


namespace Elastos {
namespace Droid {
namespace Server {

CarClass(CClipboardService)
{
private:
    class PerUserClipboard : public ElRefBase
    {
    public:
        PerUserClipboard(
            /* [in] */ Int32 userId);
    public:
        Int32 mUserId;

        AutoPtr<IRemoteCallbackList> mPrimaryClipListeners;

        AutoPtr<IClipData> mPrimaryClip;

        HashSet<String> mActivePermissionOwners;
    };

    class MyBroadcastReceiver : public BroadcastReceiver
    {
    public:
        MyBroadcastReceiver(
            /* [in] */ CClipboardService* host);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CClipboardService::MyBroadcastReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CClipboardService* mHost;
    };

public:
    /**
     * Instantiates the clipboard.
     */
    CARAPI constructor(
        /* [in] */ IContext *context);

    CARAPI SetPrimaryClip(
        /* [in] */ IClipData* clip);

    CARAPI GetPrimaryClip(
        /* [in] */ const String& pkg,
        /* [out] */ IClipData** clip);

    CARAPI GetPrimaryClipDescription(
        /* [out] */ IClipDescription** description);

    CARAPI HasPrimaryClip(
        /* [out] */ Boolean* hasPrimaryClip);

    CARAPI AddPrimaryClipChangedListener(
        /* [in] */ IOnPrimaryClipChangedListener* listener);

    CARAPI RemovePrimaryClipChangedListener(
        /* [in] */ IOnPrimaryClipChangedListener* listener);

    CARAPI HasClipboardText(
        /* [out] */ Boolean* hasClipboardText);

private:
    CARAPI_(AutoPtr<PerUserClipboard>) GetClipboard();

    CARAPI_(AutoPtr<PerUserClipboard>) GetClipboard(
        /* [in] */ Int32 userId);

    CARAPI_(void) RemoveClipboard(
        /* [in] */ Int32 userId);

    CARAPI CheckUriOwnerLocked(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 uid);

    CARAPI CheckItemOwnerLocked(
        /* [in] */ IClipDataItem* item,
        /* [in] */ Int32 uid);

    CARAPI CheckDataOwnerLocked(
        /* [in] */ IClipData* data,
        /* [in] */ Int32 uid);

    CARAPI_(void) GrantUriLocked(
        /* [in] */ IUri* uri,
        /* [in] */ const String& pkg);

    CARAPI_(void) GrantItemLocked(
        /* [in] */ IClipDataItem* item,
        /* [in] */ const String& pkg);

    CARAPI AddActiveOwnerLocked(
        /* [in] */ Int32 uid,
        /* [in] */ const String& pkg);

    CARAPI_(void) RevokeUriLocked(
        /* [in] */ IUri* uri);

    CARAPI_(void) RevokeItemLocked(
        /* [in] */ IClipDataItem* item);

    CARAPI_(void) ClearActiveOwnersLocked();

private:
    static const String TAG;
    AutoPtr<IContext> mContext;
    AutoPtr<IIActivityManager> mAm;
    AutoPtr<IPackageManager> mPm;
    AutoPtr<IBinder> mPermissionOwner;

    HashMap<Int32, AutoPtr<PerUserClipboard> > mClipboards;
    Object mClipboardsLock;
};

}//namespace Server
}//namesapce Droid
}//namespace Elastos

#endif //__CCLIPBOARDSERVICE_H__
