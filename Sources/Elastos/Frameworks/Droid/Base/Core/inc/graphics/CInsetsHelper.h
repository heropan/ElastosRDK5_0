
#ifndef __CINSETSHELPER_H__
#define __CINSETSHELPER_H__

#include "_CInsetsHelper.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CInsetsHelper)
{
public:
    /**
     * Return an Insets instance with the appropriate values.
     *
     * @param left the left inset
     * @param top the top inset
     * @param right the right inset
     * @param bottom the bottom inset
     *
     * @return Insets instance with the appropriate values
     */
    CARAPI Of(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom,
        /* [out] */ IInsets** insets);

    /**
     * Return an Insets instance with the appropriate values.
     *
     * @param r the rectangle from which to take the values
     *
     * @return an Insets instance with the appropriate values
     */
    CARAPI Of(
        /* [in] */ IRect* r,
        /* [out] */ IInsets** insets);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif //__CINSETSHELPER_H__
