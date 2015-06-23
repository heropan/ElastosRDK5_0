
#ifndef __MENUBUILDER_H__
#define __MENUBUILDER_H__

#include "ext/frameworkext.h"
#include "_CMenuBuilder.h"

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
