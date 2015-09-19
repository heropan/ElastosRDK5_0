
#ifndef __ELASTOS_DROID_GRAPHICS_INSETS_H__
#define __ELASTOS_DROID_GRAPHICS_INSETS_H__

#include "Elastos.Droid.Core_server.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

class Insets
    : public ElRefBase
    , public IInsets
{
public:
    CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

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
    static CARAPI_(AutoPtr<Insets>) Of(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    /**
     * Return an Insets instance with the appropriate values.
     *
     * @param r the rectangle from which to take the values
     *
     * @return an Insets instance with the appropriate values
     */
    static CARAPI_(AutoPtr<Insets>) Of(
        /* [in] */ IRect* r);

    CARAPI_(Boolean) Equals(
        /* [in] */ IInsets* o);

    CARAPI GetLeft(
        /* [out] */ Int32* left);

    CARAPI GetTop(
        /* [out] */ Int32* top);

    CARAPI GetRight(
        /* [out] */ Int32* right);

    CARAPI GetBottom(
        /* [out] */ Int32* bottom);

private:
    Insets(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

public:
    static const AutoPtr<Insets> NONE;

    const Int32 mLeft;
    const Int32 mTop;
    const Int32 mRight;
    const Int32 mBottom;
};

} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_INSETS_H__
