
#include "elastos/droid/webkit/webview/chromium/WebBackForwardListChromium.h"
#include "elastos/core/AutoLock.h"
#include "elastos/droid/webkit/webview/chromium/WebHistoryItemChromium.h"

using Elastos::Core::AutoLock;
using Elastos::Utility::CArrayList;
using Elastos::Droid::Webkit::Webview::Chromium::WebHistoryItemChromium;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {

//=====================================================================
//                      WebBackForwardListChromium
//=====================================================================
WebBackForwardListChromium::WebBackForwardListChromium(
    /* [in] */ NavigationHistory* nav_history)
{
    // ==================before translated======================
    // mCurrentIndex = nav_history.getCurrentEntryIndex();
    // mHistroryItemList = new ArrayList<WebHistoryItemChromium>(nav_history.getEntryCount());
    // for (int i = 0; i < nav_history.getEntryCount(); ++i) {
    //     mHistroryItemList.add(
    //             new WebHistoryItemChromium(nav_history.getEntryAtIndex(i)));
    // }

    assert(0);
    mCurrentIndex = nav_history->GetCurrentEntryIndex();
    CArrayList::New(nav_history->GetEntryCount(), (IList**)&mHistroryItemList);
    for (Int32 i = 0; i < nav_history->GetEntryCount(); ++i) {
        AutoPtr<WebHistoryItemChromium> itemChromium = new WebHistoryItemChromium(nav_history->GetEntryAtIndex(i));
        IInterface* interfaceTmp = itemChromium->Probe(EIID_IInterface);
        mHistroryItemList->Set(i, interfaceTmp);
    }
}

// synchronized
ECode WebBackForwardListChromium::GetCurrentItem(
    /* [out] */ IInterface/*IWebHistoryItem*/** item)

{
    AutoLock lock(this);
    VALIDATE_NOT_NULL(item);
    // ==================before translated======================
    // if (getSize() == 0) {
    //     return null;
    // } else {
    //     return getItemAtIndex(getCurrentIndex());
    // }

    Int32 size = 0;
    GetSize(&size);
    if (0 == size) {
        *item = NULL;
    }
    else {
        Int32 index = 0;
        GetCurrentIndex(&index);
        return GetItemAtIndex(index, item);
    }
    return NOERROR;
}

// synchronized
ECode WebBackForwardListChromium::GetCurrentIndex(
    /* [out] */ Int32* index)
{
    AutoLock lock(this);
    VALIDATE_NOT_NULL(index);
    // ==================before translated======================
    // return mCurrentIndex;

    *index = mCurrentIndex;
    return NOERROR;
}

// synchronized
ECode WebBackForwardListChromium::GetItemAtIndex(
    /* [in] */ Int32 index,
    /* [out] */ IInterface/*IWebHistoryItem*/** item)
{
    AutoLock lock(this);
    VALIDATE_NOT_NULL(item);
    // ==================before translated======================
    // if (index < 0 || index >= getSize()) {
    //     return null;
    // } else {
    //     return mHistroryItemList.get(index);
    // }

    Int32 size = 0;
    GetSize(&size);
    if (index < 0 || index >= size) {
        *item = NULL;
    }
    else {
        AutoPtr<IInterface> interfaceTmp;
        mHistroryItemList->Get(index, (IInterface**)&interfaceTmp);
        //*item = IWebHistoryItem::Probe(interfaceTmps);
        REFCOUNT_ADD(*item);
    }
    return NOERROR;
}

// synchronized
CARAPI WebBackForwardListChromium::GetSize(
    /* [out] */ Int32* size)
{
    AutoLock lock(this);
    VALIDATE_NOT_NULL(size);
    // ==================before translated======================
    // return mHistroryItemList.size();

    mHistroryItemList->GetSize(size);
    return NOERROR;
}

// synchronized
ECode WebBackForwardListChromium::Clone(
    /* [out] */ WebBackForwardListChromium** chromium)
{
    AutoLock lock(this);
    VALIDATE_NOT_NULL(chromium);
    // ==================before translated======================
    // List<WebHistoryItemChromium> list =
    //         new ArrayList<WebHistoryItemChromium>(getSize());
    // for (int i = 0; i < getSize(); ++i) {
    //     list.add(mHistroryItemList.get(i).clone());
    // }
    // return new WebBackForwardListChromium(list, mCurrentIndex);

    assert(0);
    Int32 size = 0;
    GetSize(&size);
    AutoPtr<IList> list;
    CArrayList::New(size, (IList**)&list);

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> itemTmp;
        mHistroryItemList->Get(i, (IInterface**)&itemTmp);
        AutoPtr<IObject> objectTmp = IObject::Probe(itemTmp);
        AutoPtr<WebHistoryItemChromium> itemChromiumTmp = (WebHistoryItemChromium*)objectTmp.Get();

        AutoPtr<WebHistoryItemChromium> cloneItem;
        itemChromiumTmp->Clone((WebHistoryItemChromium**)&cloneItem);
        IInterface* interfaceTmp = cloneItem->Probe(EIID_IInterface);
        list->Set(i, interfaceTmp);
    }

    *chromium = new WebBackForwardListChromium(list, mCurrentIndex);
    REFCOUNT_ADD(*chromium);
    return NOERROR;
}

WebBackForwardListChromium::WebBackForwardListChromium(
    /* [in] */ IList* list,
    /* [in] */ Int32 currentIndex)
    : mHistroryItemList(list)
    , mCurrentIndex(currentIndex)
{
    // ==================before translated======================
    // mHistroryItemList = list;
    // mCurrentIndex = currentIndex;
}

} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


