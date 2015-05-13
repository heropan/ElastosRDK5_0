#ifndef __VIEWSERVER_H__
#define __VIEWSERVER_H__

#include "wm/CWindowManagerService.h"

using Elastos::Core::IRunnable;
using Elastos::Net::IServerSocket;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

/**
 * The ViewServer is local socket server that can be used to communicate with the
 * views of the opened windows. Communication with the views is ensured by the
 * {@link com.android.server.wm.WindowManagerService} and is a cross-process operation.
 *
 * {@hide}
 */
class ViewServer
    : public ElRefBase
    , public IRunnable
{
public:
    class ViewServerWorker
        : public ElRefBase
        , public IRunnable
        , public CWindowManagerService::IWindowChangeListener
    {
    public:
        ViewServerWorker(
            /* [in] */ ISocket* client,
            /* [in] */ ViewServer* host);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [in] */ InterfaceID* pIID);

        CARAPI Run();

        CARAPI_(void) WindowsChanged();

        CARAPI_(void) FocusChanged();

    private:
        CARAPI_(Boolean) WindowManagerAutolistLoop();

    private:
        AutoPtr<ISocket> mClient;
        Boolean mNeedWindowListUpdate;
        Boolean mNeedFocusedWindowUpdate;
        AutoPtr<ViewServer> mHost;
        Object mLock;
    };
public:
    /**
     * Creates a new ViewServer associated with the specified window manager on the
     * specified local port. The server is not started by default.
     *
     * @param windowManager The window manager used to communicate with the views.
     * @param port The port for the server to listen to.
     *
     * @see #start()
     */
    ViewServer(
        /* [in] */ CWindowManagerService* service,
        /* [in] */ Int32 port);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface* pObject,
        /* [in] */ InterfaceID* pIID);

    /**
     * Starts the server.
     *
     * @return True if the server was successfully created, or false if it already exists.
     * @throws IOException If the server cannot be created.
     *
     * @see #stop()
     * @see #isRunning()
     * @see WindowManagerService#startViewServer(int)
     */
    CARAPI Start(
        /* [out] */ Boolean* results);

    /**
     * Stops the server.
     *
     * @return True if the server was stopped, false if an error occured or if the
     *         server wasn't started.
     *
     * @see #start()
     * @see #isRunning()
     * @see WindowManagerService#stopViewServer()
     */
    CARAPI_(Boolean) Stop();

    /**
     * Indicates whether the server is currently running.
     *
     * @return True if the server is running, false otherwise.
     *
     * @see #start()
     * @see #stop()
     * @see WindowManagerService#isViewServerRunning()
     */
    CARAPI_(Boolean) IsRunning();

    /**
     * Main server loop.
     */
    CARAPI Run();

private:
    static CARAPI_(Boolean) WriteValue(
        /* [in] */ ISocket* client,
        /* [in] */ const String& value);

public:
    /**
     * The default port used to start view servers.
     */
    static const Int32 VIEW_SERVER_DEFAULT_PORT;

private:
    static const Int32 VIEW_SERVER_MAX_CONNECTIONS;

    // Debug facility
    static const String LocalLOG_TAG;

    static const String VALUE_PROTOCOL_VERSION;
    static const String VALUE_SERVER_VERSION;

    // Protocol commands
    // Returns the protocol version
    static const String COMMAND_PROTOCOL_VERSION;
    // Returns the server version
    static const String COMMAND_SERVER_VERSION;
    // Lists all of the available windows in the system
    static const String COMMAND_WINDOW_MANAGER_LIST;
    // Keeps a connection open and notifies when the list of windows changes
    static const String COMMAND_WINDOW_MANAGER_AUTOLIST;
    // Returns the focused window
    static const String COMMAND_WINDOW_MANAGER_GET_FOCUS;

    AutoPtr<IServerSocket> mServer;
    AutoPtr<IThread> mThread;

    AutoPtr<CWindowManagerService> mWindowManager;
    Int32 mPort;

    // AutoPtr<IExecutorService> mThreadPool;
};

} // Wm
} // Server
} // Droid
} // Elastos

#endif //__VIEWSERVER_H__
