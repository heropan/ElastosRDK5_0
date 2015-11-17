
#ifndef __ELASTOS_DROID_INTERNAL_VIEW_MENU_MENUBUILDER_H__
#define __ELASTOS_DROID_INTERNAL_VIEW_MENU_MENUBUILDER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "_Elastos_Droid_Internal_View_Menu_CMenuBuilder.h"

#include "elastos/droid/internal/view/menu/MenuBuilder.h"


namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {
namespace Menu {

CarClass (CMenuBuilder) , public MenuBuilder
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
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif
