
#ifndef __SERVICEMANAGER_H__
#define __SERVICEMANAGER_H__

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#include <elastos.h>
#include <elastos/HashMap.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using Elastos::Core::Threading::Mutex;
using Elastos::Utility::HashMap;

class ServiceManager : public ElLightRefBase
{
public:
    static CARAPI_(AutoPtr<ServiceManager>) GetInstance();

    CARAPI AddService(
        /* [in] */ const String& name,
        /* [in] */ IInterface* service);

    CARAPI GetService(
        /* [in] */ const String& name,
        /* [out] */ IInterface** service);

private:
    ServiceManager();

    CARAPI_(Boolean) ConnectServer();

private:
    static AutoPtr<ServiceManager> sInstance;

    Boolean mIsConnected;
    Mutex mConnectedLock;
    String mServerIpAddress;
    Int32 mServerPort;
};

#endif //__SERVICEMANAGER_H__
