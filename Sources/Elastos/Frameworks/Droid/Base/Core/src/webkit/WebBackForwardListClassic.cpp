
#include "webkit/WebBackForwardListClassic.h"
#include "webkit/CallbackProxy.h"
#include <elastos/Logger.h>
#include "webkit/DebugFlags.h"

using Elastos::Utility::Logging::Logger;
namespace Elastos {
namespace Droid {
namespace Webkit {

const String WebBackForwardListClassic::LOGTAG("WebBackForwardListClassic");

extern "C" const InterfaceID EIID_WebBackForwardListClassic =
        { 0xafadf655, 0xd3b7, 0x40b1, { 0xa7, 0xc3, 0x75, 0xbe, 0x49, 0xc8, 0x96, 0xfe } };

Mutex WebBackForwardListClassic::sLock;
ElaWebHistoryCallback* WebBackForwardListClassic::Init_WebHistory_Callback()
{
    static ElaWebHistoryCallback t = {
        &WebBackForwardListClassic::CreateWebHistoryItem,
        &WebBackForwardListClassic::AddHistoryItem,
        &WebBackForwardListClassic::RemoveHistoryItem,
        &WebBackForwardListClassic::SetCurrentIndex
    };
    Elastos_WebHistory_InitCallback((Int32)&t);
    return &t;
}
ElaWebHistoryCallback* WebBackForwardListClassic::sElaWebHistoryCallback = WebBackForwardListClassic::Init_WebHistory_Callback();

/*package*/
WebBackForwardListClassic::WebBackForwardListClassic(
    /* [in] */ CallbackProxy* proxy)
       : mCurrentIndex(-1)
       , mClearPending(FALSE)
       , mCallbackProxy(proxy)
{
}

PInterface WebBackForwardListClassic::Probe(
    /* [in] */ REIID riid)
{
    if ( riid == EIID_IInterface) {
        return (IInterface*)(IWebBackForwardList*)this;
    }
    else if ( riid == EIID_IWebBackForwardList) {
        return (IWebBackForwardList*)this;
    }
    else if (riid == EIID_IWeakReferenceSource) {
        return (IWeakReferenceSource *)this;
    }
    else if (riid == EIID_WebBackForwardListClassic) {
        return reinterpret_cast<PInterface>(this);
    }

    return NULL;
}

UInt32 WebBackForwardListClassic::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 WebBackForwardListClassic::Release()
{
    return ElRefBase::Release();
}

ECode WebBackForwardListClassic::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (object == (IInterface*)(IWebBackForwardList*)this) {
        *iid = EIID_IWebBackForwardList ;
    }
    else if (object == (IInterface*)(IWeakReferenceSource *)this) {
        *iid = EIID_IWeakReferenceSource;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

//synchronized
ECode WebBackForwardListClassic::GetCurrentItem(
    /* [out] */ IWebHistoryItem** item)
{
    Mutex::Autolock lock(mLock);
    return GetItemAtIndex(mCurrentIndex, item);
}

//synchronized
ECode WebBackForwardListClassic::GetCurrentIndex(
    /* [out] */ Int32* index)
{
    Mutex::Autolock lock(mLock);
    VALIDATE_NOT_NULL(index);
    *index = mCurrentIndex;
    return NOERROR;
}

//synchronized
AutoPtr<WebHistoryItemClassic> WebBackForwardListClassic::GetItemAtIndex(
    /* [in] */ Int32 index)
{
    Mutex::Autolock lock(mLock);
    Int32 size;
    if (index < 0 || index >= (GetSize(&size), size)) {
        return NULL;
    }
    return mArray[index];
}

ECode WebBackForwardListClassic::GetItemAtIndex(
    /* [in] */ Int32 index,
    /* [out] */ IWebHistoryItem** item)
{
    VALIDATE_NOT_NULL(item);
    AutoPtr<WebHistoryItemClassic> h = mArray[index];
    *item = h.Get();
    INTERFACE_ADDREF(*item);
    return NOERROR;
}

//synchronized
ECode WebBackForwardListClassic::GetSize(
    /* [out] */ Int32* size)
{
    Mutex::Autolock lock(mLock);
    VALIDATE_NOT_NULL(size);
    *size = mArray.GetSize();
    return NOERROR;
}

/**
 * Mark the back/forward list as having a pending clear. This is used on the
 * UI side to mark the list as being invalid during the clearHistory method.
 */
/*package*/
//synchronized
void WebBackForwardListClassic::SetClearPending()
{
    Mutex::Autolock lock(mLock);
    mClearPending = TRUE;
}

/**
 * Return the status of the clear flag. This is used on the UI side to
 * determine if the list is valid for checking things like canGoBack.
 */
/*package*/
//synchronized
Boolean WebBackForwardListClassic::GetClearPending()
{
    Mutex::Autolock lock(mLock);
    return mClearPending;
}

/**
 * Add a new history item to the list. This will remove all items after the
 * current item and append the new item to the end of the list. Called from
 * the WebCore thread only. Synchronized because the UI thread may be
 * reading the array or the current index.
 * @param item A new history item.
 */
/*package*/
//synchronized
void WebBackForwardListClassic::AddHistoryItem(
    /* [in] */ IWebHistoryItem* item)
{
    Mutex::Autolock lock(mLock);
    // Update the current position because we are going to add the new item
    // in that slot.
    ++mCurrentIndex;
    // If the current position is not at the end, remove all history items
    // after the current item.
    Int32 size = mArray.GetSize();
    Int32 newPos = mCurrentIndex;
    if (newPos != size) {
        for (Int32 i = size - 1; i >= newPos; i--) {
            AutoPtr<WebHistoryItemClassic> h = mArray[i];
            mArray.Remove(h);
        }
    }
    // Add the item to the list.
    mArray.PushBack((WebHistoryItemClassic*) item->Probe(EIID_WebHistoryItemClassic));
    if (mCallbackProxy != NULL) {
        mCallbackProxy->OnNewHistoryItem(item);
    }
}

/**
 * Clear the back/forward list. Called from the WebCore thread.
 */
/*package*/
//synchronized
void WebBackForwardListClassic::Close(
    /* [in] */ Int32 nativeFrame)
{
    Mutex::Autolock lock(mLock);
    // Clear the array first because nativeClose will call addHistoryItem
    // with the current item.
    mArray.Clear();
    mCurrentIndex = -1;
    NativeClose(nativeFrame);
    // Reset the clear flag
    mClearPending = FALSE;
}

//synchronized
AutoPtr<WebBackForwardListClassic> WebBackForwardListClassic::Clone()
{
    Mutex::Autolock lock(mLock);
    AutoPtr<WebBackForwardListClassic> l = new WebBackForwardListClassic(NULL);
    if (mClearPending) {
        // If a clear is pending, return a copy with only the current item.
        AutoPtr<IWebHistoryItem> item;
        GetCurrentItem((IWebHistoryItem**)&item);
        l->AddHistoryItem(item);
        return l;
    }
    l->mCurrentIndex = mCurrentIndex;
    Int32 size;
    GetSize(&size);
    //l.mArray = new ArrayList<WebHistoryItemClassic>(size);
    for (Int32 i = 0; i < size; i++) {
        // Add a copy of each WebHistoryItem
        l->mArray.PushBack(mArray[i]->Clone());
    }
    return l;
}

/**
 * Set the new history index.
 * @param newIndex The new history index.
 */
/*package*/
//synchronized
void WebBackForwardListClassic::SetCurrentIndex(
    /* [in] */ Int32 newIndex)
{
    Mutex::Autolock lock(mLock);
    mCurrentIndex = newIndex;
    if (mCallbackProxy != NULL) {
        AutoPtr<IWebHistoryItem> item;
        GetItemAtIndex(newIndex, (IWebHistoryItem**)&item);
        mCallbackProxy->OnIndexChanged(item, newIndex);
    }
}

/**
 * Restore the history index.
 */
/*package*/
//synchronized
void WebBackForwardListClassic::RestoreIndex(
    /* [in] */ Int32 frame,
    /* [in] */ Int32 index)
{
    Mutex::Autolock lock(sLock);
    Elastos_WebHistory_restoreIndex(frame, index);
}

//synchronized
void WebBackForwardListClassic::RemoveHistoryItem(
    /* [in] */ Int32 index)
{
    Mutex::Autolock lock(mLock);
    // XXX: This is a special case. Since the callback is only triggered
    // when removing the first item, we can assert that the index is 0.
    // This lets us change the current index without having to query the
    // native BackForwardList.
//    if (DebugFlags.WEB_BACK_FORWARD_LIST && (index != 0)) {
//        throw new AssertionError();
//    }
    AutoPtr<WebHistoryItemClassic> h = mArray[index];
    mArray.Remove(h);
    // XXX: If we ever add another callback for removing history items at
    // any index, this will no longer be valid.
    mCurrentIndex--;
}

/* Close the native list. */
void WebBackForwardListClassic::NativeClose(
    /* [in] */ Int32 frame)
{
    Elastos_WebHistory_nativeClose(frame);
}

ECode WebBackForwardListClassic::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(Probe(EIID_IInterface), CreateWeak(this));
    INTERFACE_ADDREF(*weakReference)
    return NOERROR;
}

AutoPtr<IInterface> WebBackForwardListClassic::CreateWebHistoryItem(
    /* [in] */ Int32 nativeBridge)
{
    AutoPtr<IWebHistoryItem> result = new WebHistoryItemClassic(nativeBridge);
    return result;
}

void WebBackForwardListClassic::AddHistoryItem(
    /* [in] */ IInterface* obj,
    /* [in] */ IInterface* webHistoryItem)
{
    ((WebBackForwardListClassic*)IWebBackForwardList::Probe(obj))->AddHistoryItem(IWebHistoryItem::Probe(webHistoryItem));
    if (DebugFlags::WEB_BACK_FORWARD_LIST)
        Logger::E(LOGTAG, "WebBackForwardListClassic::AddHistoryItem");
}

void WebBackForwardListClassic::RemoveHistoryItem(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 index)
{
    ((WebBackForwardListClassic*)IWebBackForwardList::Probe(obj))->RemoveHistoryItem(index);
    if (DebugFlags::WEB_BACK_FORWARD_LIST)
        Logger::E(LOGTAG, "WebBackForwardListClassic::RemoveHistoryItem");
}

void WebBackForwardListClassic::SetCurrentIndex(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 newIndex)
{
    ((WebBackForwardListClassic*)IWebBackForwardList::Probe(obj))->SetCurrentIndex(newIndex);
    if (DebugFlags::WEB_BACK_FORWARD_LIST)
        Logger::E(LOGTAG, "WebBackForwardListClassic::SetCurrentIndex");
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
