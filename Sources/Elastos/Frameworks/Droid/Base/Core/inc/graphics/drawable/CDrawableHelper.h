#ifndef __ELASTOS_DROID_GRAPHICS_DRAWABLE_CDRAWABLEHELPER_H__
#define __ELASTOS_DROID_GRAPHICS_DRAWABLE_CDRAWABLEHELPER_H__

#include "_Elastos_Droid_Graphics_Drawable_CDrawableHelper.h"
#include "graphics/drawable/Drawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CarClass(CDrawableHelper)
{
public:
    CARAPI CreateFromPath(
        /* [in] */ const String& pathName,
        /* [out] */ IDrawable** drawable);

    CARAPI CreateFromResourceStream(
        /* [in] */ IResources* res,
        /* [in] */ ITypedValue* value,
        /* [in] */ IInputStream* is,
        /* [in] */ const String& srcName,
        /* [in] */ IBitmapFactoryOptions* opts,
        /* [out] */ IDrawable** draw);

    CARAPI CreateFromResourceStream(
        /* [in] */ IResources* res,
        /* [in] */ ITypedValue* value,
        /* [in] */ IInputStream* is,
        /* [in] */ const String& srcName,
        /* [out] */ IDrawable** draw);

    CARAPI CreateFromStream(
        /* [in] */ IInputStream* is,
        /* [in] */ const String& srcName,
        /* [out] */ IDrawable** draw);

    CARAPI CreateFromXml(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [out] */ IDrawable** draw);

    CARAPI CreateFromXmlInner(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IDrawable** draw);

};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_DRAWABLE_CDRAWABLEHELPER_H__
