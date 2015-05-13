#ifndef __COLORFILTER_H__
#define __COLORFILTER_H__

#include <elastos.h>

using namespace Elastos;

namespace Elastos {
namespace Droid {
namespace Graphics {

extern const InterfaceID EIID_ColorFilter;

class ColorFilter
{
    friend class Paint;

public:
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

#endif //__COLORFILTER_H__
