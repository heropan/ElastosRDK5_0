
#include "ext/frameworkext.h"
#include "graphics/drawable/CNinePatchDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

IDRAWABLE_METHODS_IMPL(CNinePatchDrawable, NinePatchDrawable)

ECode CNinePatchDrawable::constructor()
{
    return NOERROR;
}

ECode CNinePatchDrawable::constructor(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ArrayOf<Byte>* chunk,
    /* [in] */ IRect* padding,
    /* [in] */ const String& srcName)
{
    return NinePatchDrawable::Init(bitmap, chunk, padding, srcName);
}

ECode CNinePatchDrawable::constructor(
    /* [in] */ IResources* res,
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ArrayOf<Byte>* chunk,
    /* [in] */ IRect* padding,
    /* [in] */ const String& srcName)
{
    return NinePatchDrawable::Init(res, bitmap, chunk, padding, srcName);
}

ECode CNinePatchDrawable::constructor(
    /* [in] */ IResources* res,
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ArrayOf<Byte>* chunk,
    /* [in] */ IRect* padding,
    /* [in] */ IRect* layoutInsets,
    /* [in] */ const String& srcName)
{
    return NinePatchDrawable::Init(res, bitmap, chunk, padding, layoutInsets, srcName);
}

ECode CNinePatchDrawable::constructor(
    /* [in] */ INinePatch* patch)
{
    return NinePatchDrawable::Init(patch);
}

ECode CNinePatchDrawable::constructor(
    /* [in] */ IResources* res,
    /* [in] */ INinePatch* patch)
{
    return NinePatchDrawable::Init(res, patch);
}

ECode CNinePatchDrawable::constructor(
    /* [in] */ Handle32 state,
    /* [in] */ IResources* res)
{
    return NinePatchDrawable::Init(
            reinterpret_cast<NinePatchDrawable::NinePatchState*>(state), res);
}

PInterface CNinePatchDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CNinePatchDrawable::Probe(riid);
}

ECode CNinePatchDrawable::SetTargetDensity(
    /* [in] */ ICanvas* canvas)
{
    return NinePatchDrawable::SetTargetDensity(canvas);
}

ECode CNinePatchDrawable::SetTargetDensity(
    /* [in] */ IDisplayMetrics* metrics)
{
    return NinePatchDrawable::SetTargetDensity(metrics);
}

ECode CNinePatchDrawable::SetTargetDensity(
    /* [in] */ Int32 density)
{
    return NinePatchDrawable::SetTargetDensity(density);
}

ECode CNinePatchDrawable::GetPaint(
    /* [out] */ IPaint** paint)
{
    VALIDATE_NOT_NULL(paint);

    AutoPtr<IPaint> temp = NinePatchDrawable::GetPaint();
    *paint = temp;
    REFCOUNT_ADD(*paint);
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
