
#include "elastos/droid/webkit/JsDialogHelper.h"
#include "elastos/droid/webkit/URLUtil.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::App::IActivity;
using Elastos::Droid::App::IAlertDialog;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnCancelListener;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnClickListener;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::IView;
using Elastos::Droid::Widget::ITextView;
using Elastos::Core::ICharSequence;
using Elastos::Core::StringBuilder;
using Elastos::Net::IURL;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//                 JsDialogHelper::CancelListener
//===============================================================

CAR_INTERFACE_IMPL_2(JsDialogHelper::CancelListener, Object, IDialogInterfaceOnCancelListener, IDialogInterfaceOnClickListener);

JsDialogHelper::CancelListener::CancelListener(
    /* [in] */ JsDialogHelper* owner)
    : mOwner(owner)
{
}

ECode JsDialogHelper::CancelListener::OnCancel(
    /* [in] */ IDialogInterface* dialog)
{
    return IJsResult::Probe(mOwner->mResult)->Cancel();
}

ECode JsDialogHelper::CancelListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    return IJsResult::Probe(mOwner->mResult)->Cancel();
}

//===============================================================
//                 JsDialogHelper::PositiveListener
//===============================================================

CAR_INTERFACE_IMPL(JsDialogHelper::PositiveListener, Object, IDialogInterfaceOnClickListener);

JsDialogHelper::PositiveListener::PositiveListener(
    /* [in] */ JsDialogHelper* owner,
    /* [in] */ IEditText* edit)
    : mOwner(owner)
    , mEdit(edit)
{
}

ECode JsDialogHelper::PositiveListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    if (mEdit == NULL) {
        assert(0);
        // TODO
        // mOwner->mResult->Confirm();
    }
    else {
        AutoPtr<ICharSequence> cs;
        ITextView::Probe(mEdit)->GetText((ICharSequence**)&cs);
        String str;
        cs->ToString(&str);
        mOwner->mResult->Confirm(str);
    }

    return NOERROR;
}

//===============================================================
//                      JsDialogHelper
//===============================================================

const String JsDialogHelper::TAG("JsDialogHelper");

CAR_INTERFACE_IMPL(JsDialogHelper, Object, IJsDialogHelper);

ECode JsDialogHelper::constructor(
    /* [in] */ IJsPromptResult* result,
    /* [in] */ Int32 type,
    /* [in] */ const String& defaultValue,
    /* [in] */ const String& message,
    /* [in] */ const String& url)
{
    mResult = result;
    mDefaultValue = defaultValue;
    mMessage = message;
    mType = type;
    mUrl = url;
    return NOERROR;
}

ECode JsDialogHelper::constructor(
    /* [in] */ IJsPromptResult* result,
    /* [in] */ IMessage* msg)
{
    mResult = result;
    AutoPtr<IBundle> data;
    msg->GetData((IBundle**)&data);
    data->GetString(String("default"), &mDefaultValue);
    data->GetString(String("message"), &mMessage);
    data->GetInt32(String("type"), &mType);
    data->GetString(String("url"), &mUrl);
    return NOERROR;
}

ECode JsDialogHelper::InvokeCallback(
    /* [in] */ IWebChromeClient* client,
    /* [in] */ IWebView* webView,
    /* [out] */ Boolean* result)
{
    switch (mType) {
        case ALERT:
            return client->OnJsAlert(webView, mUrl, mMessage, IJsResult::Probe(mResult), result);
        case CONFIRM:
            return client->OnJsConfirm(webView, mUrl, mMessage, IJsResult::Probe(mResult), result);
        case UNLOAD:
            return client->OnJsBeforeUnload(webView, mUrl, mMessage, IJsResult::Probe(mResult), result);
        case PROMPT:
            return client->OnJsPrompt(webView, mUrl, mMessage, mDefaultValue, mResult, result);
        default:
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
}

ECode JsDialogHelper::ShowDialog(
    /* [in] */ IContext* context)
{
    assert(0);
    // TODO
    // if (!CanShowAlertDialog(context)) {
    //     Logger::W(TAG, "Cannot create a dialog, the WebView context is not an Activity");
    //     return mResult->Cancel();
    // }

    // String title, displayMessage;
    // Int32 positiveTextId, negativeTextId;
    // if (mType == UNLOAD) {
    //     context->GetString(com::android::internal::R::string::js_dialog_before_unload_title, &title);
    //     context->GetString(
    //             com::android::internal::R::string::js_dialog_before_unload, mMessage, &displayMessage);
    //     positiveTextId = com::android::internal::R::string::js_dialog_before_unload_positive_button;
    //     negativeTextId = com::android::internal::R::string::js_dialog_before_unload_negative_button;
    // }
    // else {
    //     title = GetJsDialogTitle(context);
    //     displayMessage = mMessage;
    //     positiveTextId = com::android::internal::R::string::ok;
    //     negativeTextId = com::android::internal::R::string::cancel;
    // }
    // AutoPtr<IAlertDialogBuilder> builder;
    // CAlertDialogBuilder::New(context, (IAlertDialogBuilder**)&builder);
    // builder->SetTitle(title);
    // AutoPtr<IDialogInterfaceOnCancelListener> listener = new CancelListener(this);
    // builder->SetOnCancelListener(listener);
    // if (mType != PROMPT) {
    //     builder->SetMessage(displayMessage);
    //     AutoPtr<IDialogInterfaceOnClickListener> positiveListener = new PositiveListener(this, NULL);
    //     builder->SetPositiveButton(positiveTextId, positiveListener);
    // }
    // else {
    //     AutoPtr<ILayoutInflaterHelper> helper;
    //     CLayoutInflaterHelper::AcquireSingleton((ILayoutInflaterHelper**)&helper);
    //     AutoPtr<ILayoutInflater> flater;
    //     AutoPtr<IView> view;
    //     flater->Inflate(
    //             com::android::internal::R::layout::js_prompt, NULL, (IView**)&view);
    //     AutoPtr<IEditText> edit;
    //     view->FindViewById(com::android::internal::R::id::value, (IInterface**)&edit);
    //     edit->SetText(mDefaultValue);
    //     AutoPtr<IDialogInterfaceOnClickListener> positiveListener = new PositiveListener(this, edit);
    //     builder->SetPositiveButton(positiveTextId, positiveListener);
    //     AutoPtr<ITextView> textView;
    //     view->FindViewById(com::android::internal::R::id::message, (ITextView**)&textView);
    //     textView->SetText(mMessage);
    //     builder->SetView(view);
    // }

    // if (mType != ALERT) {
    //     AutoPtr<IDialogInterfaceOnClickListener> listener = new CancelListener(this);
    //     builder->SetNegativeButton(negativeTextId, listener);
    // }

    // return builder->Show();

    return E_NOT_IMPLEMENTED;
}

String JsDialogHelper::GetJsDialogTitle(
    /* [in] */ IContext* context)
{
    assert(0);
    // TODO
    // String title = mUrl;
    // if (URLUtil::IsDataUrl(mUrl)) {
    //     // For data: urls, we just display 'JavaScript' similar to Chrome.
    //      context->GetString(com::android::internal::R::string::js_dialog_title_default, &title);
    // } else {
    //     // try {
    //         AutoPtr<IURL> alertUrl;
    //         CURL::New(mUrl, (IURL**)&alertUrl);
    //         // For example: "The page at 'http://www.mit.edu' says:"
    //         String protocol, host;
    //         alertUrl->GetProtocol(&protocol);
    //         alertUrl->GetHost(&host);
    //         StringBuilder str(host);
    //         str += "://";
    //         str += host;
    //         context->GetString(com::android::internal::R::string::js_dialog_title,
    //                 str.ToString(), &title);
    //     // } catch (MalformedURLException ex) {
    //     //     // do nothing. just use the url as the title
    //     // }
    // }
    // return title;
    return String(NULL);
}

Boolean JsDialogHelper::CanShowAlertDialog(
    /* [in] */ IContext* context)
{
    // We can only display the alert dialog if mContext is
    // an Activity context.
    // FIXME: Should we display dialogs if mContext does
    // not have the window focus (e.g. if the user is viewing
    // another Activity when the alert should be displayed) ?
    // See bug 3166409
    return IActivity::Probe(context) != NULL;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
