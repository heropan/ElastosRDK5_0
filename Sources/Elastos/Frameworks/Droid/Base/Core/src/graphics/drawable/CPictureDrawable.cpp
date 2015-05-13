
#include "ext/frameworkext.h"
#include "graphics/drawable/CPictureDrawable.h"


namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

/**
 * Construct a new drawable referencing the specified picture. The picture
 * may be null.
 *
 * @param picture The picture to associate with the drawable. May be null.
 */
ECode CPictureDrawable::constructor(
    /* [in] */ IPicture* picture)
{
    return PictureDrawable::Init(picture);
}

PInterface CPictureDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CPictureDrawable::Probe(riid);
}

IDRAWABLE_METHODS_IMPL(
    CPictureDrawable, PictureDrawable);

/**
 * Return the picture associated with the drawable. May be null.
 *
 * @return the picture associated with the drawable, or null.
 */
ECode CPictureDrawable::GetPicture(
    /* [out] */ IPicture** picture)
{
    VALIDATE_NOT_NULL(picture);
    AutoPtr<IPicture> p = PictureDrawable::GetPicture();
    *picture = p;
    INTERFACE_ADDREF(*picture);
    return NOERROR;
}

/**
 * Associate a picture with this drawable. The picture may be null.
 *
 * @param picture The picture to associate with the drawable. May be null.
 */
ECode CPictureDrawable::SetPicture(
    /* [in] */ IPicture* picture)
{
    return PictureDrawable::SetPicture(picture);
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
