
#ifndef __ELASTOS_DROID_GRAPHICS_PATHEFFECT_H__
#define __ELASTOS_DROID_GRAPHICS_PATHEFFECT_H__

#include <elastos.h>

using namespace Elastos;

namespace Elastos {
namespace Droid {
namespace Graphics {

extern const InterfaceID EIID_PathEffect;

/**
 * PathEffect is the base class for objects in the Paint that affect
 * the geometry of a drawing primitive before it is transformed by the
 * canvas' matrix and drawn.
 */
class PathEffect
    : public Object
    , public IPathEffect
{
    friend class Paint;

public:
    CAR_INTERFACE_DECL();

    virtual ~PathEffect();

private:
    static CARAPI_(void) NativeDestructor(
        /* [in] */ Int32 nativeInstance);

public:
    Int32 mNativeInstance;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_PATHEFFECT_H__
