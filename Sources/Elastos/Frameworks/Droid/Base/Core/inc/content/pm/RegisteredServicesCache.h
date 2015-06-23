
#ifndef __REGISTEREDSERVICESCACHE_H__
#define __REGISTEREDSERVICESCACHE_H__

#include "ext/frameworkext.h"
#include "content/BroadcastReceiver.h"
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Etl::List;
using Elastos::Core::IInteger32;
using Elastos::Core::IRunnable;
using Elastos::Core::Mutex;
using Elastos::IO::IFileDescriptor;
using Elastos::IO::IPrintWriter;
using Elastos::IO::IFileInputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Utility::IAtomicFile;
using Elastos::Droid::Utility::IAttributeSet;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

/**
 * Cache of registered services. This cache is lazily built by interrogating
 * {@link PackageManager} on a per-user basis. It's updated as packages are
 * added, removed and changed. Users are responsible for calling
 * {@link #invalidateCache(int)} when a user is started, since
 * {@link PackageManager} broadcasts aren't sent for stopped users.
 * <p>
 * The services are referred to by type V and are made available via the
 * {@link #getServiceInfo} method.
 *
 * @hide
 */
class RegisteredServicesCache : public ElRefBase
{
public:
    /**
     * Value type that describes a Service. The information within can be used
     * to bind to the service.
     */
    class ServiceInfo : public ElRefBase
    {
    public:
        /** @hide */
        ServiceInfo(
            /* [in] */ IInterface* type,
            /* [in] */ IComponentName* componentName,
            /* [in] */ Int32 uid);

        CARAPI_(String) ToString();

    public:
        AutoPtr<IInterface> mType;
        AutoPtr<IComponentName> mComponentName;
        Int32 mUid;
    };

private:
    class UserServices : public ElRefBase
    {
    public:
        UserServices();

        ~UserServices();

    public:
        // @GuardedBy("mServicesLock")
        HashMap<AutoPtr<IInterface>, AutoPtr<IInteger32> > mPersistentServices;

        // @GuardedBy("mServicesLock")
        AutoPtr<HashMap<AutoPtr<IInterface>, AutoPtr<ServiceInfo> > > mServices;
    };

    class PackageReceiver : public BroadcastReceiver
    {
    public:
        PackageReceiver(
            /* [in] */ RegisteredServicesCache* parent);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("RegisteredServicesCache::PackageReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        RegisteredServicesCache* mParent;
    };

    class ExternalReceiver : public BroadcastReceiver
    {
    public:
        ExternalReceiver(
            /* [in] */ RegisteredServicesCache* parent);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("RegisteredServicesCache::ExternalReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        RegisteredServicesCache* mParent;
    };

    class ListenerRunnable
        : public ElRefBase
        , public IRunnable
    {
    public:
        CAR_INTERFACE_DECL();

        ListenerRunnable(
            /* [in] */ IInterface* type,
            /* [in] */ Int32 userId,
            /* [in] */ Boolean removed,
            /* [in] */ IRegisteredServicesCacheListener* listener);

        CARAPI Run();

    private:
        AutoPtr<IInterface> mType;

        Int32 mUserId;

        Boolean mRemoved;

        AutoPtr<IRegisteredServicesCacheListener> mListener;
    };

public:
    RegisteredServicesCache(
        /* [in] */ IContext* context,
        /* [in] */ const String& interfaceName,
        /* [in] */ const String& metaDataName,
        /* [in] */ const String& attributeName,
        /* [in] */ IXmlSerializerAndParser* serializerAndParser);

    CARAPI_(void) InvalidateCache(
        /* [in] */ Int32 userId);

    CARAPI_(void) Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* fout,
        /* [in] */ ArrayOf<String>* args,
        /* [in] */ Int32 userId);

    CARAPI_(AutoPtr<IRegisteredServicesCacheListener>) GetListener();

    CARAPI_(void) SetListener(
        /* [in] */ IRegisteredServicesCacheListener* listener,
        /* [in] */ IHandler* handler);

    /**
     * Accessor for the registered authenticators.
     * @param type the account type of the authenticator
     * @return the AuthenticatorInfo that matches the account type or null if none is present
     */
    CARAPI_(AutoPtr<ServiceInfo>) GetServiceInfo(
        /* [in] */ IInterface* type,
        /* [in] */ Int32 userId);

    /**
     * @return a collection of {@link RegisteredServicesCache.ServiceInfo} objects for all
     * registered authenticators.
     */
    CARAPI_(AutoPtr< List<AutoPtr<ServiceInfo> > >) GetAllServices(
        /* [in] */ Int32 userId);

    virtual CARAPI ParseServiceAttributes(
            /* [in] */ IResources* res,
            /* [in] */ const String& packageName,
            /* [in] */ IAttributeSet* attrs,
            /* [out] */ IInterface** attributes) = 0;

private:
    CARAPI_(AutoPtr<UserServices>) FindOrCreateUserLocked(
        /* [in] */ Int32 userId);

    CARAPI_(void) NotifyListener(
        /* [in] */ IInterface* type,
        /* [in] */ Int32 userId,
        /* [in] */ Boolean removed);

    CARAPI_(Boolean) InSystemImage(
        /* [in] */ Int32 callerUid);

    /**
     * Populate {@link UserServices#services} by scanning installed packages for
     * given {@link UserHandle}.
     */
    CARAPI_(void) GenerateServicesMap(
        /* [in] */ Int32 userId);

    CARAPI_(Boolean) ContainsType(
        /* [in] */ List<AutoPtr<ServiceInfo> >* serviceInfos,
        /* [in] */ IInterface* type);

    CARAPI_(Boolean) ContainsTypeAndUid(
        /* [in] */ List<AutoPtr<ServiceInfo> >* serviceInfos,
        /* [in] */ IInterface* type,
        /* [in] */ Int32 uid);

    CARAPI ParseServiceInfo(
        /* [in] */ IResolveInfo* service,
        /* [out] */ ServiceInfo** info);

    /**
     * Read all sync status back in to the initial engine state.
     */
    CARAPI_(void) ReadPersistentServicesLocked();

    /**
     * Write all sync status to the sync status file.
     */
    CARAPI_(void) WritePersistentServicesLocked();

    CARAPI ErrorCheckAndClose(
        /* [in] */ ECode ec,
        /* [in] */ IFileInputStream* fos);

    CARAPI ErrorCheckAndFailWrite(
        /* [in] */ ECode ec,
        /* [in] */ IAtomicFile* atomicFile,
        /* [in] */ IFileOutputStream* fis);

public:
    AutoPtr<IContext> mContext;

private:
    static const String TAG;

    String mInterfaceName;
    String mMetaDataName;
    String mAttributesName;
    AutoPtr<IXmlSerializerAndParser> mSerializerAndParser;

    Mutex mServicesLock;

    // @GuardedBy("mServicesLock")
    Boolean mPersistentServicesFileDidNotExist;

    // @GuardedBy("mServicesLock")
    HashMap<Int32, AutoPtr<UserServices> > mUserServices;

    /**
     * This file contains the list of known services. We would like to maintain this forever
     * so we store it as an XML file.
     */
    AutoPtr<IAtomicFile> mPersistentServicesFile;

    // the listener and handler are synchronized on "this" and must be updated together
    AutoPtr<IRegisteredServicesCacheListener> mListener;
    AutoPtr<IHandler> mHandler;

    AutoPtr<IBroadcastReceiver> mPackageReceiver;

    AutoPtr<IBroadcastReceiver> mExternalReceiver;

    Mutex mLock;
};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __REGISTEREDSERVICESCACHE_H__
