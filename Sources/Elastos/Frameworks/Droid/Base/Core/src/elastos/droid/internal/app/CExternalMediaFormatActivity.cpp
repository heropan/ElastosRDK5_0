
#include "elastos/droid/app/CExternalMediaFormatActivity.h"
#include "elastos/droid/content/CIntentFilter.h"
#include "elastos/droid/content/CIntent.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Os::Storage::IExternalStorageFormatter;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

CExternalMediaFormatActivity::StorageReceiver::StorageReceiver(
    /* [in] */ CExternalMediaFormatActivity* host)
    : mHost(host)
{}

ECode CExternalMediaFormatActivity::StorageReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    Slogger::D("CExternalMediaFormatActivity", "got action %s", action.string());

    if (action == IIntent::ACTION_MEDIA_REMOVED ||
        action == IIntent::ACTION_MEDIA_CHECKING ||
        action == IIntent::ACTION_MEDIA_MOUNTED ||
        action == IIntent::ACTION_MEDIA_SHARED) {
        mHost->Finish();
    }
    return NOERROR;
}


const Int32 CExternalMediaFormatActivity::POSITIVE_BUTTON;

CExternalMediaFormatActivity::CExternalMediaFormatActivity()
{
    mStorageReceiver = new StorageReceiver(this);
}

UInt32 CExternalMediaFormatActivity::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CExternalMediaFormatActivity::Release()
{
    return ElRefBase::Release();
}

ECode CExternalMediaFormatActivity::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    AlertActivity::OnCreate(savedInstanceState);

    Slogger::D("CExternalMediaFormatActivity", "onCreate!");
    // Set up the "dialog"
    AutoPtr<CAlertControllerAlertParams> p = mAlertParams;
    p->SetIconId(0x0108008a)/*com.android.internal.R.drawable.stat_sys_warning*/;
    String str;
    AutoPtr<ICharSequence> strSequence;
    GetString(0/*com.android.internal.R.string.extmedia_format_title*/, &str);
    CString::New(str, (ICharSequence**)&strSequence);
    p->SetTitle(strSequence);
    GetString(0/*com.android.internal.R.string.extmedia_format_message*/, &str);
    strSequence = NULL;
    CString::New(str, (ICharSequence**)&strSequence);
    p->SetMessage(strSequence);
    GetString(0/*com.android.internal.R.string.extmedia_format_button_format*/, &str);
    strSequence = NULL;
    CString::New(str, (ICharSequence**)&strSequence);
    p->SetPositiveButtonText(strSequence);
    p->SetPositiveButtonListener(IDialogInterfaceOnClickListener::Probe(this));
    GetString(0/*com.android.internal.R.string.cancel*/, &str);
    strSequence = NULL;
    CString::New(str, (ICharSequence**)&strSequence);
    p->SetNegativeButtonText(strSequence);
    p->SetNegativeButtonListener(IDialogInterfaceOnClickListener::Probe(this));
    SetupAlert();
    return NOERROR;
}

ECode CExternalMediaFormatActivity::OnStart()
{
    return AlertActivity::OnStart();
}

ECode CExternalMediaFormatActivity::OnResume()
{
    AlertActivity::OnResume();

    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New((IIntentFilter**)&filter);
    filter->AddAction(IIntent::ACTION_MEDIA_REMOVED);
    filter->AddAction(IIntent::ACTION_MEDIA_CHECKING);
    filter->AddAction(IIntent::ACTION_MEDIA_MOUNTED);
    filter->AddAction(IIntent::ACTION_MEDIA_SHARED);
    AutoPtr<IIntent> intent;
    RegisterReceiver(mStorageReceiver, filter, (IIntent**)&intent);
    return NOERROR;
}

ECode CExternalMediaFormatActivity::OnPause()
{
    AlertActivity::OnPause();

    UnregisterReceiver(mStorageReceiver);
    return NOERROR;
}

ECode CExternalMediaFormatActivity::OnStop()
{
    return AlertActivity::OnStop();
}

ECode CExternalMediaFormatActivity::OnDestroy()
{
    return AlertActivity::OnDestroy();
}

ECode CExternalMediaFormatActivity::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{

    if (which == POSITIVE_BUTTON) {
        AutoPtr<IIntent> intent;
//        CIntent::New(CExternalStorageFormatter::FORMAT_ONLY, (IIntent**)&intent);
//        intent->SetComponent(CExternalStorageFormatter::COMPONENT_NAME);
        AutoPtr<IComponentName> component;
        StartService(intent, (IComponentName**)&component);
    }

    // No matter what, finish the activity
    Finish();
    return NOERROR;
}

} //namespace App
} //namespace Internal
} //namespace Droid
} //namespace Elastos
