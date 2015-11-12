
#ifndef __ELASTOS_DROID_VIEW_VIEWGROUPLAYOUTPARAMS_H__
#define __ELASTOS_DROID_VIEW_VIEWGROUPLAYOUTPARAMS_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::Animation::IAnimationParameters;
using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace View {

class ViewGroupLayoutParams
    : public Object
    , public IViewGroupLayoutParams
{
public:
    CAR_INTERFACE_DECL();

    /**
     * Used internally by MarginLayoutParams.
     * @hide
     */
    ViewGroupLayoutParams();

    virtual ~ViewGroupLayoutParams();

    CARAPI constructor(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* source);

    virtual CARAPI SetWidth(
        /* [in] */ Int32 width);

    virtual CARAPI SetHeight(
        /* [in] */ Int32 height);

    virtual CARAPI GetWidth(
        /* [out] */ Int32* width);

    virtual CARAPI GetHeight(
        /* [out] */ Int32* height);

    virtual CARAPI SetLayoutAnimationParameters(
        /* [in] */ IAnimationParameters* ap);

    virtual CARAPI GetLayoutAnimationParameters(
        /* [out] */ IAnimationParameters** ap);

    virtual CARAPI ResolveLayoutDirection(
        /* [in] */ Int32 layoutDirection);

    virtual CARAPI OnDebugDraw(
        /* [in] */ IView* view,
        /* [in] */ ICanvas* canvas,
        /* [in] */ IPaint* paint);

protected:
    /**
     * Extracts the layout parameters from the supplied attributes.
     *
     * @param a the style attributes to extract the parameters from
     * @param widthAttr the identifier of the width attribute
     * @param heightAttr the identifier of the height attribute
     */
    virtual CARAPI SetBaseAttributes(
        /* [in] */ ITypedArray* a,
        /* [in] */ Int32 widthAttr,
        /* [in] */ Int32 heightAttr);

    /**
     * Converts the specified size to a readable String.
     *
     * @param size the size to convert
     * @return a String instance representing the supplied size
     *
     * @hide
     */
    static CARAPI SizeToString(
        /* [in] */ Int32 size,
        /* [out] */ String* des);

public:
    /**
     * Information about how wide the view wants to be. Can be one of the
     * constants FILL_PARENT (replaced by MATCH_PARENT ,
     * in API Level 8) or WRAP_CONTENT. or an exact size.
     */
    Int32 mWidth;

    /**
     * Information about how tall the view wants to be. Can be one of the
     * constants FILL_PARENT (replaced by MATCH_PARENT ,
     * in API Level 8) or WRAP_CONTENT. or an exact size.
     */
    Int32 mHeight;

    /**
     * Used to animate layouts.
     */
    AutoPtr<IAnimationParameters> mLayoutAnimationParameters;
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_VIEWGROUPLAYOUTPARAMS_H__
