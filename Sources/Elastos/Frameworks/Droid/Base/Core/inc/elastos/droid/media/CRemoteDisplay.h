
#ifndef __ELASTOS_DROID_MEDIA_CREMOTEDISPLAY_H__
#define __ELASTOS_DROID_MEDIA_CREMOTEDISPLAY_H__

#include "_Elastos_Droid_Media_CRemoteDisplay.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/os/Runnable.h"

using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::View::ISurface;
using Elastos::Core::ICloseGuard;

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CRemoteDisplay)
{
public:
    class NotifyDisplayConnectedRun
         : public Runnable
    {
    public:
        NotifyDisplayConnectedRun(
            /* [in] */ CRemoteDisplay* owner,
            /* [in] */ ISurface* surface,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height,
            /* [in] */ Int32 flags);

        CARAPI Run();

    private:
        CRemoteDisplay* mOwner;

        AutoPtr<ISurface> mSurface;

        Int32 mWidth, mHeight, mFlags;
    };

    class NotifyDisplayDisconnectedRun
         : public Runnable
    {
    public:
        NotifyDisplayDisconnectedRun(
            /* [in] */ CRemoteDisplay* owner);

        CARAPI Run();

    private:
        CRemoteDisplay* mOwner;
    };

    class NotifyDisplayErrorRun
         : public Runnable
    {
    public:
        NotifyDisplayErrorRun(
            /* [in] */ CRemoteDisplay* owner,
            /* [in] */ Int32 error);

        CARAPI Run();

    private:
        CRemoteDisplay* mOwner;

        Int32 mError;
    };

    CRemoteDisplay();

    ~CRemoteDisplay();

    CARAPI constructor(
        /* [in] */ IRemoteDisplayListener* listener,
        /* [in] */ IHandler* handler);

    /**
     * Disconnects the remote display and stops listening for new connections.
     */
    CARAPI Dispose();

    CARAPI Pause();

    CARAPI Resume();

    /**
     * Starts listening for displays to be connected on the specified interface.
     *
     * @param iface The interface address and port in the form "x.x.x.x:y".
     * @param listener The listener to invoke when displays are connected or disconnected.
     * @param handler The handler on which to invoke the listener.
     */
    static CARAPI Listen(
        /* [in] */ const String& iface,
        /* [in] */ IRemoteDisplayListener* listener,
        /* [in] */ IHandler* handler,
        /* [out] */ IRemoteDisplay** result);

    // Called from native.
    CARAPI_(void) NotifyDisplayConnected(
        /* [in] */ ISurface* surface,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 session);

    // Called from native.
    CARAPI_(void) NotifyDisplayDisconnected();

    // Called from native.
    CARAPI_(void) NotifyDisplayError(
        /* [in] */ Int32 error);

protected:
    //@Override
    CARAPI_(void) Finalize();

private:
    CARAPI_(Handle32) NativeListen(
        /* [in] */ const String& iface);

    CARAPI_(void) NativeDispose(
        /* [in] */ Handle32 ptr);

    CARAPI_(void) NativePause(
        /* [in] */ Handle32 ptr);

    CARAPI_(void) NativeResume(
        /* [in] */ Handle32 ptr);

    CARAPI_(void) Dispose(
        /* [in] */ Boolean finalized);

    CARAPI_(void) StartListening(
        /* [in] */ const String& iface);

private:
    AutoPtr<ICloseGuard> mGuard; // = CloseGuard.get();

    AutoPtr<IRemoteDisplayListener> mListener;

    AutoPtr<IHandler> mHandler;

    Handle32 mPtr;
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_CREMOTEDISPLAY_H__
