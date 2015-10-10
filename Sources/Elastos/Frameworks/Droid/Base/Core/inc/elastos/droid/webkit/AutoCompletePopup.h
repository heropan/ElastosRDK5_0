
#ifndef __ELASTOS_DROID_WEBKIT_AUTOCOMPLETEPOPUP_H__
#define __ELASTOS_DROID_WEBKIT_AUTOCOMPLETEPOPUP_H__

#include "elastos/droid/ext/frameworkext.h"

#include "os/Handler.h"

#include "view/View.h"


using Elastos::Core::ICharSequence;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::Handler;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::View;
using Elastos::Droid::Widget::IFilter;
using Elastos::Droid::Widget::IFilterListener;
using Elastos::Droid::Widget::IListAdapter;
using Elastos::Droid::Widget::IListPopupWindow;
using Elastos::Droid::Widget::IAdapterView;
using Elastos::Droid::Widget::IAdapterViewOnItemClickListener;
using Elastos::Droid::Widget::IPopupWindowOnDismissListener;

namespace Elastos {
namespace Droid {
namespace Webkit {

class CWebViewClassic;
class CWebViewClassicWebViewInputConnection;

class AutoCompletePopup
    : public ElRefBase
    , public IAdapterViewOnItemClickListener
    , public IFilterListener
    , public IPopupWindowOnDismissListener
{
private:
    class AnchorView
        : public ElRefBase
        , public Elastos::Droid::View::View
        , public IView
    {
    public:
        AnchorView(
            /* [in] */ IContext* context)
             : View(context)
        {
            SetFocusable(FALSE);
            SetVisibility(INVISIBLE);
        }

        CAR_INTERFACE_DECL();

        IVIEW_METHODS_DECL();
    };

public:
    AutoCompletePopup(
        /* [in] */ CWebViewClassic* webView,
        /* [in] */ CWebViewClassicWebViewInputConnection* inputConnection);

    CAR_INTERFACE_DECL();

    CARAPI_(Boolean) OnKeyPreIme(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    CARAPI_(void) SetText(
        /* [in] */ ICharSequence* text);

    CARAPI_(void) SetAutoFillQueryId(
        /* [in] */ Int32 queryId);

    CARAPI_(void) ClearAdapter();

    CARAPI_(void) SetAdapter(
        /* [in] */ IInterface* adapter);

    CARAPI_(void) ResetRect();

    // AdapterView.OnItemClickListener implementation
    CARAPI OnItemClick(
        /* [in] */ /*AdapterView<?>*/IAdapterView* parent,
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    CARAPI_(void) SetIsAutoFillProfileSet(
        /* [in] */ Boolean isAutoFillProfileSet);

    CARAPI OnFilterComplete(
        /* [in] */ Int32 count);

    CARAPI OnDismiss();

private:
    CARAPI_(void) PushTextToInputConnection();

    CARAPI_(void) EnsurePopup();

    CARAPI_(void) HandleAutoFillForm();

private:
    static const Int32 AUTOFILL_FORM = 100;
    Boolean mIsAutoFillProfileSet;
    AutoPtr<IHandler> mHandler;
    Int32 mQueryId;
    AutoPtr<IListPopupWindow> mPopup;
    AutoPtr<IFilter> mFilter;
    AutoPtr<ICharSequence> mText;
    AutoPtr<IListAdapter> mAdapter;
    AutoPtr<IView> mAnchor;
    AutoPtr<CWebViewClassicWebViewInputConnection> mInputConnection;
    AutoPtr<CWebViewClassic> mWebView;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_AUTOCOMPLETEPOPUP_H__
