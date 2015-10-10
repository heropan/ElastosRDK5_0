
#ifndef __ELASTOS_DROID_VIEW_MENU_MENUBUILDER_H__
#define __ELASTOS_DROID_VIEW_MENU_MENUBUILDER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "_Elastos_Droid_View_Menu_CMenuBuilder.h"

#include "view/menu/MenuBuilderBase.h"



namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CarClass (CMenuBuilder) , public MenuBuilderBase
{
public:
    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IMENU_METHODS_DECL();
    IMENUBUILDER_METHODS_DECL();
};

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif
