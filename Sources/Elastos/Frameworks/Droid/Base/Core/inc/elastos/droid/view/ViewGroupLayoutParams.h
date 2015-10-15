
#ifndef __ELASTOS_DROID_VIEW_VIEWGROUPLAYOUTPARAMS_H__
#define __ELASTOS_DROID_VIEW_VIEWGROUPLAYOUTPARAMS_H__

#include "Elastos.Droid.Core_server.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::Animation::IAnimationParameters;

namespace Elastos {
namespace Droid {
namespace View {

class ViewGroupLayoutParams
{
public:
    /**
     * Creates a new set of layout parameters. The values are extracted from
     * the supplied attributes set and context. The XML attributes mapped
     * to this set of layout parameters are:
     *
     * <ul>
     *   <li><code>layout_width</code>: the width, either an exact value,
     *   {@link #WRAP_CONTENT}, or {@link #FILL_PARENT} (replaced by
     *   {@link #MATCH_PARENT} in API Level 8)</li>
     *   <li><code>layout_height</code>: the height, either an exact value,
     *   {@link #WRAP_CONTENT}, or {@link #FILL_PARENT} (replaced by
     *   {@link #MATCH_PARENT} in API Level 8)</li>
     * </ul>
     *
     * @param c the application environment
     * @param attrs the set of attributes from which to extract the layout
     *              parameters' values
     */
    ViewGroupLayoutParams(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    /**
     * Creates a new set of layout parameters with the specified width
     * and height.
     *
     * @param width the width, either {@link #WRAP_CONTENT},
     *        {@link #FILL_PARENT} (replaced by {@link #MATCH_PARENT} in
     *        API Level 8), or a fixed size in pixels
     * @param height the height, either {@link #WRAP_CONTENT},
     *        {@link #FILL_PARENT} (replaced by {@link #MATCH_PARENT} in
     *        API Level 8), or a fixed size in pixels
     */
    ViewGroupLayoutParams(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    /**
     * Copy constructor. Clones the width and height values of the source.
     *
     * @param source The layout params to copy from.
     */
    ViewGroupLayoutParams(
        /* [in] */ ViewGroupLayoutParams* source);

    /**
     * Used internally by MarginLayoutParams.
     * @hide
     */
    ViewGroupLayoutParams();

    virtual ~ViewGroupLayoutParams();

    CARAPI Init(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI Init(
        /* [in] */ IViewGroupLayoutParams* source);

    virtual CARAPI SetWidth(
        /* [in] */ Int32 width);

    virtual CARAPI SetHeight(
        /* [in] */ Int32 height);

    virtual CARAPI_(Int32) GetWidth();

    virtual CARAPI_(Int32) GetHeight();

    virtual CARAPI SetLayoutAnimationParameters(
        /* [in] */ IAnimationParameters* ap);

    virtual CARAPI GetLayoutAnimationParameters(
        /* [out] */ IAnimationParameters** ap);

    virtual CARAPI_(AutoPtr<IAnimationParameters>) GetLayoutAnimationParameters();

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
