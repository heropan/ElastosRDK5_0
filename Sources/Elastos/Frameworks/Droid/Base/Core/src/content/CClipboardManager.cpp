
#include "content/CClipboardManager.h"
#include "content/CClipDataHelper.h"
#include "os/ServiceManager.h"

using Elastos::Droid::Os::ServiceManager;

namespace Elastos {
namespace Droid {
namespace Content {

AutoPtr<IClipboard> CClipboardManager::sService;
Mutex CClipboardManager::sStaticLock;
const Int32 CClipboardManager::MSG_REPORT_PRIMARY_CLIP_CHANGED = 1;

ECode CClipboardManager::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);

    switch(what) {
        case CClipboardManager::MSG_REPORT_PRIMARY_CLIP_CHANGED:
            mHost->ReportPrimaryClipChanged();
            break;
    }

    return NOERROR;
}

CClipboardManager::PrimaryClipChangedServiceListener::PrimaryClipChangedServiceListener(
    /* [in] */ CClipboardManager* context)
    : mHost(context)
{}

CClipboardManager::PrimaryClipChangedServiceListener::~PrimaryClipChangedServiceListener()
{}

CAR_INTERFACE_IMPL(CClipboardManager::PrimaryClipChangedServiceListener, IOnPrimaryClipChangedListener)

ECode CClipboardManager::PrimaryClipChangedServiceListener::DispatchPrimaryClipChanged()
{
    Boolean res = FALSE;
    return mHost->mHandler->SendEmptyMessage(
        CClipboardManager::MSG_REPORT_PRIMARY_CLIP_CHANGED, &res);
}

CClipboardManager::CClipboardManager()
    : mContext(NULL)
{
    mHandler = new MyHandler(this);
    mPrimaryClipChangedServiceListener =
        new PrimaryClipChangedServiceListener(this);
}

CClipboardManager::~CClipboardManager()
{
    mPrimaryClipChangedListeners.Clear();
}

ECode CClipboardManager::SetPrimaryClip(
    /* [in] */ IClipData* clip)
{
    AutoPtr<IClipboard> clipboard;
    FAIL_RETURN(GetService((IClipboard**)&clipboard));
    FAIL_RETURN(clipboard->SetPrimaryClip(clip));
    return NOERROR;
}

ECode CClipboardManager::GetPrimaryClip(
    /* [out] */ IClipData** clipData)
{
    VALIDATE_NOT_NULL(clipData)
    AutoPtr<IClipboard> clipboard;
    FAIL_RETURN(GetService((IClipboard**)&clipboard));
    String pkg;
    FAIL_RETURN(mContext->GetPackageName(&pkg));
    FAIL_RETURN(clipboard->GetPrimaryClip(pkg, clipData));
    return NOERROR;
}

ECode CClipboardManager::GetPrimaryClipDescription(
    /* [out] */ IClipDescription** clipDescription)
{
    VALIDATE_NOT_NULL(clipDescription)
    AutoPtr<IClipboard> clipboard;
    FAIL_RETURN(GetService((IClipboard**)&clipboard));
    FAIL_RETURN(clipboard->GetPrimaryClipDescription(clipDescription));
    return NOERROR;
}

ECode CClipboardManager::HasPrimaryClip(
    /* [out] */ Boolean* hasClip)
{
    VALIDATE_NOT_NULL(hasClip)
    AutoPtr<IClipboard> clipboard;
    FAIL_RETURN(GetService((IClipboard**)&clipboard));
    FAIL_RETURN(clipboard->HasPrimaryClip(hasClip));
    return NOERROR;
}

ECode CClipboardManager::AddPrimaryClipChangedListener(
    /* [in] */ IClipboardManagerOnPrimaryClipChangedListener* what)
{
    AutoLock lock(mPrimaryClipChangedListenersLock);
    if (mPrimaryClipChangedListeners.IsEmpty()) {
        AutoPtr<IClipboard> clipboard;
        FAIL_RETURN(GetService((IClipboard**)&clipboard));
        FAIL_RETURN(clipboard->AddPrimaryClipChangedListener(mPrimaryClipChangedServiceListener));
    }
    mPrimaryClipChangedListeners.PushBack(what);
    return NOERROR;
}

ECode CClipboardManager::RemovePrimaryClipChangedListener(
    /* [in] */ IClipboardManagerOnPrimaryClipChangedListener* what)
{
    AutoLock lock(mPrimaryClipChangedListenersLock);
    mPrimaryClipChangedListeners.Remove(what);
    if (mPrimaryClipChangedListeners.IsEmpty()) {
        AutoPtr<IClipboard> clipboard;
        FAIL_RETURN(GetService((IClipboard**)&clipboard));
        FAIL_RETURN(clipboard->RemovePrimaryClipChangedListener(mPrimaryClipChangedServiceListener));
    }
    return NOERROR;
}

ECode CClipboardManager::GetText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text)
    AutoPtr<IClipData> clip;
    FAIL_RETURN(GetPrimaryClip((IClipData**)&clip));
    Int32 count = 0;

    if (NULL != clip && (clip->GetItemCount(&count), count) > 0) {
        AutoPtr<IClipDataItem> item;
        clip->GetItemAt(0, (IClipDataItem**)&item);
        return item->CoerceToText(mContext, text);
    }

    *text = NULL;
    return NOERROR;
}

ECode CClipboardManager::SetText(
    /* [in] */ ICharSequence* text)
{
    AutoPtr<IClipDataHelper> helper;
    FAIL_RETURN(CClipDataHelper::AcquireSingleton((IClipDataHelper**)&helper));
    AutoPtr<IClipData> clipData;
    FAIL_RETURN(helper->NewPlainText((ICharSequence*) NULL, text, (IClipData**)&clipData));
    assert(clipData != NULL);
    FAIL_RETURN(SetPrimaryClip(clipData));
    return NOERROR;
}

ECode CClipboardManager::HasText(
    /* [out] */ Boolean* hasText)
{
    VALIDATE_NOT_NULL(hasText)
    AutoPtr<IClipboard> clipboard;
    FAIL_RETURN(GetService((IClipboard**)&clipboard));
    FAIL_RETURN(clipboard->HasClipboardText(hasText));
    return NOERROR;
}

ECode CClipboardManager::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;
    return NOERROR;
}

ECode CClipboardManager::GetService(
    /* [out] */ IClipboard** clipboard)
{
    VALIDATE_NOT_NULL(clipboard)
    AutoLock lock(sStaticLock);
    if (NULL == sService) {
        sService = IClipboard::Probe(ServiceManager::GetService(String("clipboard")));
    }
    *clipboard = sService;
    REFCOUNT_ADD(*clipboard);
    return NOERROR;
}

ECode CClipboardManager::ReportPrimaryClipChanged()
{
    AutoPtr<ArrayOf<IInterface*> > listeners;
    AutoLock lock(mPrimaryClipChangedListenersLock);
    const Int32 N = mPrimaryClipChangedListeners.GetSize();

    if (N <= 0) {
        return NOERROR;
    }

    listeners = ArrayOf<IInterface*>::Alloc(N);
    List<AutoPtr<IClipboardManagerOnPrimaryClipChangedListener> >::Iterator it = mPrimaryClipChangedListeners.Begin();
    Int32 index = 0;

    for (; it != mPrimaryClipChangedListeners.End(); ++it) {
        listeners->Set(index, (IInterface*)*it);
        index++;
    }

    for (Int32 i = 0; i < listeners->GetLength(); i++) {
        ((IClipboardManagerOnPrimaryClipChangedListener*)(IInterface*)(*listeners)[i])->OnPrimaryClipChanged();
    }

    return NOERROR;
}

}
}
}

