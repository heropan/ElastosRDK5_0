
#include "webkit/WebHistoryItemClassic.h"

using Elastos::Net::CURL;
using Elastos::Net::IURL;

namespace Elastos {
namespace Droid {
namespace Webkit {

extern "C" const InterfaceID EIID_WebHistoryItemClassic =
        { 0xafadf666, 0x03b7, 0x40b7, { 0x17, 0x43, 0x09, 0xbe, 0x49, 0xc0, 0x36, 0x87 } };

Int32 WebHistoryItemClassic::sNextId = 0;
Mutex WebHistoryItemClassic::sLock;

/**
 * Construct a new WebHistoryItem with initial flattened data.
 * @param data The pre-flattened data coming from restoreState.
 */
/*package*/
WebHistoryItemClassic::WebHistoryItemClassic(
    /* [in] */ ArrayOf<Byte>* data)
{
    mFlattenedData = data;
    {
        AutoLock lock(sLock);
        mId = sNextId++;
    }
}

/**
 * Basic constructor that assigns a unique id to the item. Called by JNI
 * only.
 */
WebHistoryItemClassic::WebHistoryItemClassic(
    /* [in] */ Int32 nativeBridge)
{
    {
        AutoLock lock(sLock);
        mId = sNextId++;
    }
    mNativeBridge = nativeBridge;
    NativeRef(mNativeBridge);
}

/**
 * Construct a clone of a WebHistoryItem from the given item.
 * @param item The history item to clone.
 */
WebHistoryItemClassic::WebHistoryItemClassic(
    /* [in] */ WebHistoryItemClassic* item)
       : mId(item->mId)
       , mNativeBridge(item->mNativeBridge)
       , mFavicon(item->mFavicon)
       , mFlattenedData(item->mFlattenedData)
{
    if (mNativeBridge != 0) {
        NativeRef(mNativeBridge);
    }
}

PInterface WebHistoryItemClassic::Probe(
    /* [in] */ REIID riid)
{
    if ( riid == EIID_IInterface) {
        return (IInterface*)(IWebHistoryItem*)this;
    }
    else if ( riid == EIID_IWebHistoryItem) {
        return (IWebHistoryItem*)this;
    }
    else if (riid == EIID_WebHistoryItemClassic) {
        return this;
    }

    return NULL;
}

UInt32 WebHistoryItemClassic::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 WebHistoryItemClassic::Release()
{
    return ElRefBase::Release();
}

ECode WebHistoryItemClassic::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (object == (IInterface*)(IWebHistoryItem*)this) {
        *iid = EIID_IWebHistoryItem ;
    }
    else if (object == (IInterface*)(WebHistoryItemClassic*)this) {
        *iid = EIID_WebHistoryItemClassic;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

//@Deprecated
ECode WebHistoryItemClassic::GetId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mId;
    return NOERROR;
}

ECode WebHistoryItemClassic::GetUrl(
    /* [out] */ String* url)
{
    VALIDATE_NOT_NULL(url);
    if (mNativeBridge == 0) {
        *url = NULL;
        return NOERROR;
    }
    *url = NativeGetUrl(mNativeBridge);
    return NOERROR;
}

ECode WebHistoryItemClassic::GetOriginalUrl(
    /* [out] */ String* url)
{
    VALIDATE_NOT_NULL(url);
    if (mNativeBridge == 0) {
        *url = NULL;
        return NOERROR;
    }
    *url = NativeGetOriginalUrl(mNativeBridge);
    return NOERROR;
}

ECode WebHistoryItemClassic::GetTitle(
    /* [out] */ String* title)
{
    VALIDATE_NOT_NULL(title);
    if (mNativeBridge == 0) {
        *title = NULL;
        return NOERROR;
    }
    *title = NativeGetTitle(mNativeBridge);
    return NOERROR;
}

ECode WebHistoryItemClassic::GetFavicon(
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    if (mFavicon == NULL && mNativeBridge != 0) {
        mFavicon = NativeGetFavicon(mNativeBridge);
    }
    *bitmap = mFavicon;
    REFCOUNT_ADD(*bitmap);
    return NOERROR;
}

/**
 * Return the touch icon url.
 * If no touch icon <link> tag was specified, returns
 * <host>/apple-touch-icon.png. The DownloadTouchIcon class that
 * attempts to retrieve the touch icon will handle the case where
 * that file does not exist. An icon set by a <link> tag is always
 * used in preference to an icon saved on the server.
 * @hide
 */
String WebHistoryItemClassic::GetTouchIconUrl()
{
    if (mTouchIconUrlFromLink != NULL) {
        return mTouchIconUrlFromLink;
    } else if (mTouchIconUrlServerDefault != NULL) {
        return mTouchIconUrlServerDefault;
    }

    //try {
        AutoPtr<IURL> url;
        String urlStr;
        GetOriginalUrl(&urlStr);
        CURL::New(urlStr, (IURL**)&url);

        String protocol;
        String hostName;
        Int32 port;
        url->GetProtocol(&protocol);
        url->GetHost(&hostName);
        url->GetPort(&port);
        AutoPtr<IURL> urlTemp;
        CURL::New(protocol, hostName, port, String("/apple-touch-icon.png"), (IURL**)&urlTemp);
        assert(0);
//        urlTemp->ToString(&mTouchIconUrlServerDefault);
    //} catch (MalformedURLException e) {
    //    return null;
    //}
    return mTouchIconUrlServerDefault;
}

/**
 * Return the custom data provided by the client.
 * @hide
 */
AutoPtr<IInterface> WebHistoryItemClassic::GetCustomData()
{
    return mCustomData;
}

/**
 * Set the custom data field.
 * @param data An Object containing any data the client wishes to associate
 *             with the item.
 * @hide
 */
void WebHistoryItemClassic::SetCustomData(
    /* [in] */ IInterface* data)
{
    // NOTE: WebHistoryItems are used in multiple threads. However, the
    // public facing apis are all getters with the exception of this one
    // api. Since this api is exclusive to clients, we don't make any
    // promises about thread safety.
    mCustomData = data;
}

/**
 * Set the favicon.
 * @param icon A Bitmap containing the favicon for this history item.
 * Note: The VM ensures 32-bit atomic read/write operations so we don't have
 * to synchronize this method.
 */
/*package*/
void WebHistoryItemClassic::SetFavicon(
    /* [in] */ IBitmap* icon)
{
    mFavicon = icon;
}

/**
 * Set the touch icon url. Will not overwrite an icon that has been
 * set already from a <link> tag, unless the new icon is precomposed.
 * @hide
 */
/*package*/
void WebHistoryItemClassic::SetTouchIconUrl(
    /* [in] */ const String& url,
    /* [in] */ Boolean precomposed)
{
    if (precomposed || mTouchIconUrlFromLink == NULL) {
        mTouchIconUrlFromLink = url;
    }
}

/**
 * Get the pre-flattened data.
 * Note: The VM ensures 32-bit atomic read/write operations so we don't have
 * to synchronize this method.
 */
/*package*/
AutoPtr< ArrayOf<Byte> > WebHistoryItemClassic::GetFlattenedData()
{
    if (mNativeBridge != 0) {
        return NativeGetFlattenedData(mNativeBridge);
    }
    return mFlattenedData;
}

/**
 * Inflate this item.
 * Note: The VM ensures 32-bit atomic read/write operations so we don't have
 * to synchronize this method.
 */
/*package*/
void WebHistoryItemClassic::Inflate(
    /* [in] */ Int32 nativeFrame)
{
    mNativeBridge = Inflate(nativeFrame, mFlattenedData);
    mFlattenedData = NULL;
}

//synchronized
AutoPtr<WebHistoryItemClassic> WebHistoryItemClassic::Clone()
{
    Elastos::Core::AutoLock lock(mLock);
    return new WebHistoryItemClassic(this);
}

/* Natively inflate this item, this method is called in the WebCore thread.
 */
Int32 WebHistoryItemClassic::Inflate(
    /* [in] */ Int32 frame,
    /* [in] */ ArrayOf<Byte>* data)
{
    return Elastos_WebHistory_inflate(frame, data);
}

void WebHistoryItemClassic::NativeRef(
    /* [in] */ Int32 ptr)
{
    Elastos_WebHistory_nativeRef(ptr);
}

void WebHistoryItemClassic::NativeUnref(
    /* [in] */ Int32 ptr)
{
    Elastos_WebHistory_nativeUnref(ptr);
}

String WebHistoryItemClassic::NativeGetTitle(
    /* [in] */ Int32 ptr)
{
    return Elastos_WebHistory_nativeGetTitle(ptr);
}

String WebHistoryItemClassic::NativeGetUrl(
    /* [in] */ Int32 ptr)
{
    return Elastos_WebHistory_nativeGetUrl(ptr);
}

String WebHistoryItemClassic::NativeGetOriginalUrl(
    /* [in] */ Int32 ptr)
{
    return Elastos_WebHistory_nativeGetOriginalUrl(ptr);
}

AutoPtr< ArrayOf<Byte> > WebHistoryItemClassic::NativeGetFlattenedData(
    /* [in] */ Int32 ptr)
{
    return Elastos_WebHistory_nativeGetFlattenedData(ptr);
}

AutoPtr<IBitmap> WebHistoryItemClassic::NativeGetFavicon(
    /* [in] */ Int32 ptr)
{
    return IBitmap::Probe(Elastos_WebHistory_nativeGetFavicon(ptr));
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
