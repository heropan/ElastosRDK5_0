
#ifndef __ELASTOS_DROID_VIEW_MENU_CEXPANDEDMENUVIEW_H__
#define __ELASTOS_DROID_VIEW_MENU_CEXPANDEDMENUVIEW_H__

#include "elastos/droid/ext/frameworkext.h"




#include "_Elastos_Droid_View_Menu_CExpandedMenuView.h"
#include "elastos/droid/view/menu/ExpandedMenuView.h"

using Elastos::Droid::Text::IEditable;
using Elastos::Droid::Widget::IAdapterViewOnItemLongClickListener;
using Elastos::Droid::Widget::IAdapterViewOnItemSelectedListener;
using Elastos::Droid::Widget::IAbsListViewOnScrollListener;
using Elastos::Droid::Widget::IMultiChoiceModeListener;
using Elastos::Droid::Widget::IRecyclerListener;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CarClass(CExpandedMenuView), public ExpandedMenuView
{
public:
    /**
     * Instantiates the ExpandedMenuView that is linked with the provided MenuBuilder.
     * @param menu The model for the menu which this MenuView will display
     */
    constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IVIEW_METHODS_DECL()

    IVIEWGROUP_METHODS_DECL()

    IVIEWPARENT_METHODS_DECL()

    IVIEWMANAGER_METHODS_DECL()

    IADAPTERVIEW_METHODS_DECL()

    IABSLISTVIEW_METHODS_DECL()

    ILISTVIEW_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    IKEYEVENTCALLBACK_METHODS_DECL()

    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    //from IMenuBuilderItemInvoker
    CARAPI InvokeItem(
        /* [in] */ IMenuItemImpl* item,
        /* [out] */ Boolean* state);

    //from IMenuView
    CARAPI Initialize(
        /* [in] */ IMenuBuilder* menu);

    CARAPI GetWindowAnimations(
        /* [out] */ Int32* animations);

    //from IAdapterViewOnItemClickListener
    CARAPI OnItemClick(
        /* [in] */ IAdapterView* parent,
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    CARAPI OnTouchModeChanged(
        /* [in] */ Boolean isInTouchMode);

    CARAPI OnGlobalLayout();

    CARAPI BeforeTextChanged(
        /* [in] */ ICharSequence* s,
        /* [in] */ Int32 start,
        /* [in] */ Int32 count,
        /* [in] */ Int32 after);

    CARAPI OnTextChanged(
        /* [in] */ ICharSequence* s,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 count);

    CARAPI AfterTextChanged(
        /* [in] */ IEditable* s);

    CARAPI OnFilterComplete(
        /* [in] */ Int32 count);

    CARAPI OnRemoteAdapterConnected(
        /* [out] */  Boolean* rst);

    CARAPI OnRemoteAdapterDisconnected();

    CARAPI DeferNotifyDataSetChanged();
};

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_MENU_CEXPANDEDMENUVIEW_H__
