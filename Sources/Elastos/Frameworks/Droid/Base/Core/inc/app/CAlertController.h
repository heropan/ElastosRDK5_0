
#ifndef __CALERTCONTROLLER_H__
#define __CALERTCONTROLLER_H__

#include "_CAlertController.h"
#include "os/HandlerBase.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IWindow;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::Widget::IButton;
using Elastos::Droid::Widget::IListView;
using Elastos::Droid::Widget::ITextView;
using Elastos::Droid::Widget::IImageView;
using Elastos::Droid::Widget::IScrollView;
using Elastos::Droid::Widget::IListAdapter;
using Elastos::Droid::Widget::ILinearLayout;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Content::IDialogInterfaceOnClickListener;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Graphics::Drawable::IDrawable;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

CarClass(CAlertController)
{
public:
    class ButtonHandler
        : public HandlerBase
    {
    public:
        // Button clicks have Message.what as the BUTTON{1,2,3} constant
        static const Int32 MSG_DISMISS_DIALOG;// = 1;

        ButtonHandler(
            /* [in] */ IDialogInterface* dialog);

        ~ButtonHandler();

        virtual CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        //private WeakReference<DialogInterface> mDialog;
        AutoPtr<IWeakReference> mDialog;
    };

    class ButtonViewOnClickListener
        : public ElRefBase
        , public IViewOnClickListener
    {
    public:
        CAR_INTERFACE_DECL();

        ButtonViewOnClickListener(
            /* [in] */ IWeakReference* host);

        ~ButtonViewOnClickListener();

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        AutoPtr<IWeakReference> mWeakHost;
    };

public:
    CAlertController();

    ~CAlertController();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IDialogInterface* di,
        /* [in] */ IWindow* window);

    CARAPI GetDialogInterface(
        /* [out] */ IDialogInterface** dialog);

    CARAPI GetSingleChoiceItemLayout(
        /* [out] */ Int32* layout);

    CARAPI GetMultiChoiceItemLayout(
        /* [out] */ Int32* layout);

    CARAPI GetListLayout(
        /* [out] */ Int32* layout);

    CARAPI GetListItemLayout(
        /* [out] */ Int32* layout);

    CARAPI SetAdapter(
        /* [in] */ IListAdapter* adapter);

    CARAPI SetCheckedItem(
        /* [in] */ Int32 checkedItem);

    CARAPI InstallContent();

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    /**
     * @see AlertDialog.Builder#setCustomTitle(View)
     */
    CARAPI SetCustomTitle(
        /* [in] */ IView* customTitleView);

    CARAPI SetMessage(
        /* [in] */ ICharSequence* message);

    /**
     * Set the view to display in the dialog.
     */
    CARAPI SetView(
        /* [in] */ IView* view);

    /**
     * Set the view to display in the dialog along with the spacing around that view
     */
    CARAPI SetView(
        /* [in] */ IView* view,
        /* [in] */ Int32 viewSpacingLeft,
        /* [in] */ Int32 viewSpacingTop,
        /* [in] */ Int32 viewSpacingRight,
        /* [in] */ Int32 viewSpacingBottom);

    /**
     * Sets a click listener or a message to be sent when the button is clicked.
     * You only need to pass one of {@code listener} or {@code msg}.
     *
     * @param whichButton Which button, can be one of
     *            {@link DialogInterface#BUTTON_POSITIVE},
     *            {@link DialogInterface#BUTTON_NEGATIVE}, or
     *            {@link DialogInterface#BUTTON_NEUTRAL}
     * @param text The text to display in positive button.
     * @param listener The {@link DialogInterface.OnClickListener} to use.
     * @param msg The {@link Message} to be sent when clicked.
     */
    CARAPI SetButton(
        /* [in] */ Int32 whichButton,
        /* [in] */ ICharSequence* text,
        /* [in] */ IDialogInterfaceOnClickListener* listener,
        /* [in] */ IMessage* msg);

    /**
     * Set resId to 0 if you don't want an icon.
     * @param resId the resourceId of the drawable to use as the icon or 0
     * if you don't want an icon.
     */
    CARAPI SetIcon(
        /* [in] */ Int32 resId);

    CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    /**
     * @param attrId the attributeId of the theme-specific drawable
     * to resolve the resourceId for.
     *
     * @return resId the resourceId of the theme-specific drawable
     */
    CARAPI GetIconAttributeResId(
        /* [in] */ Int32 attrId,
        /* [out] */ Int32* resId);

    CARAPI SetInverseBackgroundForced(
        /* [in] */ Boolean forceInverseBackground);

    CARAPI SetListView(
        /* [in] */ IListView* listview);

    CARAPI GetListView(
        /* [out] */ IListView** listview);

    CARAPI GetButton(
        /* [in] */ Int32 whichButton,
        /* [out] */ IButton** button);

    CARAPI OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

public:
    static CARAPI_(Boolean) CanTextInput(
        /* [in] */ IView* v);

private:
    static CARAPI_(Boolean) ShouldCenterSingleButton(
        /* [in] */ IContext* context);

    CARAPI_(void) SetupView();

    CARAPI_(Boolean) SetupTitle(
        /* [in] */ ILinearLayout* topPanel);

    CARAPI_(void) SetupContent(
        /* [in] */ ILinearLayout* contentPanel);

    CARAPI_(Boolean) SetupButtons();

    CARAPI_(void) CenterButton(
        /* [in] */ IButton* button);

    CARAPI_(void) SetBackground(
        /* [in] */ ILinearLayout* topPanel,
        /* [in] */ ILinearLayout* contentPanel,
        /* [in] */ IView* customPanel,
        /* [in] */ Boolean hasButtons,
        /* [in] */ ITypedArray* a,
        /* [in] */ Boolean hasTitle,
        /* [in] */ IView* buttonPanel);

private:
    AutoPtr<IContext> mContext;
    // AutoPtr<IWeakReference> mWeakDialogInterface;
    AutoPtr<IDialogInterface> mDialogInterface;  // TODO memery leak. luo.zhaohui
    AutoPtr<IWindow> mWindow;

    AutoPtr<ICharSequence> mTitle;

    AutoPtr<ICharSequence> mMessage;

    AutoPtr<IListView> mListView;

    AutoPtr<IView> mView;

    Int32 mViewSpacingLeft;

    Int32 mViewSpacingTop;

    Int32 mViewSpacingRight;

    Int32 mViewSpacingBottom;

    Boolean mViewSpacingSpecified;

    AutoPtr<IButton> mButtonPositive;
    AutoPtr<ICharSequence> mButtonPositiveText;
    AutoPtr<IMessage> mButtonPositiveMessage;
    AutoPtr<IDialogInterfaceOnClickListener> mButtonPositiveClickListener;

    AutoPtr<IButton> mButtonNegative;
    AutoPtr<ICharSequence> mButtonNegativeText;
    AutoPtr<IMessage> mButtonNegativeMessage;
    AutoPtr<IDialogInterfaceOnClickListener> mButtonNegativeClickListener;

    AutoPtr<IButton> mButtonNeutral;
    AutoPtr<ICharSequence> mButtonNeutralText;
    AutoPtr<IMessage> mButtonNeutralMessage;
    AutoPtr<IDialogInterfaceOnClickListener> mButtonNeutralClickListener;

    AutoPtr<IScrollView> mScrollView;

    Int32 mIconId;

    AutoPtr<IDrawable> mIcon;

    AutoPtr<IImageView> mIconView;

    AutoPtr<ITextView> mTitleView;

    AutoPtr<ITextView> mMessageView;

    AutoPtr<IView> mCustomTitleView;

    Boolean mForceInverseBackground;

    AutoPtr<IListAdapter> mAdapter;

    Int32 mCheckedItem;

    Int32 mAlertDialogLayout;
    Int32 mListLayout;
    Int32 mMultiChoiceItemLayout;
    Int32 mSingleChoiceItemLayout;
    Int32 mListItemLayout;

    AutoPtr<ButtonHandler> mHandler;

    AutoPtr<ButtonViewOnClickListener> mButtonHandler;
};

} // namespace App
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__CALERTCONTROLLER_H__
