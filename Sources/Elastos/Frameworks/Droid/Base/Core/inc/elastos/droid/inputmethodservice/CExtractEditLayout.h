
#ifndef __ELASTOS_DROID_INPUTMETHODSERVICE_ELASTOS_DROID_INPUTMEHTODSERVICE_CEXTRACTEDITLAYOUT_H__
#define __ELASTOS_DROID_INPUTMETHODSERVICE_ELASTOS_DROID_INPUTMEHTODSERVICE_CEXTRACTEDITLAYOUT_H__

#include "Elastos.Droid.Internal.h"
#include "_Elastos_Droid_InputMethodService_CExtractEditLayout.h"
#include "elastos/droid/widget/LinearLayout.h"
#include "elastos/droid/view/ActionMode.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Widget::LinearLayout;
using Elastos::Droid::Widget::IButton;
using Elastos::Droid::View::IMenu;
using Elastos::Droid::View::ActionMode;
using Elastos::Droid::View::IMenuItem;
using Elastos::Droid::View::IMenuInflater;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IActionMode;
using Elastos::Droid::View::IActionModeCallback;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::Internal::View::Menu::IMenuBuilder;
using Elastos::Droid::Internal::View::Menu::IMenuBuilderCallback;
using Elastos::Droid::Utility::IAttributeSet;

namespace Elastos {
namespace Droid {
namespace InputMethodService {

/**
 * CExtractEditLayout provides an ActionMode presentation for the
 * limited screen real estate in extract mode.
 *
 * @hide
 */
CarClass(CExtractEditLayout)
    , public LinearLayout
    , public IExtractEditLayout
{
protected:
    class ExtractActionMode
        : public ActionMode
        , public IMenuBuilderCallback
    {
    public:
        CAR_INTERFACE_DECL();

        ExtractActionMode(
            /* [in] */ IActionModeCallback* cb,
            /* [in] */ CExtractEditLayout* host);

        //@Override
        CARAPI SetTitle(
            /* [in] */ ICharSequence* title);

        //@Override
        CARAPI SetTitle(
            /* [in] */ Int32 resId);

        //@Override
        CARAPI SetSubtitle(
            /* [in] */ ICharSequence* subtitle);

        //@Override
        CARAPI SetSubtitle(
            /* [in] */ Int32 resId);

        //@Override
        CARAPI IsTitleOptional(
            /* [out] */ Boolean* result);

        //@Override
        CARAPI SetCustomView(
            /* [in] */ IView* view);

        //@Override
        CARAPI Invalidate();

        CARAPI_(Boolean) DispatchOnCreate();

        //@Override
        CARAPI Finish();

        //@Override
        CARAPI GetMenu(
            /* [out] */ IMenu** menu);

        //@Override
        CARAPI GetTitle(
            /* [out] */ ICharSequence** title);

        //@Override
        CARAPI GetSubtitle(
            /* [out] */ ICharSequence** subtitle);

        //@Override
        CARAPI GetCustomView(
            /* [out] */ IView** view);

        //@Override
        CARAPI GetMenuInflater(
            /* [out] */ IMenuInflater** inflater);

        //@Override
        CARAPI OnMenuItemSelected(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ IMenuItem* item,
            /* [out] */ Boolean* result);

        //@Override
        CARAPI OnMenuModeChange(
            /* [in] */ IMenuBuilder* menu);

    private:
        AutoPtr<IActionModeCallback> mCallback;
        AutoPtr<IMenuBuilder> mMenu;
        CExtractEditLayout* mHost;

        friend class CExtractEditLayout;
    };

    class _OnClickListener
        : public Object
        , public IViewOnClickListener
    {
    public:
        CAR_INTERFACE_DECL();

        _OnClickListener(
            /* [in] */ CExtractEditLayout* host);

        CARAPI OnClick(
            /* [in] */ IView* clicked);

    private:
        CExtractEditLayout* mHost;
    };

public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    /**
     * @return true if an action mode is currently active.
     */
    CARAPI IsActionModeStarted(
        /* [out] */ Boolean* started);

    /**
     * Finishes a possibly started action mode.
     */
    CARAPI FinishActionMode();

    //@Override
    CARAPI_(AutoPtr<IActionMode>) StartActionModeForChild(
        /* [in] */ IView* sourceView,
        /* [in] */ IActionModeCallback* cb);

    //@Override
    CARAPI OnFinishInflate();

protected:
    AutoPtr<ExtractActionMode> mActionMode;
    AutoPtr<IButton> mExtractActionButton;
    AutoPtr<IButton> mEditButton;
};

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos

#endif  // __ELASTOS_DROID_INPUTMETHODSERVICE_ELASTOS_DROID_INPUTMEHTODSERVICE_CEXTRACTEDITLAYOUT_H__
