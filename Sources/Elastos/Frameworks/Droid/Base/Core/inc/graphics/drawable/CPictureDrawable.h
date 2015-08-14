
#ifndef __ELASTOS_DROID_GRAPHICS_DRAWABLE_CPICTUREDRAWABLE_H__
#define __ELASTOS_DROID_GRAPHICS_DRAWABLE_CPICTUREDRAWABLE_H__

#include "_CPictureDrawable.h"
#include "graphics/drawable/PictureDrawable.h"


namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CarClass(CPictureDrawable), PictureDrawable
{
public:
    IDRAWABLE_METHODS_DECL()

    /**
     * Return the picture associated with the drawable. May be null.
     *
     * @return the picture associated with the drawable, or null.
     */
    CARAPI GetPicture(
        /* [out] */ IPicture** picture);

    /**
     * Associate a picture with this drawable. The picture may be null.
     *
     * @param picture The picture to associate with the drawable. May be null.
     */
    CARAPI SetPicture(
        /* [in] */ IPicture* picture);

    /**
     * Construct a new drawable referencing the specified picture. The picture
     * may be null.
     *
     * @param picture The picture to associate with the drawable. May be null.
     */
    CARAPI constructor(
        /* [in] */ IPicture* picture);

    PInterface Probe(
        /* [in] */ REIID riid);
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_DRAWABLE_CPICTUREDRAWABLE_H__
