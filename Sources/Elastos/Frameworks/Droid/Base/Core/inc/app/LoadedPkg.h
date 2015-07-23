
#ifndef __LOADEDPKG_H__
#define __LOADEDPKG_H__

#include "ext/frameworkext.h"
#include "app/CActivityThread.h"
#include "content/BroadcastReceiver.h"
#include "os/Runnable.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::IO::IFile;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::IIntentReceiver;
using Elastos::Droid::Content::IServiceConnection;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Res::IAssetManager;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::ICompatibilityInfo;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IPendingResult;
using Elastos::Droid::View::ICompatibilityInfoHolder;


namespace Elastos{
namespace Droid{
namespace App{

extern "C" const InterfaceID EIID_LoadedPkg;
extern "C" const InterfaceID EIID_ReceiverDispatcher;

class CInnerConnection;

class LoadedPkg
    : public ElRefBase
    , public IWeakReferenceSource
{
public:
    class ReceiverDispatcher
        : public ElRefBase
        , public IWeakReferenceSource
    {
    public:
        class Args
            : public ElRefBase
            , public IPendingResult
            , public BroadcastReceiver::PendingResult
            , public IRunnable
        {
        public:
            Args(
                /* [in] */ IIntent* intent,
                /* [in] */ Int32 resultCode,
                /* [in] */ const String& resultData,
                /* [in] */ IBundle* resultExtras,
                /* [in] */ Boolean ordered,
                /* [in] */ Boolean sticky,
                /* [in] */ Int32 sendingUser,
                /* [in] */ ReceiverDispatcher* rd);

            CARAPI Run();

            CARAPI_(PInterface) Probe(
                /* [in]  */ REIID riid);

            CARAPI_(UInt32) AddRef();

            CARAPI_(UInt32) Release();

            CARAPI GetInterfaceID(
                /* [in] */ IInterface *pObject,
                /* [out] */ InterfaceID *pIID);

            /**
             * Version of {@link BroadcastReceiver#setResultCode(int)
             * BroadcastReceiver.setResultCode(int)} for
             * asynchronous broadcast handling.
             */
            CARAPI SetResultCode(
                /* [in] */ Int32 code);

            /**
             * Version of {@link BroadcastReceiver#getResultCode()
             * BroadcastReceiver.getResultCode()} for
             * asynchronous broadcast handling.
             */
            CARAPI GetResultCode(
                /* [out] */ Int32* resultCode);

            /**
             * Version of {@link BroadcastReceiver#setResultData(String)
             * BroadcastReceiver.setResultData(String)} for
             * asynchronous broadcast handling.
             */
            CARAPI SetResultData(
                /* [in] */ const String& data);

            /**
             * Version of {@link BroadcastReceiver#getResultData()
             * BroadcastReceiver.getResultData()} for
             * asynchronous broadcast handling.
             */
            CARAPI GetResultData(
                /* [out] */ String* resultData);

            /**
             * Version of {@link BroadcastReceiver#setResultExtras(Bundle)
             * BroadcastReceiver.setResultExtras(Bundle)} for
             * asynchronous broadcast handling.
             */
            CARAPI SetResultExtras(
                /* [in] */ IBundle* extras);

            /**
             * Version of {@link BroadcastReceiver#getResultExtras(boolean)
             * BroadcastReceiver.getResultExtras(boolean)} for
             * asynchronous broadcast handling.
             */
            CARAPI GetResultExtras(
                /* [in] */ Boolean makeMap,
                /* [out] */ IBundle** resultExtras);

            /**
             * Version of {@link BroadcastReceiver#setResult(int, String, Bundle)
             * BroadcastReceiver.setResult(int, String, Bundle)} for
             * asynchronous broadcast handling.
             */
            CARAPI SetResult(
                /* [in] */ Int32 code,
                /* [in] */ const String& data,
                /* [in] */ IBundle* extras);

            /**
             * Version of {@link BroadcastReceiver#getAbortBroadcast()
             * BroadcastReceiver.getAbortBroadcast()} for
             * asynchronous broadcast handling.
             */
            CARAPI GetAbortBroadcast(
                /* [out] */ Boolean* isAborted);

            /**
             * Version of {@link BroadcastReceiver#abortBroadcast()
             * BroadcastReceiver.abortBroadcast()} for
             * asynchronous broadcast handling.
             */
            CARAPI AbortBroadcast();

            /**
             * Version of {@link BroadcastReceiver#clearAbortBroadcast()
             * BroadcastReceiver.clearAbortBroadcast()} for
             * asynchronous broadcast handling.
             */
            CARAPI ClearAbortBroadcast();

            /**
             * Finish the broadcast.  The current result will be sent and the
             * next broadcast will proceed.
             */
            CARAPI Finish();

            /** @hide */
            CARAPI SetExtrasClassLoader(
                /* [in] */ IClassLoader* cl);

            /** @hide */
            CARAPI SendFinished(
                /* [in] */ IIActivityManager* am);

            /** @hide */
            CARAPI GetSendingUserId(
                /* [out] */ Int32* userId);

            CARAPI GetOrderedHint(
                /* [out] */ Boolean* orderedHint);

            CARAPI GetInitialStickyHint(
                /* [out] */ Boolean* initialStickyHint);

        public:
            AutoPtr<IIntent> mCurIntent;
            Boolean mOrdered;
            AutoPtr<ReceiverDispatcher> mHost;
        };

    public:
        ReceiverDispatcher(
            /* [in] */ IBroadcastReceiver* receiver,
            /* [in] */ IContext* context,
            /* [in] */ IHandler* handler,
            /* [in] */ IInstrumentation* instrumentation,
            /* [in] */ Boolean registered);

        ~ReceiverDispatcher();

        CAR_INTERFACE_DECL();

        CARAPI GetWeakReference(
            /* [out] */ IWeakReference** weakReference);

        CARAPI Validate(
           /* [in] */ IContext* context,
           /* [in] */ IHandler* handler);

        //CARAPI GetLocation(
        //    /* [out] */ IIntentReceiverLeaked* location);

        CARAPI_(AutoPtr<IBroadcastReceiver>) GetIntentReceiver();

        CARAPI_(AutoPtr<IIntentReceiver>) GetIIntentReceiver();

        CARAPI_(void) PerformReceive(
            /* [in] */ IIntent* intent,
            /* [in] */ Int32 resultCode,
            /* [in] */ const String& data,
            /* [in] */ IBundle* extras,
            /* [in] */ Boolean ordered,
            /* [in] */ Boolean sticky,
            /* [in] */ Int32 sendingUser);

    public:
        IIntentReceiver* mIIntentReceiver;
        AutoPtr<IBroadcastReceiver> mReceiver;
        AutoPtr<IContext> mContext;
        AutoPtr<IHandler> mActivityThread;
        AutoPtr<IInstrumentation> mInstrumentation;
        Boolean mRegistered;
        //AutoPtr<IIntentReceiverLeaked> mLocation;
        Boolean mForgotten;
        Boolean mHasUsed;
    };

    class ServiceDispatcher
        : public ElRefBase
        , public IWeakReferenceSource
    {
    private:
        class ConnectionInfo : public ElRefBase
        {
        public:
            AutoPtr<IBinder> mBinder;
            AutoPtr<IProxyDeathRecipient> mDeathMonitor;
        };

        class RunConnection
            : public Runnable
        {
        public:
            RunConnection(
               /* [in] */ ServiceDispatcher* serviceDispatcher,
               /* [in] */ IComponentName* name,
               /* [in] */ IBinder* service,
               /* [in] */ Int32 command);

            virtual CARAPI Run();

        private:
            AutoPtr<ServiceDispatcher> mServiceDispatcher;
            AutoPtr<IComponentName> mName;
            AutoPtr<IBinder> mService;
            Int32 mCommand;
        };

        class DeathMonitor
            : public ElRefBase
            , public IProxyDeathRecipient
        {
        public:
            CAR_INTERFACE_DECL()

            DeathMonitor(
                /* [in] */ IComponentName* name,
                /* [in] */ IBinder* service,
                /* [in] */ ServiceDispatcher* owner);

            //@Override
            CARAPI ProxyDied();

        private:
            AutoPtr<IComponentName> mName;
            AutoPtr<IBinder> mService;
            AutoPtr<IWeakReference> mOwner;
        };

    public:
        CAR_INTERFACE_DECL()

        CARAPI GetWeakReference(
            /* [out] */ IWeakReference** weakReference);

        ServiceDispatcher(
           /* [in] */ IServiceConnection* conn,
           /* [in] */ IContext* context,
           /* [in] */ IHandler* activityThread,
           /* [in] */ Int32 flags);

        CARAPI Validate(
           /* [in] */ IContext* context,
           /* [in] */ IHandler* handler);

        CARAPI_(void) DoForget();

        CARAPI_(AutoPtr<IServiceConnection>) GetServiceConnection();

        CARAPI_(AutoPtr<IIServiceConnection>) GetIServiceConnection();

        CARAPI_(Int32) GetFlags();

        CARAPI_(void) Connected(
            /* [in] */ IComponentName* name,
            /* [in] */ IBinder* service);

        CARAPI DoConnected(
            /* [in] */ IComponentName* name,
            /* [in] */ IBinder* service);

        CARAPI Death(
            /* [in] */ IComponentName* name,
            /* [in] */ IBinder* service);

        CARAPI DoDeath(
            /* [in] */ IComponentName* name,
            /* [in] */ IBinder* service);

    private:
        AutoPtr<CInnerConnection> mIServiceConnection;
        AutoPtr<IServiceConnection> mConnection;
        AutoPtr<IContext> mContext;
        AutoPtr<IHandler> mActivityThread;
        // private final ServiceConnectionLeaked mLocation;
        Int32 mFlags;
        Boolean mDied;
        Boolean mForgotten;
        HashMap<AutoPtr<IComponentName>, AutoPtr<ConnectionInfo> > mActiveConnections;
        Object mLock;
    };

public:
    LoadedPkg(
        /* [in] */ CActivityThread* activityThread,
        /* [in] */ IApplicationInfo* aInfo,
        /* [in] */ ICompatibilityInfo* compatInfo,
        /* [in] */ IActivityThread* mainActivityThread,
        /* [in] */ IClassLoader* baseLoader,
        /* [in] */ Boolean securityViolation,
        /* [in] */ Boolean includeCode);

    LoadedPkg(
        /* [in] */ CActivityThread* activityThread,
        /* [in] */ const String& name,
        /* [in] */ IContext* systemContext,
        /* [in] */ IApplicationInfo* info,
        /* [in] */ ICompatibilityInfo* compatInfo);

    ~LoadedPkg();

    CAR_INTERFACE_DECL();

    CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);

    CARAPI_(AutoPtr<IApplication>) GetApplication();

    CARAPI GetPackageName(
        /* [out] */ String* packageName);

    CARAPI GetApplicationInfo(
        /* [out] */ IApplicationInfo** info);

    CARAPI_(Boolean) IsSecurityViolation();

    CARAPI GetClassLoader(
        /* [out] */ IClassLoader** loader);

    CARAPI GetAppDir(
        /* [out] */ String* appDir);

    CARAPI GetLibDir(
        /* [out] */ String* appDir);

    CARAPI GetResDir(
        /* [out] */ String* resDir);

    CARAPI GetDataDir(
        /* [out] */ String* dataDir);

    CARAPI GetDataDirFile(
        /* [out] */ IFile** dirFile);

    CARAPI GetAssets(
        /* [in] */ IActivityThread* mainThread,
        /* [out] */ IAssetManager** manager);

    CARAPI GetResources(
        /* [in] */ IActivityThread* activityThread,
        /* [out] */ IResources** res);

    CARAPI MakeApplication(
        /* [in] */ Boolean forceDefaultAppClass,
        /* [in] */ IInstrumentation* instrumentation,
        /* [out] */ IApplication** app);

    CARAPI RemoveContextRegistrations(
        /* [in] */ IContext* context,
        /* [in] */ const String& who,
        /* [in] */ const String& what);

    CARAPI GetReceiverDispatcher(
        /* [in] */ IBroadcastReceiver* c,
        /* [in] */ IContext* context,
        /* [in] */ IHandler* handler,
        /* [in] */ IInstrumentation* instrumentation,
        /* [in] */ Boolean registered,
        /* [out] */ IIntentReceiver** ir);

    CARAPI ForgetReceiverDispatcher(
        /* [in] */ IContext* context,
        /* [in] */ IBroadcastReceiver* r,
        /* [out] */ IIntentReceiver** ir);

    CARAPI  GetServiceDispatcher(
        /* [in] */ IServiceConnection* c,
        /* [in] */ IContext* context,
        /* [in] */ IHandler* handler,
        /* [in] */ Int32 flags,
        /* [out] */ IIServiceConnection** result);

    CARAPI ForgetServiceDispatcher(
        /* [in] */ IContext* context,
        /* [in] */ IServiceConnection* c,
        /* [out] */ IIServiceConnection** result);

public:
    String mPackageName;
    AutoPtr<IResources> mResources;
    AutoPtr<ICompatibilityInfoHolder> mCompatibilityInfo;

private:
    static const String TAG;
    Object mLock;

    // CActivityThread has this's reference
    CActivityThread* mActivityThread;
    AutoPtr<IApplicationInfo> mApplicationInfo;
    String mAppDir;
    String mResDir;
    AutoPtr<ArrayOf<String> > mSharedLibraries;
    String mDataDir;
    String mLibDir;
    AutoPtr<IFile> mDataDirFile;
    AutoPtr<IClassLoader> mBaseClassLoader;
    Boolean mSecurityViolation;
    Boolean mIncludeCode;
    AutoPtr<IClassLoader> mClassLoader;
    AutoPtr<IApplication> mApplication;

    typedef HashMap<AutoPtr<IBroadcastReceiver>, AutoPtr<ReceiverDispatcher> > ReceiverMap;
    typedef typename ReceiverMap::Iterator ReceiverMapIterator;
    HashMap<AutoPtr<IContext>, AutoPtr<ReceiverMap> > mReceivers;
    HashMap<AutoPtr<IContext>, AutoPtr<ReceiverMap> > mUnregisteredReceivers;
    Object mReceiversLock;

    typedef HashMap<AutoPtr<IServiceConnection>, AutoPtr<ServiceDispatcher> > ServiceMap;
    typedef typename ServiceMap::Iterator ServiceMapIterator;
    HashMap<AutoPtr<IContext>, AutoPtr<ServiceMap> > mServices;
    HashMap<AutoPtr<IContext>, AutoPtr<ServiceMap> > mUnboundServices;
    Object mServicesLock;

    Int32 mClientCount;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__LOADEDPKG_H__
