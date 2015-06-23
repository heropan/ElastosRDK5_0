
#ifndef __WEBICONDATABASECLASSIC_H__
#define __WEBICONDATABASECLASSIC_H__

#include "ext/frameworkext.h"
#include "os/HandlerBase.h"
#include "webkit/WebIconDatabase.h"
#include <elastos/Vector.h>

using Elastos::Core::Mutex;
using Elastos::Utility::Vector;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Net::Http::IEventHandler;

#ifdef __cplusplus
extern "C"
{
#endif
    extern void Elastos_WebIconDatabase_nativeOpen(const Elastos::String& path);
    extern void Elastos_WebIconDatabase_nativeClose();
    extern void Elastos_WebIconDatabase_nativeRemoveAllIcons();
    extern AutoPtr<IInterface> Elastos_WebIconDatabase_nativeIconForPageUrl(const Elastos::String& url);
    extern void Elastos_WebIconDatabase_nativeRetainIconForPageUrl(const Elastos::String& url);
    extern void Elastos_WebIconDatabase_nativeReleaseIconForPageUrl(const Elastos::String& url);
#ifdef __cplusplus
}
#endif

namespace Elastos {
namespace Droid {
namespace Webkit {

class WebIconDatabaseClassic
    : public ElRefBase
    , public WebIconDatabase
    , public IWebIconDatabase
{
private:
    // Class to handle messages before WebCore is ready
    class EventHandler : public HandlerBase
    {
        friend class WebIconDatabaseClassic;
    private:
        // Class to handle a result dispatch
        class IconResult
            : public ElLightRefBase
            , public IInterface
        {
        public:
            IconResult(
                /* [in] */ const String& url,
                /* [in] */ IBitmap* icon,
                /* [in] */ IIconListener* l);

            CAR_INTERFACE_DECL();

            CARAPI_(void) Dispatch();

        private:
            const String mUrl;
            const AutoPtr<IBitmap> mIcon;
            const AutoPtr<IIconListener> mListener;
        };

        class InnerHandler : public HandlerBase
        {
        public:
            InnerHandler(
                /* [in] */ EventHandler* owner);

            CARAPI HandleMessage(
                /* [in] */ IMessage* msg);

        private:
            EventHandler* mOwner;
        };

    public:
        EventHandler();

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        // Called by WebCore thread to create the actual handler
        CARAPI_(void) CreateHandler();

        CARAPI_(Boolean) HasHandler();

        CARAPI_(void) PostMessage(
            /* [in] */ IMessage* msg);

        CARAPI_(void) BulkRequestIcons(
            /* [in] */ IMessage* msg);

        CARAPI_(void) RequestIconAndSendResult(
            /* [in] */ const String& url,
            /* [in] */ IIconListener* listener);

    public:
        // Message ids
        static const Int32 OPEN              = 0;
        static const Int32 CLOSE             = 1;
        static const Int32 REMOVE_ALL        = 2;
        static const Int32 REQUEST_ICON      = 3;
        static const Int32 RETAIN_ICON       = 4;
        static const Int32 RELEASE_ICON      = 5;
        static const Int32 BULK_REQUEST_ICON = 6;

    private:
        // Message for dispatching icon request results
        static const Int32 ICON_RESULT = 10;
        // Actual handler that runs in WebCore thread
        AutoPtr<IHandler> mHandler;
        // Vector of messages before the WebCore thread is ready
        AutoPtr< Vector< AutoPtr<IMessage> > > mMessages;
        Mutex mLock;
    };

public:
    CAR_INTERFACE_DECL();

    CARAPI Open(
        /* [in] */ const String& path);

    CARAPI Close();

    CARAPI RemoveAllIcons();

    /**
     * Request the Bitmap representing the icon for the given page
     * url. If the icon exists, the listener will be called with the result.
     * @param url The page's url.
     * @param listener An implementation on IconListener to receive the result.
     */
    CARAPI RequestIconForPageUrl(
        /* [in] */ const String& url,
        /* [in] */ IIconListener* listener);

    /** {@hide}
     */
    CARAPI BulkRequestIconForPageUrl(
        /* [in] */ IContentResolver* cr,
        /* [in] */ const String& where,
        /* [in] */ IIconListener* listener);

    CARAPI RetainIconForPageUrl(
        /* [in] */ const String& url);

    CARAPI ReleaseIconForPageUrl(
        /* [in] */ const String& url);

    /**
     * Get the global instance of WebIconDatabase.
     * @return A single instance of WebIconDatabase. It will be the same
     *         instance for the current process each time this method is
     *         called.
     */
    static CARAPI_(AutoPtr<WebIconDatabaseClassic>) GetInstance();

    /**
     * Create the internal handler and transfer all pending messages.
     * XXX: Called by WebCore thread only!
     */
    /*package*/
    CARAPI_(void) CreateHandler();

protected:
    /**
     * Private constructor to avirtual CARAPI_(void) anyone else creating an instance.
     */
    WebIconDatabaseClassic();

private:
    // Native functions
    static CARAPI_(void) NativeOpen(
        /* [in] */ const String& path);

    static CARAPI_(void) NativeClose();

    static CARAPI_(void) NativeRemoveAllIcons();

    static CARAPI_(AutoPtr<IBitmap>) NativeIconForPageUrl(
        /* [in] */ const String& url);

    static CARAPI_(void) NativeRetainIconForPageUrl(
        /* [in] */ const String& url);

    static CARAPI_(void) NativeReleaseIconForPageUrl(
        /* [in] */ const String& url);

private:
    static const String LOGTAG;

    // Global instance of a WebIconDatabase
    static AutoPtr<WebIconDatabaseClassic> sIconDatabase;

    // EventHandler for handling messages before and after the WebCore thread is
    // ready.
    AutoPtr<EventHandler> mEventHandler;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__WEBICONDATABASECLASSIC_H__
