#ifndef __MASKFILTER_H__
#define __MASKFILTER_H__

#include <elastos.h>

using namespace Elastos;

namespace Elastos {
namespace Droid {
namespace Graphics {

extern const InterfaceID EIID_MaskFilter;

/**
 * MaskFilter is the base class for object that perform transformations on
 * an alpha-channel mask before drawing it. A subclass of MaskFilter may be
 * installed into a Paint. Blur and emboss are implemented as subclasses of MaskFilter.
 */
class MaskFilter
{
    friend class Paint;

public:
    virtual ~MaskFilter();

private:
    static CARAPI_(void) NativeDestructor(
        /* [in] */ Int32 nativeInstance);

protected:
    Int32 mNativeInstance;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif //__MASKFILTER_H__
