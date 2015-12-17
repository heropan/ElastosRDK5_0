
#ifndef __ELASTOS_DROID_SERVER_CRECOGNITIONMANAGERSERVICE_H__
#define __ELASTOS_DROID_SERVER_CRECOGNITIONMANAGERSERVICE_H__

#include "_Elastos_Droid_Server_CRecognitionManagerService.h"
#include "elastos/droid/ext/frameworkdef.h"
#include "Elastos.Droid.Server.h"
#include "elastos/droid/content/PackageMonitor.h"
#include "elastos/droid/content/BroadcastReceiver.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IIPackageManager;
using Elastos::Droid::Internal::Content::PackageMonitor;

namespace Elastos {
namespace Droid {
namespace Server {

CarClass(CRecognitionManagerService)
{
private:
    class MyPackageMonitor : public PackageMonitor
    {
    public:
        MyPackageMonitor(
            /* [in] */ CRecognitionManagerService* owner);

        CARAPI OnSomePackagesChanged();
    private:
        CRecognitionManagerService* mOwner;
    };

    class MyBroadcastReceiver : public BroadcastReceiver
    {
    public:
        MyBroadcastReceiver(
            /* [in] */ CRecognitionManagerService* Owner);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CRecognitionManagerService::MyBroadcastReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CRecognitionManagerService* mOwner;
    };

    CARAPI InitForUser(
        /* [in] */ Int32 userHandle);

public:
    CARAPI SystemReady();

    CARAPI FindAvailRecognizer(
        /* [in] */ const String& prefPackage,
        /* [in] */ Int32 userHandle,
        /* [out] */ IComponentName** componentName);

    CARAPI GetCurRecognizer(
        /* [in] */ Int32 userHandle,
        /* [out] */ IComponentName** componentName);

    CARAPI SetCurRecognizer(
        /* [in] */ IComponentName* comp,
        /* [in] */ Int32 userHandle);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI constructor(
        /* [in] */ IContext* context);

private:
    static const String TAG;
    AutoPtr<IContext> mContext;
    AutoPtr<MyPackageMonitor> mMonitor;
    AutoPtr<IIPackageManager> mIPm;
    static const Boolean DEBUG;
    AutoPtr<MyBroadcastReceiver> mBroadcastReceiver;

};

}
}
}

#endif // __ELASTOS_DROID_SERVER_CRECOGNITIONMANAGERSERVICE_H__
