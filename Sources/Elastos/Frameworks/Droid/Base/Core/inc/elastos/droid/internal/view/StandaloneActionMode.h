#ifndef __ELASTOS_DROID_INTERNAL_VIEW_STANDALONEACTIONMODE_H__
#define __ELASTOS_DROID_INTERNAL_VIEW_STANDALONEACTIONMODE_H__

#include "elastos/droid/internal/view/ActionMode.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::Menu::IMenuBuilder;
using Elastos::Droid::View::Menu::IMenuBuilderCallback;
using Elastos::Droid::View::Menu::ISubMenuBuilder;
using Elastos::Droid::Widget::Internal::IActionBarContextView;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {

class StandaloneActionMode
    : public ElRefBase
    , public ActionMode
    , public IMenuBuilderCallback
{
public:
    StandaloneActionMode(
        /* [in] */ IContext* context,
        /* [in] */ IActionBarContextView* view,
        /* [in] */ IActionModeCallback* callback,
        /* [in] */ Boolean isFocusable);

    CAR_INTERFACE_DECL();

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI SetSubtitle(
        /* [in] */ ICharSequence* subtitle);

    CARAPI SetTitle(
        /* [in] */ Int32 resId);

    CARAPI SetSubtitle(
        /* [in] */ Int32 resId);

    CARAPI SetTitleOptionalHint(
        /* [in] */ Boolean titleOptional);

    CARAPI IsTitleOptional(
        /* [out] */ Boolean* isTitleOptional);

    CARAPI SetCustomView(
        /* [in] */ IView* view);

    CARAPI Invalidate();

    CARAPI Finish();

    CARAPI GetMenu(
        /* [out] */ IMenu** menu);

    CARAPI GetTitle(
        /* [out] */ ICharSequence** title);

    CARAPI GetSubtitle(
        /* [out] */ ICharSequence** subtitle);

    CARAPI GetCustomView(
        /* [out] */ IView** view);

    CARAPI GetMenuInflater(
        /* [out] */ IMenuInflater** menuInflater);

    CARAPI OnMenuItemSelected(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* state);

    CARAPI_(void) OnCloseMenu(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ Boolean allMenusAreClosing);

    CARAPI_(Boolean) OnSubMenuSelected(
        /* [in] */ ISubMenuBuilder* subMenu);

    CARAPI_(void) OnCloseSubMenu(
        /* [in] */ ISubMenuBuilder* menu);

    CARAPI OnMenuModeChange(
        /* [in] */ IMenuBuilder* menu);

    CARAPI_(Boolean) IsUiFocusable();

private:
    AutoPtr<IContext> mContext;
    AutoPtr<IActionBarContextView> mContextView;
    AutoPtr<IActionModeCallback> mCallback;
    AutoPtr<IWeakReference> mCustomView;
    Boolean mFinished;
    Boolean mFocusable;

    AutoPtr<IMenuBuilder> mMenu;
};

} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_VIEW_STANDALONEACTIONMODE_H__
