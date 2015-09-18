#ifndef __ELASTOS_DROID_GRAPHICS_COLORFILTER_H__
#define __ELASTOS_DROID_GRAPHICS_COLORFILTER_H__

#include "ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace Graphics {

extern const InterfaceID EIID_ColorFilter;

class ColorFilter
    : public Object
    , public IColorFilter
{
    friend class Paint;

public:
    CAR_INTERFACE_DECL();

    virtual ~ColorFilter();

private:
    static CARAPI_(void) Finalizer(
        /* [in] */ Int32 nativeInstance,
        /* [in] */ Int32 nativeColorFilter);

public:
    Int32 mNativeInstance;
    /**
     * @hide
     */
    Int32 mNativeColorFilter;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_COLORFILTER_H__
