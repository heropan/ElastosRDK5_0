
#include "elastos/droid/internal/view/menu/CMenuBuilder.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {
namespace Menu {

ECode CMenuBuilder::constructor(
    /* [in] */ IContext* context)
{
    MenuBuilder::Init(context);
    return NOERROR;
}

PInterface CMenuBuilder::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_MenuBuilderBase == riid) {
        return reinterpret_cast<PInterface>((MenuBuilder *)this);
    }

    return _CMenuBuilder::Probe(riid);
}

IMENU_METHODS_IMPL(CMenuBuilder, MenuBuilder, NULL);
IMENUBUILDER_METHODS_IMPL(CMenuBuilder, MenuBuilder, NULL);

} // namespace Menu
} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos
