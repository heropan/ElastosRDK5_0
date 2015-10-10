
#ifndef __ELASTOS_DROID_WIDGET_SPINNER_H__
#define __ELASTOS_DROID_WIDGET_SPINNER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "widget/AbsSpinner.h"
#include "widget/ListPopupWindow.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::App::IAlertDialog;
using Elastos::Droid::App::IAlertDialogBuilder;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IDialogInterfaceOnClickListener;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnClickListener;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Database::IDataSetObserver;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::IOnGlobalLayoutListener;
using Elastos::Droid::View::IViewTreeObserver;

namespace Elastos {
namespace Droid {
namespace Widget {

extern "C" const InterfaceID EIID_DropdownPopup;
extern "C" const InterfaceID EIID_Spinner;

class Spinner : public AbsSpinner
{
private:
    class DropDownAdapter
        : public IListAdapter
        , public ISpinnerAdapter
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        DropDownAdapter(
            /* [in] */ ISpinnerAdapter* adapter);

        CARAPI GetCount(
            /* [out] */ Int32* count);

        CARAPI GetItem(
            /* [in] */ Int32 position,
            /* [out] */ IInterface** item);

        CARAPI GetItemId(
            /* [in] */ Int32 position,
            /* [out] */ Int64* itemId);

        CARAPI GetView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent,
            /* [out] */ IView** view);

        CARAPI GetDropDownView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent,
            /* [out] */ IView** view);

        CARAPI HasStableIds(
            /* [out] */ Boolean* hasStableIds);

        CARAPI RegisterDataSetObserver(
            /* [in] */ IDataSetObserver* observer);

        CARAPI UnregisterDataSetObserver(
            /* [in] */ IDataSetObserver* observer);

        CARAPI AreAllItemsEnabled(
            /* [out] */ Boolean* enabled);

        CARAPI IsEnabled(
            /* [in] */ Int32 position,
            /* [out] */ Boolean* enabled);

        CARAPI GetItemViewType(
            /* [in] */ Int32 position,
            /* [out] */ Int32* viewType);

        CARAPI GetViewTypeCount(
            /* [out] */ Int32* count);

        CARAPI IsEmpty(
            /* [out] */ Boolean* isEmpty);

    private:
        AutoPtr<ISpinnerAdapter> mAdapter;
        AutoPtr<IListAdapter> mListAdapter;
    };

    class DialogPopup
        : public ISpinnerPopup
        , public IDialogInterfaceOnClickListener
        , public ElRefBase
    {
    public:
        DialogPopup(
            /* [in] */ Spinner* host);

        CAR_INTERFACE_DECL()

        CARAPI Dismiss();

        CARAPI IsShowing(
            /* [out] */ Boolean* res);

        CARAPI SetAdapter(
            /* [in] */ IListAdapter* adapter);

        CARAPI SetPromptText(
            /* [in] */ ICharSequence* hintText);

        CARAPI GetHintText(
            /* [out] */ ICharSequence** csq);

        CARAPI Show();

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

        CARAPI SetBackgroundDrawable(
            /* [in] */ IDrawable* bg);

        CARAPI SetVerticalOffset(
            /* [in] */ Int32 px);

        CARAPI SetHorizontalOffset(
            /* [in] */ Int32 px);

        CARAPI GetBackground(
            /* [out] */ IDrawable** d);

        CARAPI GetVerticalOffset(
            /* [out] */ Int32* offset);

        CARAPI GetHorizontalOffset(
            /* [out] */ Int32* offset);

    private:
        AutoPtr<IAlertDialog> mPopup;
        AutoPtr<IListAdapter> mListAdapter;
        AutoPtr<ICharSequence> mPrompt;
        Spinner* mHost;
    };

    class _DropdownPopup;
    class ItemClickListener
        : public IAdapterViewOnItemClickListener
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        ItemClickListener(
            /* [in] */ Spinner* spinnerHost,
            /* [in] */ _DropdownPopup* popupHost);

        ECode OnItemClick(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* view,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id);
    private:
        Spinner* mSpinnerHost;
        _DropdownPopup* mPopupHost;
    };

    class GlobalLayoutListener
        : public IOnGlobalLayoutListener
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        GlobalLayoutListener(
            /* [in] */ Spinner* spinnerHost,
            /* [in] */ _DropdownPopup* popupHost);

        CARAPI OnGlobalLayout();

    private:
        Spinner* mSpinnerHost;
        _DropdownPopup* mPopupHost;
    };

    class DismissListener
        : public IPopupWindowOnDismissListener
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        DismissListener(
            /* [in] */ Spinner* spinnerHost,
            /* [in] */ GlobalLayoutListener* layoutListener);

        CARAPI OnDismiss();

    private:
        Spinner* mSpinnerHost;
        AutoPtr<GlobalLayoutListener> mLayoutListener;
    };

    class _DropdownPopup : public ListPopupWindow
    {
    public:
        _DropdownPopup(
            /* [in] */ IContext* context,
            /* [in] */ IAttributeSet* attrs,
            /* [in] */ Int32 defStyle,
            /* [in] */ Spinner* host);

        CARAPI SetAdapter(
            /* [in] */ IListAdapter* adapter);

        CARAPI_(AutoPtr<ICharSequence>) GetHintText();

        CARAPI SetPromptText(
            /* [in] */ ICharSequence* hintText);

        CARAPI Show();

    public:
        AutoPtr<ICharSequence> mHintText;
        AutoPtr<IListAdapter> mAdapter;
        Spinner* mHost;
    };

    class DropdownPopup
        : public _DropdownPopup
        , public IListPopupWindow
        , public ISpinnerPopup
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        DropdownPopup(
            /* [in] */ IContext* context,
            /* [in] */ IAttributeSet* attrs,
            /* [in] */ Int32 defStyle,
            /* [in] */ Spinner* host);

        CARAPI SetAdapter(
            /* [in] */ IListAdapter* adapter);

        CARAPI SetPromptPosition(
            /* [in] */ Int32 position);

        CARAPI GetPromptPosition(
            /* [out] */ Int32* position);

        CARAPI SetModal(
            /* [in] */ Boolean modal);

        CARAPI IsModal(
            /* [out] */ Boolean* modal);

        CARAPI SetForceIgnoreOutsideTouch(
            /* [in] */ Boolean forceIgnoreOutsideTouch);

        CARAPI SetDropDownAlwaysVisible(
            /* [in] */ Boolean dropDownAlwaysVisible);

        CARAPI IsDropDownAlwaysVisible(
            /* [out] */ Boolean* visible);

        CARAPI SetSoftInputMode(
            /* [in] */ Int32 mode);

        CARAPI GetSoftInputMode(
            /* [out] */ Int32* mode);

        CARAPI SetListSelector(
            /* [in] */ IDrawable* selector);

        CARAPI GetBackground(
            /* [out] */ IDrawable** d);

        CARAPI SetBackgroundDrawable(
            /* [in] */ IDrawable* d);

        CARAPI SetAnimationStyle(
            /* [in] */ Int32 animationStyle);

        CARAPI GetAnimationStyle(
            /* [out] */ Int32* style);

        CARAPI GetAnchorView(
            /* [out] */ IView** view);

        CARAPI SetAnchorView(
            /* [in] */ IView* anchor);

        CARAPI GetHorizontalOffset(
            /* [out] */ Int32* offset);

        CARAPI SetHorizontalOffset(
            /* [in] */ Int32 offset);

        CARAPI GetVerticalOffset(
            /* [out] */ Int32* offset);

        CARAPI SetVerticalOffset(
            /* [in] */ Int32 offset);

        CARAPI GetWidth(
            /* [out] */ Int32* width);

        CARAPI SetWidth(
            /* [in] */ Int32 width);

        CARAPI SetContentWidth(
            /* [in] */ Int32 width);

        CARAPI GetHeight(
            /* [out] */ Int32* height);

        CARAPI SetHeight(
            /* [in] */ Int32 height);

        CARAPI SetOnItemClickListener(
            /* [in] */ IAdapterViewOnItemClickListener* clickListener);

        CARAPI SetOnItemSelectedListener(
            /* [in] */ IAdapterViewOnItemSelectedListener* selectedListener);

        CARAPI SetPromptView(
            /* [in] */ IView* prompt);

        CARAPI PostShow();

        CARAPI Show();

        CARAPI Dismiss();

        CARAPI SetOnDismissListener(
            /* [in] */ IPopupWindowOnDismissListener* listener);

        CARAPI SetInputMethodMode(
            /* [in] */ Int32 mode);

        CARAPI GetInputMethodMode(
            /* [out] */ Int32* mode);

        CARAPI SetSelection(
            /* [in] */ Int32 position);

        CARAPI ClearListSelection();

        CARAPI IsShowing(
            /* [out] */ Boolean* showing);

        CARAPI IsInputMethodNotNeeded(
            /* [out] */ Boolean* needed);

        CARAPI PerformItemClick(
            /* [in] */ Int32 position,
            /* [out] */ Boolean* click);

        CARAPI GetSelectedItem(
            /* [out] */ IInterface** item);

        CARAPI GetSelectedItemPosition(
            /* [out] */ Int32* position);

        CARAPI GetSelectedItemId(
            /* [out] */ Int64* id);

        CARAPI GetSelectedView(
            /* [out] */ IView** view);

        CARAPI GetListView(
            /* [out] */ IListView** view);

        CARAPI SetListItemExpandMax(
            /* [in] */ Int32 max);

        CARAPI OnKeyDown(
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* res);

        CARAPI OnKeyUp(
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* res);

        CARAPI OnKeyPreIme(
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* res);

        CARAPI SetPromptText(
            /* [in] */ ICharSequence* hintText);

        CARAPI GetHintText(
            /* [out] */ ICharSequence** csq);
    };

public:
    Spinner(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::spinnerStyle,
        /* [in] */ Int32 mode = MODE_THEME);

    Spinner(
        /* [in] */ IContext* context,
        /* [in] */ Int32 mode);

    CARAPI_(void) SetPopupBackgroundDrawable(
        /* [in] */ IDrawable* background);

    CARAPI_(void) SetPopupBackgroundResource(
        /* [in] */ Int32 resId);

    CARAPI_(AutoPtr<IDrawable>) GetPopupBackground();

    CARAPI_(void) SetDropDownVerticalOffset(
        /* [in] */ Int32 pixels);

    CARAPI_(Int32) GetDropDownVerticalOffset();

    CARAPI_(void) SetDropDownHorizontalOffset(
        /* [in] */ Int32 pixels);

    CARAPI_(Int32) GetDropDownHorizontalOffset();

    CARAPI_(void) SetDropDownWidth(
        /* [in] */ Int32 pixels);

    CARAPI_(Int32) GetDropDownWidth();

    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI_(void) SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI_(Int32) GetGravity();

    CARAPI SetAdapter(
        /* [in] */ IAdapter* adapter);

    CARAPI GetBaseline(
            /* [out] */ Int32* baseline);

    CARAPI SetOnItemClickListener(
        /* [in] */ IAdapterViewOnItemClickListener* l);

    CARAPI SetOnItemClickListenerInt(
        /* [in] */ IAdapterViewOnItemClickListener* l);

    CARAPI_(Boolean) PerformClick();

    CARAPI_(void) OnClick(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ Int32 which);

    CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

    CARAPI_(void) SetPrompt(
        /* [in] */ ICharSequence* prompt);

    CARAPI_(void) SetPromptId(
        /* [in] */ Int32 promptId);

    CARAPI_(AutoPtr<ICharSequence>) GetPrompt();

    CARAPI_(Int32) MeasureContentWidth(
        /* [in] */ ISpinnerAdapter* adapter,
        /* [in] */ IDrawable* background);

protected:
    Spinner();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::spinnerStyle,
        /* [in] */ Int32 mode = MODE_THEME);

    CARAPI InitImpl(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::spinnerStyle,
        /* [in] */ Int32 mode = MODE_THEME);

    virtual CARAPI OnDetachedFromWindow();

    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    CARAPI_(void) Layout(
        /* [in] */ Int32 delta,
        /* [in] */ Boolean animate);

    using AdapterView::Layout;

private:
    CARAPI_(AutoPtr<IView>) MakeAndAddView(
        /* [in] */ Int32 position);

    CARAPI_(void) SetUpChild(
        /* [in] */ IView* child);

private:
    static const Int32 MAX_ITEMS_MEASURED = 15;

    static const Int32 MODE_DIALOG = 0;

    static const Int32 MODE_DROPDOWN = 1;

    static const Int32 MODE_THEME = -1;

    static const String SPINNER_NAME;

    AutoPtr<ISpinnerPopup> mPopup;
    AutoPtr<IListAdapter> mTempAdapter;
    Int32 mDropDownWidth;

    Int32 mGravity;
    Boolean mDisableChildrenWhenDisabled;
    AutoPtr<IRect> mTempRect; // = new Rect()
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_SPINNER_H__
