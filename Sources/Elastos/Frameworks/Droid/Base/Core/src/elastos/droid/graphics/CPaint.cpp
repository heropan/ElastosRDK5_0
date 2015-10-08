
#include "graphics/CPaint.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CAR_OBJECT_IMPL(CPaint);
PInterface CPaint::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Paint) {
        return reinterpret_cast<PInterface>((Paint*)this);
    }
    if (riid == EIID_IPaint) {
        return (IPaint*)this;
    }
    return Paint::Probe(riid);
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
