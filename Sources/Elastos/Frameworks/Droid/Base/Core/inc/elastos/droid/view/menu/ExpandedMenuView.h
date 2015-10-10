
#ifndef __ELASTOS_DROID_VIEW_MENU_EXPANDEDMENUVIEW_H__
#define __ELASTOS_DROID_VIEW_MENU_EXPANDEDMENUVIEW_H__

#include "elastos/droid/ext/frameworkext.h"
#include "widget/ListView.h"

using Elastos::Droid::Widget::ListView;
using Elastos::Droid::Widget::IAdapterView;
using Elastos::Droid::Widget::IAdapterViewOnItemClickListener;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

/**
 * The expanded menu view is a list-like menu with all of the available menu items.  It is opened
 * by the user clicking no the 'More' button on the icon menu view.
 */
class ExpandedMenuView : public ListView
{
private:
    class OnItemClickListener
        : public IAdapterViewOnItemClickListener
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        OnItemClickListener(
            /* [in] */ ExpandedMenuView* owner)
            : mOwner(owner)
        {}

        CARAPI OnItemClick(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* v,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id);

    private:
        ExpandedMenuView* mOwner;
    };

public:
    ExpandedMenuView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Initialize(
        /* [in] */ IMenuBuilder* menu);

    CARAPI_(Boolean) InvokeItem(
        /* [in] */ IMenuItemImpl* item);

    CARAPI OnItemClick(
        /* [in] */ IAdapterView* parent,
        /* [in] */ IView* v,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    CARAPI_(Int32) GetWindowAnimations();

protected:
    ExpandedMenuView();
    /**
     * Instantiates the ExpandedMenuView that is linked with the provided MenuBuilder.
     * @param menu The model for the menu which this MenuView will display
     */
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    //@override
    virtual CARAPI OnDetachedFromWindow();

private:
    AutoPtr<IMenuBuilder> mMenu;

    /** Default animations for this menu */
    Int32 mAnimations;

};

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_MENU_EXPANDEDMENUVIEW_H__
