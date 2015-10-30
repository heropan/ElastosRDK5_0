
#ifndef __ELASTOS_DROID_WEBKIT_GEOLOCATIONPERMISSIONSCLASSIC_H__
#define __ELASTOS_DROID_WEBKIT_GEOLOCATIONPERMISSIONSCLASSIC_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/os/HandlerBase.h"
#include "elastos/droid/webkit/GeolocationPermissions.h"
#include <elastos/utility/etl/HashSet.h>
#include <elastos/utility/etl/Vector.h>

using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::View::IActionMode;
using Elastos::Droid::View::IMenu;
using Elastos::Utility::ISet;
using Elastos::Utility::Etl::Vector;

namespace Elastos {
namespace Droid {
namespace Webkit {

// This class is the Java counterpart of the WebKit C++ GeolocationPermissions
// class. It simply marshals calls from the UI thread to the WebKit thread.
class GeolocationPermissionsClassic
    : public Object
    , public GeolocationPermissions
    , public IGeolocationPermissions
{
private:
    class InnerUIHandler : public HandlerBase
    {
    public:
        InnerUIHandler(
            /* [in] */ GeolocationPermissionsClassic* owner);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

        CARAPI ToString(
            /* [out] */ String* info);

    private:
        GeolocationPermissionsClassic* mOwner;
    };

    class InnerHandler : public HandlerBase
    {
    public:
        InnerHandler(
            /* [in] */ GeolocationPermissionsClassic* owner);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

        CARAPI ToString(
            /* [out] */ String* info);

    private:
        GeolocationPermissionsClassic* mOwner;
    };

public:
    // Message ids
    static const Int32 GET_ORIGINS      = 0;
    static const Int32 GET_ALLOWED      = 1;
    static const Int32 CLEAR            = 2;
    static const Int32 ALLOW            = 3;
    static const Int32 CLEAR_ALL        = 4;

    // Message ids on the UI thread
    static const Int32 RETURN_ORIGINS   = 0;
    static const Int32 RETURN_ALLOWED   = 1;

public:
    GeolocationPermissionsClassic();

    CAR_INTERFACE_DECL();

    static CARAPI_(AutoPtr<GeolocationPermissionsClassic>) GetInstance();

    /**
     * Creates the UI message handler. Must be called on the UI thread.
     * @hide
     */
    virtual CARAPI_(void) CreateUIHandler();

    /**
     * Creates the message handler. Must be called on the WebKit thread.
     * @hide
     */
    //synchronized
    virtual CARAPI_(void) CreateHandler();


    // Note that we represent the origins as strings. These are created using
    // WebCore::SecurityOrigin::toString(). As long as all 'HTML 5 modules'
    // (Database, Geolocation etc) do so, it's safe to match up origins based
    // on this string.
    CARAPI GetOrigins(
        /* [in] */ IValueCallback* callback);

    CARAPI GetAllowed(
        /* [in] */ const String& origin,
        /* [in] */ IValueCallback* callback);

    // This method may be called before the WebKit
    // thread has intialized the message handler. Messages will be queued until
    // this time.
    CARAPI Clear(
        /* [in] */ const String& origin);

    // This method may be called before the WebKit
    // thread has intialized the message handler. Messages will be queued until
    // this time.
    CARAPI Allow(
        /* [in] */ const String& origin);

    CARAPI ClearAll();

    CARAPI ToString(
        /* [out] */ String* info);

private:
    /**
     * Utility function to send a message to our handler.
     */
    //synchronized
    CARAPI_(void) PostMessage(
        /* [in] */ IMessage* msg);

    /**
     * Utility function to send a message to the handler on the UI thread
     */
    CARAPI_(void) PostUIMessage(
        /* [in] */ IMessage* msg);

    // Native functions, run on the WebKit thread.
    static CARAPI_(AutoPtr<ISet>) NativeGetOrigins();

    static CARAPI_(Boolean) NativeGetAllowed(
        /* [in] */ const String& origin);

    static CARAPI_(void) NativeClear(
        /* [in] */ const String& origin);

    static CARAPI_(void) NativeAllow(
        /* [in] */ const String& origin);

    static CARAPI_(void) NativeClearAll();

private:
    Object mLock;
    AutoPtr<IHandler> mHandler;
    AutoPtr<IHandler> mUIHandler;

    // A queue to store messages until the handler is ready.
    Vector< AutoPtr<IMessage> > mQueuedMessages;

    static const String ORIGINS;
    static const String ORIGIN;
    static const String CALLBACK;
    static const String ALLOWED;

    // Global instance
    static AutoPtr<GeolocationPermissionsClassic> sInstance;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_GEOLOCATIONPERMISSIONSCLASSIC_H__
