#ifndef __CDRAWABLEHELPER__H_
#define __CDRAWABLEHELPER__H_

#include "_CDrawableHelper.h"
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

    CARAPI CreateFromResourceStreamEx(
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

#endif //__CDRAWABLEHELPER__H_