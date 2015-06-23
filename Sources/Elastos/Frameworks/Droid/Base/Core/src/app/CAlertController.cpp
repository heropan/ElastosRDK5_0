
#include "widget/CLinearLayoutLayoutParams.h"
#include "R.h"
#include "os/CMessageHelper.h"
#include "app/CAlertController.h"
#include "view/CViewGroupLayoutParams.h"
#include "util/CTypedValue.h"
#include "text/TextUtils.h"
#include "ext/frameworkext.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::R;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Droid::Utility::CTypedValue;
using Elastos::Droid::Os::EIID_IHandler;
using Elastos::Droid::Os::IMessageHelper;
using Elastos::Droid::Os::CMessageHelper;
using Elastos::Droid::Content::EIID_IDialogInterface;
using Elastos::Droid::Content::Res::IResourcesTheme;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::CViewGroupLayoutParams;
using Elastos::Droid::View::IWindowManagerLayoutParams;
using Elastos::Droid::Widget::IFrameLayout;
using Elastos::Droid::Widget::ILinearLayoutLayoutParams;
using Elastos::Droid::Widget::CLinearLayoutLayoutParams;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

//==============================================================================
//                  CAlertController::ButtonViewOnClickListener
//==============================================================================
const Int32 CAlertController::ButtonHandler::MSG_DISMISS_DIALOG = 1;

CAlertController::ButtonHandler::ButtonHandler(
    /* [in] */ IDialogInterface* dialog)
{
    Slogger::V("CAlertController", " >> create ButtonHandler(): %p", this);

    AutoPtr<IWeakReferenceSource> wr = IWeakReferenceSource::Probe(dialog);
    if (wr) {
        wr->GetWeakReference((IWeakReference**)&mDialog);
    }
}

CAlertController::ButtonHandler::~ButtonHandler()
{
    Slogger::V("CAlertController", " >> destory ButtonHandler(): %p", this);
}

ECode CAlertController::ButtonHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case IDialogInterface::BUTTON_POSITIVE:
        case IDialogInterface::BUTTON_NEGATIVE:
        case IDialogInterface::BUTTON_NEUTRAL: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            IDialogInterfaceOnClickListener* listener = IDialogInterfaceOnClickListener::Probe(obj);
            if (listener) {
                AutoPtr<IDialogInterface> dialog;
                mDialog->Resolve(EIID_IDialogInterface, (IInterface**)&dialog);
                listener->OnClick(dialog, what);
            }
            break;
        }

        case MSG_DISMISS_DIALOG: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            CAlertController* ac = (CAlertController*)IAlertController::Probe(obj);
            ac->mDialogInterface->Dismiss();
            // TODO
            // AutoPtr<IDialogInterface> dialog;
            // ac->mWeakDialogInterface->Resolve(EIID_IDialogInterface, (IInterface**)&obj);
            // if (dialog) {
            //     dialog->Dismiss();
            // }
            break;
        }
    }
    return NOERROR;
}

//==============================================================================
//                  CAlertController::ButtonViewOnClickListener
//==============================================================================
CAR_INTERFACE_IMPL(CAlertController::ButtonViewOnClickListener, IViewOnClickListener);

CAlertController::ButtonViewOnClickListener::ButtonViewOnClickListener(
    /* [in] */ IWeakReference* host)
    : mWeakHost(host)
{
    Slogger::V("CAlertController", " >> create ButtonViewOnClickListener(): %p", this);
}

CAlertController::ButtonViewOnClickListener::~ButtonViewOnClickListener()
{
    Slogger::V("CAlertController", " >> destory ~ButtonViewOnClickListener(): %p", this);
}

ECode CAlertController::ButtonViewOnClickListener::OnClick(
    /* [in] */ IView* v)
{
    AutoPtr<IAlertController> ac;
    mWeakHost->Resolve(EIID_IAlertController, (IInterface**)&ac);
    if (ac == NULL) {
        Slogger::E("CAlertController", "ButtonViewOnClickListener::OnClick, CAlertController has been destoryed!");
        return NOERROR;
    }

    CAlertController* mHost = (CAlertController*)ac.Get();

    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> m;
    if (v == IView::Probe(mHost->mButtonPositive) && mHost->mButtonPositiveMessage != NULL) {
        helper->Obtain(mHost->mButtonPositiveMessage, (IMessage**)&m);
    }
    else if (v == IView::Probe(mHost->mButtonNegative) && mHost->mButtonNegativeMessage != NULL) {
        helper->Obtain(mHost->mButtonNegativeMessage, (IMessage**)&m);
    }
    else if (v == IView::Probe(mHost->mButtonNeutral) && mHost->mButtonNeutralMessage != NULL) {
        helper->Obtain(mHost->mButtonNeutralMessage, (IMessage**)&m);
    }
    if (m != NULL) {
        m->SendToTarget();
    }

    // Post a message so we dismiss after the above handlers are executed
    AutoPtr<IMessage> msg;
    helper->Obtain(mHost->mHandler, ButtonHandler::MSG_DISMISS_DIALOG,
        ac->Probe(EIID_IAlertController), (IMessage**)&msg);
    msg->SendToTarget();

    return NOERROR;
}

//==============================================================================
//                  CAlertController
//==============================================================================

CAlertController::CAlertController()
    : mViewSpacingLeft(0)
    , mViewSpacingTop(0)
    , mViewSpacingRight(0)
    , mViewSpacingBottom(0)
    , mViewSpacingSpecified(FALSE)
    , mIconId(-1)
    , mForceInverseBackground(FALSE)
    , mCheckedItem(-1)
    , mAlertDialogLayout(0)
    , mListLayout(0)
    , mMultiChoiceItemLayout(0)
    , mSingleChoiceItemLayout(0)
    , mListItemLayout(0)
{
    Slogger::V("CAlertController", " >> create CAlertController(): %p", this);
}

CAlertController::~CAlertController()
{
    Slogger::V("CAlertController", " >> destory ~CAlertController(): %p", this);
}

Boolean CAlertController::ShouldCenterSingleButton(
    /* [in] */ IContext* context)
{
    AutoPtr<ITypedValue> outValue;
    CTypedValue::New((ITypedValue**)&outValue);
    AutoPtr<IResourcesTheme> rTheme;
    context->GetTheme((IResourcesTheme**)&rTheme);
    Boolean result;
    rTheme->ResolveAttribute(R::attr::alertDialogCenterButtons, outValue, TRUE, &result);
    Int32 data;
    outValue->GetData(&data);
    return data != 0;
}

ECode CAlertController::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IDialogInterface* di,
    /* [in] */ IWindow* window)
{
    mContext = context;
    AutoPtr<IWeakReferenceSource> wrs = IWeakReferenceSource::Probe(di);
    assert(wrs != NULL && "Error: Invalid dialog interface, IWeakReferenceSource not implemented!");
    // wrs->GetWeakReference((IWeakReference**)&mWeakDialogInterface);
    mDialogInterface = di;  // TODO memery leak. luo.zhaohui
    mWindow = window;
    mHandler = new ButtonHandler(di);
    AutoPtr<IWeakReference> wr;
    GetWeakReference((IWeakReference**)&wr);
    mButtonHandler = new ButtonViewOnClickListener(wr);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::AlertDialog),
        ARRAY_SIZE(R::styleable::AlertDialog));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(
        NULL, attrIds, R::attr::alertDialogStyle, 0, (ITypedArray**)&a);
    assert(a != NULL);
    a->GetResourceId(R::styleable::AlertDialog_layout,
            R::layout::alert_dialog, &mAlertDialogLayout);
    a->GetResourceId(
            R::styleable::AlertDialog_listLayout,
            R::layout::select_dialog, &mListLayout);
    a->GetResourceId(
            R::styleable::AlertDialog_multiChoiceItemLayout,
            R::layout::select_dialog_multichoice, &mMultiChoiceItemLayout);
    a->GetResourceId(
            R::styleable::AlertDialog_singleChoiceItemLayout,
            R::layout::select_dialog_singlechoice, &mSingleChoiceItemLayout);
    a->GetResourceId(
            R::styleable::AlertDialog_listItemLayout,
            R::layout::select_dialog_item, &mListItemLayout);

    a->Recycle();
    return NOERROR;
}

ECode CAlertController::GetDialogInterface(
    /* [out] */ IDialogInterface** dialog)
{
    VALIDATE_NOT_NULL(dialog);
    *dialog = mDialogInterface;
    REFCOUNT_ADD(*dialog);
    return NOERROR;
    // return mWeakDialogInterface->Resolve(EIID_IDialogInterface, (IInterface**)dialog);
}

ECode CAlertController::GetSingleChoiceItemLayout(
    /* [out] */ Int32* layout)
{
    VALIDATE_NOT_NULL(layout);
    *layout = mSingleChoiceItemLayout;
    return NOERROR;
}

ECode CAlertController::GetMultiChoiceItemLayout(
    /* [out] */ Int32* layout)
{
    VALIDATE_NOT_NULL(layout);
    *layout = mMultiChoiceItemLayout;
    return NOERROR;
}

ECode CAlertController::GetListLayout(
    /* [out] */ Int32* layout)
{
    VALIDATE_NOT_NULL(layout);
    *layout = mListLayout;
    return NOERROR;
}

ECode CAlertController::GetListItemLayout(
    /* [out] */ Int32* layout)
{
    VALIDATE_NOT_NULL(layout);
    *layout = mListItemLayout;
    return NOERROR;
}

ECode CAlertController::SetAdapter(
    /* [in] */ IListAdapter* adapter)
{
    mAdapter = adapter;
    return NOERROR;
}

ECode CAlertController::SetCheckedItem(
    /* [in] */ Int32 checkedItem)
{
    mCheckedItem = checkedItem;
    return NOERROR;
}

Boolean CAlertController::CanTextInput(
    /* [in] */ IView* v)
{
    Boolean result;
    v->OnCheckIsTextEditor(&result);
    if (result) {
        return TRUE;
    }

    AutoPtr<IViewGroup> vg = IViewGroup::Probe(v);
    if (NULL == vg) {
        return FALSE;
    }

    Int32 i;
    vg->GetChildCount(&i);
    AutoPtr<IView> tempView;
    while (i > 0) {
        i--;
        tempView = NULL;
        vg->GetChildAt(i, (IView**)&tempView);
        if (CanTextInput(tempView)) {
            return TRUE;
        }
    }

    return FALSE;
}

ECode CAlertController::InstallContent()
{
    /* We use a custom title so never request a window title */
    Boolean result;
    mWindow->RequestFeature(IWindow::FEATURE_NO_TITLE, &result);

    if (mView == NULL || !CanTextInput(mView)) {
        mWindow->SetFlags(IWindowManagerLayoutParams::FLAG_ALT_FOCUSABLE_IM,
                IWindowManagerLayoutParams::FLAG_ALT_FOCUSABLE_IM);
    }

    mWindow->SetContentView(mAlertDialogLayout);

    SetupView();
    return NOERROR;
}

ECode CAlertController::SetTitle(
    /* [in] */ ICharSequence* title)
{
    mTitle = title;
    if (mTitleView != NULL) {
        mTitleView->SetText(title);
    }
    return NOERROR;
}

ECode CAlertController::SetCustomTitle(
    /* [in] */ IView* customTitleView)
{
    mCustomTitleView = customTitleView;
    return NOERROR;
}

ECode CAlertController::SetMessage(
       /* [in] */ ICharSequence* message)
{
    mMessage = message;
    if (mMessageView != NULL) {
        mMessageView->SetText(message);
    }
    return NOERROR;
}

ECode CAlertController::SetView(
       /* [in] */ IView* view)
{
    mView = view;
    mViewSpacingSpecified = FALSE;
    return NOERROR;
}

ECode CAlertController::SetView(
    /* [in] */ IView* view,
    /* [in] */ Int32 viewSpacingLeft,
    /* [in] */ Int32 viewSpacingTop,
    /* [in] */ Int32 viewSpacingRight,
    /* [in] */ Int32 viewSpacingBottom)
{
    mView = view;
    mViewSpacingSpecified = TRUE;
    mViewSpacingLeft = viewSpacingLeft;
    mViewSpacingTop = viewSpacingTop;
    mViewSpacingRight = viewSpacingRight;
    mViewSpacingBottom = viewSpacingBottom;
    return NOERROR;
}

ECode CAlertController::SetButton(
    /* [in] */ Int32 whichButton,
    /* [in] */ ICharSequence* text,
    /* [in] */ IDialogInterfaceOnClickListener* listener,
    /* [in] */ IMessage* _msg)
{
    AutoPtr<IMessage> msg = _msg;
    if (msg == NULL && listener != NULL) {
        mHandler->ObtainMessage(whichButton, listener, (IMessage**)&msg);
    }

    switch (whichButton) {
        case IDialogInterface::BUTTON_POSITIVE:
            mButtonPositiveText = text;
            mButtonPositiveMessage = msg;
            mButtonPositiveClickListener = listener;
            break;

        case IDialogInterface::BUTTON_NEGATIVE:
            mButtonNegativeText = text;
            mButtonNegativeMessage = msg;
            mButtonNegativeClickListener = listener;
            break;

        case IDialogInterface::BUTTON_NEUTRAL:
            mButtonNeutralText = text;
            mButtonNeutralMessage = msg;
            mButtonNeutralClickListener = listener;
            break;

        default:
            Slogger::E("CAlertController", "SetButton: Button does not exist");
//             throw new IllegalArgumentException("Button does not exist");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CAlertController::SetIcon(
   /* [in] */ Int32 resId)
{
    mIconId = resId;
    if (mIconView != NULL) {
        if (resId > 0) {
            mIconView->SetImageResource(mIconId);
        } else if (resId == 0) {
            mIconView->SetVisibility(IView::GONE);
        }
    }
    return NOERROR;
}

ECode CAlertController::SetIcon(
   /* [in] */ IDrawable* icon)
{
    mIcon = icon;
    if ((mIconView != NULL) && (mIcon != NULL)) {
        mIconView->SetImageDrawable(icon);
    }
    return NOERROR;
}

ECode CAlertController::GetIconAttributeResId(
    /* [in] */ Int32 attrId,
    /* [out] */ Int32* resId)
{
    VALIDATE_NOT_NULL(resId);
    AutoPtr<ITypedValue> out;
    CTypedValue::New((ITypedValue**)&out);
    AutoPtr<IResourcesTheme> rTheme;
    mContext->GetTheme((IResourcesTheme**)&rTheme);
    Boolean result;
    rTheme->ResolveAttribute(attrId, out, TRUE, &result);
    out->GetResourceId(resId);
    return NOERROR;
}

ECode CAlertController::SetInverseBackgroundForced(
       /* [in] */ Boolean forceInverseBackground)
{
    mForceInverseBackground = forceInverseBackground;
    return NOERROR;
}

ECode CAlertController::SetListView(
    /* [in] */ IListView* listview)
{
    mListView = listview;
    return NOERROR;
}

ECode CAlertController::GetListView(
       /* [out] */ IListView** listview)
{
    VALIDATE_NOT_NULL(listview);
    *listview = mListView;
    REFCOUNT_ADD(*listview);
    return NOERROR;
}

ECode CAlertController::GetButton(
    /* [in] */ Int32 whichButton,
    /* [out] */ IButton** button)
{
    VALIDATE_NOT_NULL(button);
    *button = NULL;
    switch (whichButton) {
        case IDialogInterface::BUTTON_POSITIVE:
            *button = mButtonPositive;
            break;
        case IDialogInterface::BUTTON_NEGATIVE:
            *button = mButtonNegative;
            break;
        case IDialogInterface::BUTTON_NEUTRAL:
            *button = mButtonNeutral;
            break;
        default:
            Slogger::E("CAlertController", "GetButton: Button does not exist");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    REFCOUNT_ADD(*button);
    return NOERROR;
}

ECode CAlertController::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean handled;
    mScrollView->ExecuteKeyEvent(event, &handled);
    *result = mScrollView != NULL && handled;
    return NOERROR;
}

ECode CAlertController::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean handled;
    mScrollView->ExecuteKeyEvent(event, &handled);
    *result = mScrollView != NULL && handled;
    return NOERROR;
}

void CAlertController::SetupView()
{
    AutoPtr<IView> tempView;
    mWindow->FindViewById(R::id::contentPanel, (IView**)&tempView);
    AutoPtr<ILinearLayout> contentPanel = ILinearLayout::Probe(tempView);
    assert(contentPanel != NULL);

    SetupContent(contentPanel);

    Boolean hasButtons = SetupButtons();

    tempView = NULL;
    mWindow->FindViewById(R::id::topPanel, (IView**)&tempView);
    AutoPtr<ILinearLayout> topPanel = ILinearLayout::Probe(tempView);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::AlertDialog),
        ARRAY_SIZE(R::styleable::AlertDialog));
    AutoPtr<ITypedArray> a;
    mContext->ObtainStyledAttributes(
        NULL, attrIds, R::attr::alertDialogStyle, 0, (ITypedArray**)&a);

    Boolean hasTitle = SetupTitle(topPanel);

    AutoPtr<IView> buttonPanel;
    mWindow->FindViewById(R::id::buttonPanel, (IView**)&buttonPanel);
    if (!hasButtons) {
        buttonPanel->SetVisibility(IView::GONE);
        mWindow->SetCloseOnTouchOutsideIfNotSet(TRUE);
    }

    AutoPtr<IFrameLayout> customPanel;
    if (mView != NULL) {
        tempView = NULL;
        mWindow->FindViewById(R::id::customPanel, (IView**)&tempView);
        customPanel = IFrameLayout::Probe(tempView);

        tempView = NULL;
        mWindow->FindViewById(R::id::custom, (IView**)&tempView);
        AutoPtr<IFrameLayout> custom = IFrameLayout::Probe(tempView);

        AutoPtr<IViewGroupLayoutParams> lParams;
        CViewGroupLayoutParams::New(IViewGroupLayoutParams::MATCH_PARENT, IViewGroupLayoutParams::MATCH_PARENT,
                (IViewGroupLayoutParams**)&lParams);
        custom->AddView(mView, lParams);
        if (mViewSpacingSpecified) {
            custom->SetPadding(mViewSpacingLeft, mViewSpacingTop, mViewSpacingRight,
                    mViewSpacingBottom);
        }
        if (mListView != NULL) {
            AutoPtr<ILinearLayoutLayoutParams> vglParams;
            customPanel->GetLayoutParams((IViewGroupLayoutParams**)&vglParams);
            vglParams->SetWeight(0);
        }
    }
    else {
        tempView = NULL;
        mWindow->FindViewById(R::id::customPanel, (IView**)&tempView);
        AutoPtr<IFrameLayout> fLayout = IFrameLayout::Probe(tempView);
        fLayout->SetVisibility(IView::GONE);
    }

    /* Only display the divider if we have a title and a
     * custom view or a message.
     */
    if (hasTitle) {
        AutoPtr<IView> divider;
        if (mMessage != NULL || mView != NULL || mListView != NULL) {
            mWindow->FindViewById(R::id::titleDivider, (IView**)&divider);
        } else {
            mWindow->FindViewById(R::id::titleDividerTop, (IView**)&divider);
        }

        if (divider != NULL) {
            divider->SetVisibility(IView::VISIBLE);
        }
    }

    SetBackground(topPanel, contentPanel, customPanel, hasButtons, a, hasTitle, buttonPanel);

    a->Recycle();
}

Boolean CAlertController::SetupTitle(
    /* [in] */ ILinearLayout* topPanel)
{
    Boolean hasTitle = TRUE;

    if (mCustomTitleView != NULL) {
        // Add the custom title view directly to the topPanel layout
        AutoPtr<ILinearLayoutLayoutParams> lp;
        CLinearLayoutLayoutParams::New(
                ILinearLayoutLayoutParams::MATCH_PARENT, ILinearLayoutLayoutParams::WRAP_CONTENT,
                (ILinearLayoutLayoutParams**)&lp);

        topPanel->AddView(mCustomTitleView, 0, lp);

        // Hide the title template
        AutoPtr<IView> titleTemplate;
        mWindow->FindViewById(R::id::title_template, (IView**)&titleTemplate);
        titleTemplate->SetVisibility(IView::GONE);
    }
    else {
        Boolean hasTextTitle = !TextUtils::IsEmpty(mTitle);

        AutoPtr<IView> tmpView;
        mWindow->FindViewById(R::id::icon, (IView**)&tmpView);
        mIconView = IImageView::Probe(tmpView.Get());
        assert(mIconView != NULL);

        if (hasTextTitle) {
            /* Display the title if a title is supplied, else hide it */
            tmpView = NULL;
            mWindow->FindViewById(R::id::alertTitle, (IView**)&tmpView);
            mTitleView = ITextView::Probe(tmpView.Get());
            assert(mTitleView != NULL);
            mTitleView->SetText(mTitle);

            /* Do this last so that if the user has supplied any
             * icons we use them instead of the default ones. If the
             * user has specified 0 then make it disappear.
             */
            if (mIconId > 0) {
                mIconView->SetImageResource(mIconId);
            }
            else if (mIcon != NULL) {
                mIconView->SetImageDrawable(mIcon);
            }
            else if (mIconId == 0) {
                /* Apply the padding from the icon to ensure the
                 * title is aligned correctly.
                 */
                Int32 left, top, right, bottom;
                mIconView->GetPaddingLeft(&left);
                mIconView->GetPaddingTop(&top);
                mIconView->GetPaddingRight(&right);
                mIconView->GetPaddingBottom(&bottom);
                mTitleView->SetPadding(left, top, right, bottom);
                mIconView->SetVisibility(IView::GONE);
            }
        }
        else {
            // Hide the title template
            AutoPtr<IView> titleTemplate;
            mWindow->FindViewById(R::id::title_template, (IView**)&titleTemplate);
            titleTemplate->SetVisibility(IView::GONE);
            mIconView->SetVisibility(IView::GONE);
            topPanel->SetVisibility(IView::GONE);
            hasTitle = FALSE;
        }
    }
    return hasTitle;
}

void CAlertController::SetupContent(
    /* [in] */ ILinearLayout* contentPanel)
{
    AutoPtr<IView> tmpView;
    mWindow->FindViewById(R::id::scrollView, (IView**)&tmpView);
    mScrollView = IScrollView::Probe(tmpView.Get());
    assert(mScrollView != NULL);
    mScrollView->SetFocusable(FALSE);

    // Special case for users that only want to display a String
    tmpView = NULL;
    mWindow->FindViewById(R::id::message, (IView**)&tmpView);
    mMessageView = ITextView::Probe(tmpView.Get());
    if (mMessageView == NULL) {
        return;
    }

    if (mMessage != NULL) {
        mMessageView->SetText(mMessage);
    }
    else {
        mMessageView->SetVisibility(IView::GONE);
        mScrollView->RemoveViewInLayout(mMessageView);

        if (mListView != NULL) {
            contentPanel->RemoveViewInLayout(mScrollView);

            Int32 lp = ILinearLayoutLayoutParams::MATCH_PARENT;
            AutoPtr<ILinearLayoutLayoutParams> linearParams;
            CLinearLayoutLayoutParams::New(lp, lp, (ILinearLayoutLayoutParams**)&linearParams);
            contentPanel->AddView(
                IView::Probe(mListView.Get()),
                IViewGroupLayoutParams::Probe(linearParams.Get()));

            AutoPtr<ILinearLayoutLayoutParams> lParams;
            CLinearLayoutLayoutParams::New(lp, lp, 1.0f, (ILinearLayoutLayoutParams**)&lParams);
            contentPanel->SetLayoutParams(lParams);
        }
        else {
            contentPanel->SetVisibility(IView::GONE);
        }
    }
}

Boolean CAlertController::SetupButtons()
{
    Int32 BIT_BUTTON_POSITIVE = 1;
    Int32 BIT_BUTTON_NEGATIVE = 2;
    Int32 BIT_BUTTON_NEUTRAL = 4;
    Int32 whichButtons = 0;

    AutoPtr<IView> tempView;
    mWindow->FindViewById(R::id::button1, (IView**)&tempView);
    mButtonPositive = IButton::Probe(tempView);
    assert(mButtonPositive != NULL);
    mButtonPositive->SetOnClickListener(mButtonHandler);

    if (TextUtils::IsEmpty(mButtonPositiveText)) {
        mButtonPositive->SetVisibility(IView::GONE);
    } else {
        mButtonPositive->SetText(mButtonPositiveText);
        mButtonPositive->SetVisibility(IView::VISIBLE);
        whichButtons = whichButtons | BIT_BUTTON_POSITIVE;
    }

    tempView = NULL;
    mWindow->FindViewById(R::id::button2, (IView**)&tempView);
    mButtonNegative = IButton::Probe(tempView);
    mButtonNegative->SetOnClickListener(mButtonHandler);

    if (TextUtils::IsEmpty(mButtonNegativeText)) {
        mButtonNegative->SetVisibility(IView::GONE);
    } else {
        mButtonNegative->SetText(mButtonNegativeText);
        mButtonNegative->SetVisibility(IView::VISIBLE);

        whichButtons = whichButtons | BIT_BUTTON_NEGATIVE;
    }

    tempView = NULL;
    mWindow->FindViewById(R::id::button3, (IView**)&tempView);
    mButtonNeutral = IButton::Probe(tempView);
    mButtonNeutral->SetOnClickListener(mButtonHandler);

    if (TextUtils::IsEmpty(mButtonNeutralText)) {
        mButtonNeutral->SetVisibility(IView::GONE);
    } else {
        mButtonNeutral->SetText(mButtonNeutralText);
        mButtonNeutral->SetVisibility(IView::VISIBLE);

        whichButtons = whichButtons | BIT_BUTTON_NEUTRAL;
    }

    if (ShouldCenterSingleButton(mContext)) {
        /*
         * If we only have 1 button it should be centered on the layout and
         * expand to fill 50% of the available space.
         */
        if (whichButtons == BIT_BUTTON_POSITIVE) {
            CenterButton(mButtonPositive);
        } else if (whichButtons == BIT_BUTTON_NEGATIVE) {
            CenterButton(mButtonNeutral);
        } else if (whichButtons == BIT_BUTTON_NEUTRAL) {
            CenterButton(mButtonNeutral);
        }
    }

    return whichButtons != 0;
}

void CAlertController::CenterButton(
    /* [in] */ IButton* button)
{
    AutoPtr<ILinearLayoutLayoutParams> params;
    button->GetLayoutParams((IViewGroupLayoutParams**)&params);
    params->SetGravity(IGravity::CENTER_HORIZONTAL);
    params->SetWeight(0.5f);
    button->SetLayoutParams(params);
    AutoPtr<IView> leftSpacer;
    mWindow->FindViewById(R::id::leftSpacer, (IView**)&leftSpacer);
    if (leftSpacer != NULL) {
        leftSpacer->SetVisibility(IView::VISIBLE);
    }
    AutoPtr<IView> rightSpacer;
    mWindow->FindViewById(R::id::rightSpacer, (IView**)&rightSpacer);
    if (rightSpacer != NULL) {
        rightSpacer->SetVisibility(IView::VISIBLE);
    }
}

void CAlertController::SetBackground(
    /* [in] */ ILinearLayout* topPanel,
    /* [in] */ ILinearLayout* contentPanel,
    /* [in] */ IView* customPanel,
    /* [in] */ Boolean hasButtons,
    /* [in] */ ITypedArray* a,
    /* [in] */ Boolean hasTitle,
    /* [in] */ IView* buttonPanel)
{
    /* Get all the different background required */
    Int32 fullDark;
    a->GetResourceId(
            R::styleable::AlertDialog_fullDark,  R::drawable::popup_full_dark, &fullDark);
    Int32 topDark;
    a->GetResourceId(
            R::styleable::AlertDialog_topDark,  R::drawable::popup_top_dark, &topDark);
    Int32 centerDark;
    a->GetResourceId(
            R::styleable::AlertDialog_centerDark,  R::drawable::popup_center_dark, &centerDark);
    Int32 bottomDark;
    a->GetResourceId(
            R::styleable::AlertDialog_bottomDark,  R::drawable::popup_bottom_dark, &bottomDark);
    Int32 fullBright;
    a->GetResourceId(
            R::styleable::AlertDialog_fullBright,  R::drawable::popup_full_bright, &fullBright);
    Int32 topBright;
    a->GetResourceId(
            R::styleable::AlertDialog_topBright,  R::drawable::popup_top_bright, &topBright);
    Int32 centerBright;
    a->GetResourceId(
            R::styleable::AlertDialog_centerBright,  R::drawable::popup_center_bright, &centerBright);
    Int32 bottomBright;
    a->GetResourceId(
            R::styleable::AlertDialog_bottomBright,  R::drawable::popup_bottom_bright, &bottomBright);
    Int32 bottomMedium;
    a->GetResourceId(
            R::styleable::AlertDialog_bottomMedium,  R::drawable::popup_bottom_medium, &bottomMedium);

    /*
     * We now set the background of all of the sections of the alert.
     * First collect together each section that is being displayed along
     * with whether it is on a light or dark background, then run through
     * them setting their backgrounds.  This is complicated because we need
     * to correctly use the full, top, middle, and bottom graphics depending
     * on how many views they are and where they appear.
     */

    AutoPtr<ArrayOf<IView*> > views = ArrayOf<IView*>::Alloc(4);
    AutoPtr<ArrayOf<Boolean> > light = ArrayOf<Boolean>::Alloc(4);
    AutoPtr<IView> lastView = NULL;
    Boolean lastLight = FALSE;

    Int32 pos = 0;
    if (hasTitle) {
        views->Set(pos, topPanel);
        (*light)[pos] = FALSE;
        pos++;
    }

    /* The contentPanel displays either a custom text message or
     * a ListView. If it's text we should use the dark background
     * for ListView we should use the light background. If neither
     * are there the contentPanel will be hidden so set it as null.
     */
    Int32 visibility;
    contentPanel->GetVisibility(&visibility);
    if (visibility == IView::GONE) {
        views->Set(pos, NULL);
    } else {
        views->Set(pos, contentPanel);
    }

    (*light)[pos] = mListView != NULL;
    pos++;
    if (customPanel != NULL) {
        views->Set(pos, customPanel);
        (*light)[pos] = mForceInverseBackground;
        pos++;
    }
    if (hasButtons) {
        views->Set(pos, buttonPanel);
        (*light)[pos] = TRUE;
    }

    Boolean setView = FALSE;
    for (pos = 0; pos < views->GetLength(); pos++) {
        AutoPtr<IView> v = (*views)[pos];
        if (v == NULL) {
            continue;
        }
        if (lastView != NULL) {
            if (!setView) {
                lastView->SetBackgroundResource(lastLight ? topBright : topDark);
            } else {
                lastView->SetBackgroundResource(lastLight ? centerBright : centerDark);
            }
            setView = TRUE;
        }
        lastView = v;
        lastLight = (*light)[pos];
    }

    if (lastView != NULL) {
        if (setView) {

            /* ListViews will use the Bright background but buttons use
             * the Medium background.
             */
            lastView->SetBackgroundResource(
                    lastLight ? (hasButtons ? bottomMedium : bottomBright) : bottomDark);
        } else {
            lastView->SetBackgroundResource(lastLight ? fullBright : fullDark);
        }
    }

    /* TODO: uncomment section below. The logic for this should be if
     * it's a Contextual menu being displayed AND only a Cancel button
     * is shown then do this.
     */
    //      if (hasButtons && (mListView != null)) {

        /* Yet another *special* case. If there is a ListView with buttons
         * don't put the buttons on the bottom but instead put them in the
         * footer of the ListView this will allow more items to be
         * displayed.
         */

        /*
        contentPanel.setBackgroundResource(bottomBright);
        buttonPanel.setBackgroundResource(centerMedium);
        ViewGroup parent = (ViewGroup) mWindow.findViewById(R.id.parentPanel);
        parent.removeView(buttonPanel);
        AbsListView.LayoutParams params = new AbsListView.LayoutParams(
                AbsListView.LayoutParams.MATCH_PARENT,
                AbsListView.LayoutParams.MATCH_PARENT);
        buttonPanel.setLayoutParams(params);
        mListView.addFooterView(buttonPanel);
        */
    //    }

    if ((mListView != NULL) && (mAdapter != NULL)) {
        mListView->SetAdapter(mAdapter);
        if (mCheckedItem > -1) {
            mListView->SetItemChecked(mCheckedItem, TRUE);
            mListView->SetSelection(mCheckedItem);
        }
    }
}

} // namespace App
} // namespace Internal
} // namespace Droid
} // namespace Elastos
