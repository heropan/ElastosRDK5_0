
#ifndef __ELASTOS_DROID_WEBKIT_WEBBACKFORWARDLISTCLASSIC_H__
#define __ELASTOS_DROID_WEBKIT_WEBBACKFORWARDLISTCLASSIC_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/webkit/WebHistoryItemClassic.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;

namespace Elastos {
namespace Droid {
namespace Webkit {

struct ElaWebHistoryCallback
{
    AutoPtr<IInterface> (*createWebHistoryItem)(Int32 nativeBridge);
    void (*addHistoryItem)(IInterface* obj, IInterface* webHistoryItem);
    void (*removeHistoryItem)(IInterface* obj, Int32 index);
    void (*setCurrentIndex)(IInterface* obj, Int32 newIndex);
};

extern "C" const InterfaceID EIID_WebBackForwardListClassic;

class CallbackProxy;

/* package */
class WebBackForwardListClassic
    : public Object
    , public IWebBackForwardList
    , public IWeakReferenceSource// implements Cloneable, Serializable
{
    //friend class Elastos::Droid::Webkit::Native::WebHistory;
    friend class CWebViewClassic;
public:
    /*package*/
    WebBackForwardListClassic(
        /* [in] */ CallbackProxy* proxy);

    CAR_INTERFACE_DECL();

    CARAPI GetCurrentItem(
        /* [out] */ IWebHistoryItem** item);

    CARAPI GetCurrentIndex(
        /* [out] */ Int32* index);

    CARAPI_(AutoPtr<WebHistoryItemClassic>) GetItemAtIndex(
        /* [in] */ Int32 index);

    CARAPI GetItemAtIndex(
        /* [in] */ Int32 index,
        /* [out] */ IWebHistoryItem** item);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    /**
     * Mark the back/forward list as having a pending clear. This is used on the
     * UI side to mark the list as being invalid during the clearHistory method.
     */
    /*package*/
    virtual CARAPI_(void) SetClearPending();

    /**
     * Return the status of the clear flag. This is used on the UI side to
     * determine if the list is valid for checking things like canGoBack.
     */
    /*package*/
    virtual CARAPI_(Boolean) GetClearPending();

    /**
     * Add a new history item to the list. This will remove all items after the
     * current item and append the new item to the end of the list. Called from
     * the WebCore thread only. Synchronized because the UI thread may be
     * reading the array or the current index.
     * @param item A new history item.
     */
    /*package*/
    virtual CARAPI_(void) AddHistoryItem(
        /* [in] */ IWebHistoryItem* item);

    /**
     * Clear the back/forward list. Called from the WebCore thread.
     */
    /*package*/
    virtual CARAPI_(void) Close(
        /* [in] */ Int32 nativeFrame);

    virtual CARAPI_(AutoPtr<WebBackForwardListClassic>) Clone();

    /**
     * Set the new history index.
     * @param newIndex The new history index.
     */
    /*package*/
    virtual CARAPI_(void) SetCurrentIndex(
        /* [in] */ Int32 newIndex);

    /**
     * Restore the history index.
     */
    /*package*/
    static CARAPI_(void) RestoreIndex(
        /* [in] */ Int32 nativeFrame,
        /* [in] */ Int32 index);

    CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);

    CARAPI ToString(
        /* [out] */ String* info);

private:
    /* Remove the item at the given index. Called by JNI only. */
    CARAPI_(void) RemoveHistoryItem(
        /* [in] */ Int32 index);

    /* Close the native list. */
    static CARAPI_(void) NativeClose(
        /* [in] */ Int32 nativeFrame);

    static AutoPtr<IInterface> CreateWebHistoryItem(
        /* [in] */ Int32 nativeBridge);

    static void AddHistoryItem(
        /* [in] */ IInterface* obj,
        /* [in] */ IInterface* webHistoryItem);

    static void RemoveHistoryItem(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 index);

    static void SetCurrentIndex(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 newIndex);


    static ElaWebHistoryCallback* Init_WebHistory_Callback();

private:
    static const String LOGTAG;
    // Current position in the list.
    Int32 mCurrentIndex;
    // ArrayList of WebHistoryItems for maintaining our copy.
    List< AutoPtr<WebHistoryItemClassic> > mArray;
    // Flag to indicate that the list is invalid
    Boolean mClearPending;
    // CallbackProxy to issue client callbacks.
    AutoPtr<CallbackProxy> mCallbackProxy;

    Object mLock;

    static Object sLock;
    static ElaWebHistoryCallback* sElaWebHistoryCallback;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_WEBBACKFORWARDLISTCLASSIC_H__
