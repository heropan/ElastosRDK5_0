
#ifndef __ELASTOS_DROID_WEBKIT_WEBSTORAGECLASSIC_H__
#define __ELASTOS_DROID_WEBKIT_WEBSTORAGECLASSIC_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/os/HandlerBase.h"
#include "elastos/droid/webkit/WebStorage.h"
#include <elastos/utility/etl/Set.h>
#include <elastos/utility/etl/HashSet.h>

using Elastos::Droid::Os::HandlerBase;
using Elastos::Utility::ICollection;
using Elastos::Utility::IObjectStringMap;
using Elastos::Utility::IMap;

namespace Elastos {
namespace Droid {
namespace Webkit {

extern "C" const InterfaceID EIID_WebStorageClassic;

/** @hide */
class WebStorageClassic
    : public ElRefBase
    , public WebStorage
    , public IWebStorage
{
private:
    class InnerUIHandler : public HandlerBase
    {
    public:
        InnerUIHandler(
            /* [in] */ WebStorageClassic* owner);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        WebStorageClassic* mOwner;
    };

    class InnerHandler : public HandlerBase
    {
    public:
        InnerHandler(
            /* [in] */ WebStorageClassic* owner);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        WebStorageClassic* mOwner;
    };

public:
    WebStorageClassic();

    CAR_INTERFACE_DECL();

    /**
     * @hide
     * Message handler, UI side
     * @hide
     */
    virtual CARAPI_(void) CreateUIHandler();

    /**
     * Message handler, WebCore side
     * @hide
     */
    //synchronized
    virtual CARAPI_(void) CreateHandler();

    /*
     * When calling getOrigins(), getUsageForOrigin() and getQuotaForOrigin(),
     * we need to get the values from WebCore, but we cannot block while doing so
     * as we used to do, as this could result in a full deadlock (other WebCore
     * messages received while we are still blocked here, see http://b/2127737).
     *
     * We have to do everything asynchronously, by providing a callback function.
     * We post a message on the WebCore thread (mHandler) that will get the result
     * from WebCore, and we post it back on the UI thread (using mUIHandler).
     * We can then use the callback function to return the value.
     */
    CARAPI GetOrigins(
        /* [in] */ IValueCallback* callback);

    /**
     * Returns a list of origins having a database
     * should only be called from WebViewCore.
     */
    virtual CARAPI_(AutoPtr<ICollection>) GetOriginsSync();

    CARAPI GetUsageForOrigin(
        /* [in] */ const String& origin,
        /* [in] */ IValueCallback* callback);

    CARAPI GetQuotaForOrigin(
        /* [in] */ const String& origin,
        /* [in] */ IValueCallback* callback);

    CARAPI SetQuotaForOrigin(
        /* [in] */ const String& origin,
        /* [in] */ Int64 quota);

    CARAPI DeleteOrigin(
        /* [in] */ const String& origin);

    CARAPI DeleteAllData();

    /**
     * Sets the maximum size of the ApplicationCache.
     * This should only ever be called on the WebKit thread.
     * Not part of the base-class API: this is only used by dump render tree.
     */
    virtual CARAPI_(void) SetAppCacheMaximumSize(
        /* [in] */ Int64 size);

    /**
     * Get the singleton instance of this class.
     * @return The singleton {@link WebStorage} instance.
     */
    static CARAPI_(AutoPtr<WebStorageClassic>) GetInstance();

    /**
     * @hide
     * Post a Sync request
     */
    virtual CARAPI_(void) Update();

public:
    // Message ids
    static const Int32 UPDATE = 0;
    static const Int32 SET_QUOTA_ORIGIN = 1;
    static const Int32 DELETE_ORIGIN = 2;
    static const Int32 DELETE_ALL = 3;
    static const Int32 GET_ORIGINS = 4;
    static const Int32 GET_USAGE_ORIGIN = 5;
    static const Int32 GET_QUOTA_ORIGIN = 6;

    // Message ids on the UI thread
    static const Int32 RETURN_ORIGINS = 0;
    static const Int32 RETURN_USAGE_ORIGIN = 1;
    static const Int32 RETURN_QUOTA_ORIGIN = 2;

private:
    /**
     * Utility function to send a message to our handler
     */
    //synchronized
    CARAPI_(void) PostMessage(
        /* [in] */ IMessage* msg);

    /**
     * Utility function to send a message to the handler on the UI thread
     */
    CARAPI_(void) PostUIMessage(
        /* [in] */ IMessage* msg);

    /**
     * Run on the WebCore thread
     * set the local values with the current ones
     */
    CARAPI_(void) SyncValues();

    // Native functions
    static CARAPI_(HashSet<String>*) NativeGetOrigins();

    static CARAPI_(Int64) NativeGetUsageForOrigin(
        /* [in] */ const String& origin);

    static CARAPI_(Int64) NativeGetQuotaForOrigin(
        /* [in] */ const String& origin);

    static CARAPI_(void) NativeSetQuotaForOrigin(
        /* [in] */ const String& origin,
        /* [in] */ Int64 quota);

    static CARAPI_(void) NativeDeleteOrigin(
        /* [in] */ const String& origin);

    static CARAPI_(void) NativeDeleteAllData();

    static CARAPI_(void) NativeSetAppCacheMaximumSize(
        /* [in] */ Int64 size);

private:
    // Global instance of a WebStorage
    static AutoPtr<WebStorageClassic> sWebStorage;

    static const String ORIGINS;
    static const String ORIGIN;
    static const String CALLBACK;
    static const String USAGE;
    static const String QUOTA;

    AutoPtr<IMap> mOrigins;

    AutoPtr<IHandler> mHandler;
    AutoPtr<IHandler> mUIHandler;

    Object mLock;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_WEBSTORAGECLASSIC_H__
