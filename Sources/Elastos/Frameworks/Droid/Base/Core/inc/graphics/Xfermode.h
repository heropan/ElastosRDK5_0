
#ifndef __ELASTOS_DROID_GRAPHICS_XFERMODE_H__
#define __ELASTOS_DROID_GRAPHICS_XFERMODE_H__

#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

extern const InterfaceID EIID_Xfermode;

/**
 * Xfermode is the base class for objects that are called to implement custom
 * "transfer-modes" in the drawing pipeline. The static function Create(Modes)
 * can be called to return an instance of any of the predefined subclasses as
 * specified in the Modes enum. When an Xfermode is assigned to an Paint, then
 * objects drawn with that paint have the xfermode applied.
 */
class Xfermode
{
    friend class Paint;

public:
    virtual ~Xfermode();

private:
    static CARAPI_(void) Finalizer(
        /* [in] */ Int32 nativeInstance);

public:
    Int32 mNativeInstance;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_XFERMODE_H__
