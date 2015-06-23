
#ifndef __DEVICEMONITOR_H__
#define __DEVICEMONITOR_H__

#include "ext/frameworkext.h"
#include <elastos/Core/Object.h>

using Elastos::IO::IFile;
using Elastos::IO::ICloseable;
using Elastos::IO::IOutputStream;
using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class DeviceMonitor
    : public ElRefBase
{
private:
    class MonitorThread
        : public ThreadBase
    {
    public:
        MonitorThread(
            /* [in] */ DeviceMonitor* host)
            : mHost(host)
        {}

        CARAPI Run();

    private:
        DeviceMonitor* mHost;
    };

private:
    DeviceMonitor();

    static CARAPI_(void) Init();

    CARAPI_(void) Monitor();

    CARAPI_(void) Purge();

    CARAPI_(void) Dump();

    static CARAPI_(Boolean) IsProcessDirectory(
        /* [in] */ IFile* file);

    CARAPI_(void) Dump(
        /* [in] */ IFile* from,
        /* [in] */ IOutputStream* out);

    static CARAPI_(void) WriteHeader(
        /* [in] */ IFile* file,
        /* [in] */ IOutputStream* out);

    static CARAPI_(void) CloseQuietly(
        /* [in] */ ICloseable* closeable);

    CARAPI_(void) Pause();

    CARAPI_(void) Stop();

    CARAPI_(void) WaitForStart();

    CARAPI_(void) StartMonitoring();

    static CARAPI_(void) Start();

public:
    static const String TAG;

    /** Number of samples to take. */
    static const Int32 SAMPLE_COUNT = 10;

    /** Time to wait in ms between samples. */
    static const Int32 INTERVAL = 1000;

    /** Time to wait in ms between samples. */
    static const Int32 MAX_FILES = 30;

    AutoPtr<ArrayOf<Byte> > mBuffer;

    /** Is the monitor currently running? */
    Boolean mRunning;

    static AutoPtr<IFile> PROC;
    static AutoPtr<IFile> BASE;
    static AutoPtr<ArrayOf<IFile*> > PATHS;

    static AutoPtr<DeviceMonitor> instance;

    Object mLock;
};

}
}
}
}
#endif //__DEVICEMONITOR_H__
