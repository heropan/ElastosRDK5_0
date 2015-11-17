
#ifndef __ELASTOS_DROID_INTERNAL_VIEW_MENU_CONTEXTMENUBUILDER_H__
#define __ELASTOS_DROID_INTERNAL_VIEW_MENU_CONTEXTMENUBUILDER_H__

#include "elastos/droid/internal/view/menu/ContextMenuBuilderBase.h"
#include "_Elastos_Droid_Internal_View_Menu_CContextMenuBuilder.h"

#include "elastos/droid/internal/view/menu/MenuDialogHelper.h"

using Elastos::Droid::Os::IBinder;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {
namespace Menu {

/**
 * Implementation of the {@link android.view.ContextMenu} interface.
 * <p>
 * Most clients of the menu framework will never need to touch this
 * class.  However, if the client has a window that
 * is not a content view of a Dialog or Activity (for example, the
 * view was added directly to the window manager) and needs to show
 * context menus, it will use this class.
 * <p>
 * To use this class, instantiate it via {@link #ContextMenuBuilder(Context)},
 * and optionally populate it with any of your custom items.  Finally,
 * call {@link #show(View, IBinder)} which will populate the menu
 * with a view's context menu items and show the context menu.
 */
CarClass(CContextMenuBuilder) , public ContextMenuBuilderBase
{
public:
    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    IMENU_METHODS_DECL();
    IMENUBUILDER_METHODS_DECL();

    /**
     * Sets the context menu header's icon to the icon given in <var>icon</var>
     * {@link Drawable}.
     *
     * @param icon The {@link Drawable} used for the icon.
     * @return This ContextMenu so additional setters can be called.
     */
    CARAPI SetHeaderIcon(
        /* [in] */ IDrawable* icon);

    /**
     * Sets the context menu header's icon to the icon given in <var>iconRes</var>
     * resource id.
     *
     * @param iconRes The resource identifier used for the icon.
     * @return This ContextMenu so additional setters can be called.
     */
    CARAPI SetHeaderIcon(
        /* [in] */ Int32 iconRes);

    /**
     * Sets the context menu header's title to the title given in <var>title</var>.
     *
     * @param title The character sequence used for the title.
     * @return This ContextMenu so additional setters can be called.
     */
    CARAPI SetHeaderTitle(
        /* [in] */ ICharSequence* title);

    /**
     * Sets the context menu header's title to the title given in <var>titleRes</var>
     * resource identifier.
     *
     * @param titleRes The string resource identifier used for the title.
     * @return This ContextMenu so additional setters can be called.
     */
    CARAPI SetHeaderTitle(
        /* [in] */ Int32 titleRes);

    /**
     * Sets the header of the context menu to the {@link View} given in
     * <var>view</var>. This replaces the header title and icon (and those
     * replace this).
     *
     * @param view The {@link View} used for the header.
     * @return This ContextMenu so additional setters can be called.
     */
    CARAPI SetHeaderView(
        /* [in] */ IView* view);

    CARAPI ClearHeader();

    CARAPI Show(
        /* [in] */ IView* originalView,
        /* [in] */ IBinder* token,
        /* [out] */ IMenuDialogHelper** res);
};

} // namespace Menu
} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_INTERNAL_VIEW_MENU_CONTEXTMENUBUILDER_H__
