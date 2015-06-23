
#include "view/menu/CMenuBuilder.h"


namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

ECode CMenuBuilder::constructor(
    /*[in]*/ IContext* context)
{
    MenuBuilderBase::Init(context);
    return NOERROR;
}

PInterface CMenuBuilder::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_MenuBuilderBase == riid) {
        return reinterpret_cast<PInterface>((MenuBuilderBase *)this);
    }

    return _CMenuBuilder::Probe(riid);
}

IMENU_METHODS_IMPL(CMenuBuilder, MenuBuilderBase, NULL);
IMENUBUILDER_METHODS_IMPL(CMenuBuilder, MenuBuilderBase, NULL);

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
