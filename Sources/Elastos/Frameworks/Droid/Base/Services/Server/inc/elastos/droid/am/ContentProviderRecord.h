
#ifndef __ELASTOS_DROID_SERVER_AM_CONTENTPROVIDERRECORD_H__
#define __ELASTOS_DROID_SERVER_AM_CONTENTPROVIDERRECORD_H__

#include "ext/frameworkext.h"
#include "am/CContentProviderConnection.h"
#include <Elastos.Droid.Core.h>
#include <elastos/utility/etl/List.h>
#include <elastos/utility/etl/HashMap.h>
#include <elastos/Core/Object.h>

using Elastos::Utility::Etl::List;
using Elastos::Utility::Etl::HashMap;
using Elastos::Core::ISynchronize;
using Elastos::Core::Object;
using Elastos::IO::IPrintWriter;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Content::IIContentProvider;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Pm::IProviderInfo;
using Elastos::Droid::App::IContentProviderHolder;


namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class CActivityManagerService;

class ContentProviderRecord
    : public ElRefBase
    , public ISynchronize
    , public Object
{
private:
    // This class represents a handle from an external process to a provider.
    class ExternalProcessHandle
        : public ElRefBase
        , public IProxyDeathRecipient
    {
    public:
        CAR_INTERFACE_DECL()

        ExternalProcessHandle(
            /* [in] */ IBinder* token,
            /* [in] */ ContentProviderRecord* owner);

        CARAPI UnlinkFromOwnDeathLocked();

        CARAPI ProxyDied();

        static const String TAG;
        AutoPtr<IBinder> mToken;
        Int32 mAcquisitionCount;
        ContentProviderRecord* mOwner;
    };

public:
    ContentProviderRecord(
        /* [in] */ CActivityManagerService* service,
        /* [in] */ IProviderInfo* info,
        /* [in] */ IApplicationInfo* ai,
        /* [in] */ IComponentName* name,
        /* [in] */ Boolean singleton);

    ContentProviderRecord(
        /* [in] */ ContentProviderRecord* cpr);

    ~ContentProviderRecord();

    CAR_INTERFACE_DECL();

    CARAPI_(AutoPtr<IContentProviderHolder>) NewHolder(
        /* [in] */ CContentProviderConnection* conn);

    CARAPI_(Boolean) CanRunHere(
        /* [in] */ ProcessRecord* app);

    CARAPI_(void) AddExternalProcessHandleLocked(
    /* [in] */ IBinder* token);

    CARAPI_(Boolean) RemoveExternalProcessHandleLocked(
    /* [in] */ IBinder* token);

    CARAPI_(Boolean) HasExternalProcessHandles();

    CARAPI_(void) Dump(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& prefix,
        /* [in] */ Boolean full);

    CARAPI_(String) ToString();

    CARAPI_(String) ToShortString();

    CARAPI Lock();

    CARAPI Unlock();

    CARAPI Wait();

    CARAPI Wait(
        /* [in] */ Int64 millis);

    CARAPI Wait(
        /* [in] */ Int64 millis,
        /* [in] */ Int32 nanos);

    CARAPI Notify();

    CARAPI NotifyAll();

private:
    CARAPI_(void) RemoveExternalProcessHandleInternalLocked(
        /* [in] */ IBinder* token);

public:
    CActivityManagerService* mService;
    AutoPtr<IProviderInfo> mInfo;
    Int32 mUid;
    AutoPtr<IApplicationInfo> mAppInfo;
    AutoPtr<IComponentName> mName;
    // number of non-framework processes supported by this provider
    Boolean mSingleton;
    AutoPtr<IIContentProvider> mProvider;
    Boolean mNoReleaseNeeded;
    // All attached clients
    List< AutoPtr<CContentProviderConnection> > mConnections;
    // Handles for non-framework processes supported by this provider
    typedef HashMap<AutoPtr<IBinder>, AutoPtr<ExternalProcessHandle> > ProcessHandleHashMap;
    typedef typename ProcessHandleHashMap::ValueType ProcessHandleValueType;
    typedef typename ProcessHandleHashMap::Iterator ProcessHandleIterator;
    ProcessHandleHashMap mExternalProcessTokenToHandle;

    // Count for external process for which we have no handles.
    Int32 mExternalProcessNoHandleCount;
    ProcessRecord* mProc; // weak-ref if non-null, hosting process.
    AutoPtr<ProcessRecord> mLaunchingApp; // weak-ref if non-null, waiting for this app to be launched.
    String mStringName;
    String mShortStringName;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos


#define HASH_FUNC_FOR_AUTOPTR_CONTENTPROVIDERRECORD
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Server::Am::ContentProviderRecord)
#endif


#endif //__ELASTOS_DROID_SERVER_AM_CONTENTPROVIDERRECORD_H__
