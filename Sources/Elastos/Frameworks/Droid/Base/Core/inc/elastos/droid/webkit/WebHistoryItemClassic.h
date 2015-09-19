
#ifndef __ELASTOS_DROID_WEBKIT_WEBHISTORYITEMCLASSIC_H__
#define __ELASTOS_DROID_WEBKIT_WEBHISTORYITEMCLASSIC_H__

#include "ext/frameworkext.h"

using Elastos::Droid::Graphics::IBitmap;

#ifdef __cplusplus
extern "C"
{
#endif
    extern void Elastos_WebHistory_InitCallback(Int32 cb);
    extern void Elastos_WebHistory_nativeClose(Int32 frame);
    extern void Elastos_WebHistory_restoreIndex(Int32 frame, Int32 index);
    extern Int32 Elastos_WebHistory_inflate(Int32 frame, ArrayOf<Byte>* data);
    extern void Elastos_WebHistory_nativeRef(Int32 ptr);
    extern void Elastos_WebHistory_nativeUnref(Int32 ptr);
    extern Elastos::String Elastos_WebHistory_nativeGetTitle(Int32 ptr);
    extern Elastos::String Elastos_WebHistory_nativeGetUrl(Int32 ptr);
    extern Elastos::String Elastos_WebHistory_nativeGetOriginalUrl(Int32 ptr);
    extern AutoPtr< ArrayOf<Byte> > Elastos_WebHistory_nativeGetFlattenedData(Int32 ptr);
    extern AutoPtr<IInterface> Elastos_WebHistory_nativeGetFavicon(Int32 ptr);
#ifdef __cplusplus
}
#endif


namespace Elastos {
namespace Droid {
namespace Webkit {

extern "C" const InterfaceID EIID_WebHistoryItemClassic;

/* package */
class WebHistoryItemClassic
    : public ElRefBase
    , public IWebHistoryItem //implements Cloneable
{
public:
    /**
     * Construct a new WebHistoryItem with initial flattened data.
     * @param data The pre-flattened data coming from restoreState.
     */
    /*package*/
    WebHistoryItemClassic(
        /* [in] */ ArrayOf<Byte>* data);

    CAR_INTERFACE_DECL();

    //@Deprecated
    CARAPI GetId(
        /* [out] */ Int32* id);

    CARAPI GetUrl(
        /* [out] */ String* url);

    CARAPI GetOriginalUrl(
        /* [out] */ String* url);

    CARAPI GetTitle(
        /* [out] */ String* title);

    CARAPI GetFavicon(
        /* [out] */ IBitmap** bitmap);

    /**
     * Return the touch icon url.
     * If no touch icon <link> tag was specified, returns
     * <host>/apple-touch-icon.png. The DownloadTouchIcon class that
     * attempts to retrieve the touch icon will handle the case where
     * that file does not exist. An icon set by a <link> tag is always
     * used in preference to an icon saved on the server.
     * @hide
     */
    virtual CARAPI_(String) GetTouchIconUrl();

    /**
     * Return the custom data provided by the client.
     * @hide
     */
    virtual CARAPI_(AutoPtr<IInterface>) GetCustomData();

    /**
     * Set the custom data field.
     * @param data An Object containing any data the client wishes to associate
     *             with the item.
     * @hide
     */
    virtual CARAPI_(void) SetCustomData(
        /* [in] */ IInterface* data);
    /**
     * Set the favicon.
     * @param icon A Bitmap containing the favicon for this history item.
     * Note: The VM ensures 32-bit atomic read/write operations so we don't have
     * to synchronize this method.
     */
    /*package*/
    virtual CARAPI_(void) SetFavicon(
        /* [in] */ IBitmap* icon);

    /**
     * Set the touch icon url. Will not overwrite an icon that has been
     * set already from a <link> tag, unless the new icon is precomposed.
     * @hide
     */
    /*package*/
    virtual CARAPI_(void) SetTouchIconUrl(
        /* [in] */ const String& url,
        /* [in] */ Boolean precomposed);

    /**
     * Get the pre-flattened data.
     * Note: The VM ensures 32-bit atomic read/write operations so we don't have
     * to synchronize this method.
     */
    /*package*/
    virtual CARAPI_(AutoPtr< ArrayOf<Byte> >) GetFlattenedData();

    /**
     * Inflate this item.
     * Note: The VM ensures 32-bit atomic read/write operations so we don't have
     * to synchronize this method.
     */
    /*package*/
    virtual CARAPI_(void) Inflate(
        /* [in] */ Int32 nativeFrame);

    //synchronized
    virtual CARAPI_(AutoPtr<WebHistoryItemClassic>) Clone();

    /**
     * Basic constructor that assigns a unique id to the item. Called by JNI
     * only.
     */
    WebHistoryItemClassic(
        /* [in] */ Int32 nativeBridge);

protected:
//    protected virtual CARAPI_(void) finalize();

private:
    /**
     * Construct a clone of a WebHistoryItem from the given item.
     * @param item The history item to clone.
     */
    WebHistoryItemClassic(
        /* [in] */ WebHistoryItemClassic* item);

    /* Natively inflate this item, this method is called in the WebCore thread.
     */
    CARAPI_(Int32) Inflate(
        /* [in] */ Int32 nativeFrame,
        /* [in] */ ArrayOf<Byte>* data);

    CARAPI_(void) NativeRef(
        /* [in] */ Int32 nptr);

    CARAPI_(void) NativeUnref(
        /* [in] */ Int32 nptr);

    CARAPI_(String) NativeGetTitle(
        /* [in] */ Int32 nptr);

    CARAPI_(String) NativeGetUrl(
        /* [in] */ Int32 nptr);

    CARAPI_(String) NativeGetOriginalUrl(
        /* [in] */ Int32 nptr);

    CARAPI_(AutoPtr< ArrayOf<Byte> >) NativeGetFlattenedData(
        /* [in] */ Int32 nptr);

    CARAPI_(AutoPtr<IBitmap>) NativeGetFavicon(
        /* [in] */ Int32 nptr);

private:// Global identifier count.
    static Int32 sNextId;
    static Object sLock;
    // Unique identifier.
    Int32 mId;
    // A point to a native WebHistoryItem instance which contains the actual data
    Int32 mNativeBridge;
    // The favicon for this item.
    AutoPtr<IBitmap> mFavicon;
    // The pre-flattened data used for saving the state.
    AutoPtr< ArrayOf<Byte> > mFlattenedData;
    // The apple-touch-icon url for use when adding the site to the home screen,
    // as obtained from a <link> element in the page.
    String mTouchIconUrlFromLink;
    // If no <link> is specified, this holds the default location of the
    // apple-touch-icon.
    String mTouchIconUrlServerDefault;
    // Custom client data that is not flattened or read by native code.
    AutoPtr<IInterface> mCustomData;

    Object mLock;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_WEBHISTORYITEMCLASSIC_H__
