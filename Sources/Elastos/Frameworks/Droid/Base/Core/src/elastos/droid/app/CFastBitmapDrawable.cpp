#include "elastos/droid/app/CFastBitmapDrawable.h"

namespace Elastos {
namespace Droid {
namespace App {

IDRAWABLE_METHODS_IMPL(CFastBitmapDrawable, FastBitmapDrawable)

CFastBitmapDrawable::constructor(
    /* [in] */ IBitmap* bitmap)
{
    return FastBitmapDrawable::Init(bitmap);
}

PInterface CFastBitmapDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CFastBitmapDrawable::Probe(riid);
}

} // namespace App
} // namespace Droid
} // namespace Elastos
