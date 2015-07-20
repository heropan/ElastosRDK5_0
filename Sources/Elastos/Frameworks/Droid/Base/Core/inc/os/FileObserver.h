
#ifndef __FILEOBSERVER_H__
#define __FILEOBSERVER_H__

#include "ext/frameworkext.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Droid::Os::IFileObserver;
using Elastos::Core::IRunnable;
using Elastos::Utility::Etl::HashMap;

namespace Elastos {
namespace Droid {
namespace Os {

class FileObserver
    : public ElRefBase
    , public IFileObserver
    , public IWeakReferenceSource
{
private:
    class ObserverThread
        : public ThreadBase
    {
        friend class FileObserver;
    public:
        ObserverThread();

        CARAPI Run();

        CARAPI_(Int32) StartWatching(
            /* [in] */ const String& path,
            /* [in] */ Int32 mask,
            /* [in] */ FileObserver* observer);

        CARAPI_(void) StopWatching(
            /* [in] */ Int32 descriptor);

        CARAPI OnEvent(
            /* [in] */ Int32 wfd,
            /* [in] */ Int32 mask,
            /* [in] */ const String& path);

    private:
        CARAPI_(Int32) NativeInit();

        CARAPI_(void) NativeObserve(
            /* [in] */ Int32 fd);

        CARAPI_(Int32) NativeStartWatching(
            /* [in] */ Int32 fd,
            /* [in] */ const String& pathString,
            /* [in] */ Int32 mask);

        CARAPI_(void) NativeStopWatching(
            /* [in] */ Int32 fd,
            /* [in] */ Int32 wfd);

    private:
        HashMap<Int32, AutoPtr<IWeakReference> > mObservers;
        Object mObserverslock;
        Int32 mFd;
    };

public:
    FileObserver()
        : mDescriptor(0)
        , mMask(0)
    {}

    /**
     * Create a new file observer for a certain file or directory.
     * Monitoring does not start on creation!  You must call
     * {@link #startWatching()} before you will receive events.
     *
     * @param path The file or directory to monitor
     * @param mask The event or events (added together) to watch for
     */
    FileObserver(
        /* [in] */ const String& path,
        /* [in] */ Int32 mask = IFileObserver::ALL_EVENTS);

    virtual ~FileObserver();

    CAR_INTERFACE_DECL()

    CARAPI_(void) Init(
        /* [in] */ const String& path,
        /* [in] */ Int32 mask = IFileObserver::ALL_EVENTS);

    CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);

    /**
     * Start watching for events.  The monitored file or directory must exist at
     * this time, or else no events will be reported (even if it appears later).
     * If monitoring is already started, this call has no effect.
     */
    CARAPI StartWatching();

    /**
     * Stop watching for events.  Some events may be in process, so events
     * may continue to be reported even after this method completes.  If
     * monitoring is already stopped, this call has no effect.
     */
    CARAPI StopWatching();

private:
    static CARAPI_(Boolean) InitObserverThread();

private:
    static AutoPtr<ObserverThread> sObserverThread;
    static const Boolean sObserverThreadInited;

    // instance
    String mPath;
    Int32 mDescriptor;
    Int32 mMask;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__FILEOBSERVER_H__
