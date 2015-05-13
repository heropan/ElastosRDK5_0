#include "ext/frameworkext.h"
#include "graphics/drawable/CDrawableHelper.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

ECode CDrawableHelper::CreateFromPath(
    /* [in] */ const String& pathName,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return Drawable::CreateFromPath(pathName, drawable);
}

ECode CDrawableHelper::CreateFromResourceStream(
    /* [in] */ IResources* res,
    /* [in] */ ITypedValue* value,
    /* [in] */ IInputStream* is,
    /* [in] */ const String& srcName,
    /* [in] */ IBitmapFactoryOptions* opts,
    /* [out] */ IDrawable** draw)
{
    VALIDATE_NOT_NULL(draw);
    return Drawable::CreateFromResourceStream(res, value, is, srcName, opts, draw);
}

ECode CDrawableHelper::CreateFromResourceStreamEx(
    /* [in] */ IResources* res,
    /* [in] */ ITypedValue* value,
    /* [in] */ IInputStream* is,
    /* [in] */ const String& srcName,
    /* [out] */ IDrawable** draw)
{
    VALIDATE_NOT_NULL(draw);
    return Drawable::CreateFromResourceStream(res, value, is, srcName, draw);
}

ECode CDrawableHelper::CreateFromStream(
    /* [in] */ IInputStream* is,
    /* [in] */ const String& srcName,
    /* [out] */ IDrawable** draw)
{
    VALIDATE_NOT_NULL(draw);
    return Drawable::CreateFromStream(is, srcName, draw);
}

ECode CDrawableHelper::CreateFromXml(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [out] */ IDrawable** draw)
{
    VALIDATE_NOT_NULL(draw);
    return Drawable::CreateFromXml(r, parser, draw);
}

ECode CDrawableHelper::CreateFromXmlInner(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IDrawable** draw)
{
    VALIDATE_NOT_NULL(draw);
    return Drawable::CreateFromXmlInner(r, parser, attrs, draw);
}


} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos