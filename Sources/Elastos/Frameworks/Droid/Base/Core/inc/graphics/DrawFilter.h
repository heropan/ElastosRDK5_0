
#ifndef __DRAWFILTER_H__
#define __DRAWFILTER_H__

#include "Elastos.Droid.Core_server.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

extern const InterfaceID EIID_DrawFilter;

/**
 * A DrawFilter subclass can be installed in a Canvas. When it is present, it
 * can modify the paint that is used to draw (temporarily). With this, a filter
 * can disable/enable antialiasing, or change the color for everything this is
 * drawn.
 */
class DrawFilter
{
public:
    virtual ~DrawFilter();

private:
    static CARAPI_(void) NativeDestructor(
        /* [in] */ Int32 nativeDrawFilter);

public:
    Int32 mNativeInstance;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __DRAWFILTER_H__
