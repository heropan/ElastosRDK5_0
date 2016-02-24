#include "ServiceManager.h"
#include <sys/types.h>
#include <unistd.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <stdio.h>

int main(int argv, char**argc)
{
    android::sp<android::ProcessState> proc(android::ProcessState::self());
    android::sp<android::IServiceManager> sm = android::defaultServiceManager();
    int res = sm->addService(android::String16(ELASTOS_SERVICEMGR_NAME), new ServiceManager());
    if (res != 0) {
        ALOGE("<%s, %d> add service '%s' failed, res=%d\n",
            __FILE__, __LINE__, ELASTOS_SERVICEMGR_NAME, res);
        return -1;
    }
    else {
        ALOGE("<%s, %d> add service '%s' succeeded.\n",
            __FILE__, __LINE__, ELASTOS_SERVICEMGR_NAME);
    }

    android::IPCThreadState::self()->joinThreadPool();
    return 0;
}
