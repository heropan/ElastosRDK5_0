
#ifndef __MIPMAPDRAWABLE_H__
#define __MIPMAPDRAWABLE_H__

#include "graphics/drawable/DrawableContainer.h"
#include "utils/StateSet.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

/**
 * @hide -- we are probably moving to do MipMaps in another way (more integrated
 * with the resource system).
 *
 * A resource that manages a number of alternate Drawables, and which actually draws the one which
 * size matches the most closely the drawing bounds. Providing several pre-scaled version of the
 * drawable helps minimizing the aliasing artifacts that can be introduced by the scaling.
 *
 * <p>
 * Use {@link #addDrawable(Drawable)} to define the different Drawables that will represent the
 * mipmap levels of this MipmapDrawable. The mipmap Drawable that will actually be used when this
 * MipmapDrawable is drawn is the one which has the smallest intrinsic height greater or equal than
 * the bounds' height. This selection ensures that the best available mipmap level is scaled down to
 * draw this MipmapDrawable.
 * </p>
 *
 * If the bounds' height is larger than the largest mipmap, the largest mipmap will be scaled up.
 * Note that Drawables without intrinsic height (i.e. with a negative value, such as Color) will
 * only be used if no other mipmap Drawable are provided. The Drawables' intrinsic heights should
 * not be changed after the Drawable has been added to this MipmapDrawable.
 *
 * <p>
 * The different mipmaps' parameters (opacity, padding, color filter, gravity...) should typically
 * be similar to ensure a continuous visual appearance when the MipmapDrawable is scaled. The aspect
 * ratio of the different mipmaps should especially be equal.
 * </p>
 *
 * A typical example use of a MipmapDrawable would be for an image which is intended to be scaled at
 * various sizes, and for which one wants to provide pre-scaled versions to precisely control its
 * appearance.
 *
 * <p>
 * The intrinsic size of a MipmapDrawable are inferred from those of the largest mipmap (in terms of
 * {@link Drawable#getIntrinsicHeight()}). On the opposite, its minimum
 * size is defined by the smallest provided mipmap.
 * </p>

 * It can be defined in an XML file with the <code>&lt;mipmap></code> element.
 * Each mipmap Drawable is defined in a nested <code>&lt;item></code>. For example:
 * <pre>
 * &lt;mipmap xmlns:android="http://schemas.android.com/apk/res/android">
 *  &lt;item android:drawable="@drawable/my_image_8" />
 *  &lt;item android:drawable="@drawable/my_image_32" />
 *  &lt;item android:drawable="@drawable/my_image_128" />
 * &lt;/mipmap>
 *</pre>
 * <p>
 * With this XML saved into the res/drawable/ folder of the project, it can be referenced as
 * the drawable for an {@link android.widget.ImageView}. Assuming that the heights of the provided
 * drawables are respectively 8, 32 and 128 pixels, the first one will be scaled down when the
 * bounds' height is lower or equal than 8 pixels. The second drawable will then be used up to a
 * height of 32 pixels and the largest drawable will be used for greater heights.
 * </p>
 * @attr ref android.R.styleable#MipmapDrawableItem_drawable
 */

class MipmapDrawable : public DrawableContainer
{
private:
    class MipmapContainerState:  public DrawableContainer::DrawableContainerState
    {
    public:
        MipmapContainerState(
            /* [in] */ MipmapContainerState* orig,
            /* [in] */ MipmapDrawable* owner,
            /* [in] */ IResources* res);

        /**
         * Returns the index of the child mipmap drawable that will best fit the provided bounds.
         * This index is determined by comparing bounds' height and children intrinsic heights.
         * The returned mipmap index is the smallest mipmap which height is greater or equal than
         * the bounds' height. If the bounds' height is larger than the largest mipmap, the largest
         * mipmap index is returned.
         *
         * @param bounds The bounds of the MipMapDrawable.
         * @return The index of the child Drawable that will best fit these bounds, or -1 if there
         * are no children mipmaps.
         */
        CARAPI_(Int32) IndexForBounds(
            /* [in] */ IRect* bounds);

        /**
         * Adds a Drawable to the list of available mipmap Drawables. This list can be retrieved
         * using {@link DrawableContainer.DrawableContainerState#getChildren()} and this method
         * ensures that it is always sorted by increasing {@link Drawable#getIntrinsicHeight()}.
         *
         * @param drawable The Drawable that will be added to children list
         */
        CARAPI_(void) AddDrawable(
            /* [in] */ IDrawable* drawable);

        /**
         * Intrinsic sizes are those of the largest available mipmap.
         * Minimum sizes are those of the smallest available mipmap.
         */
        //@Override
        CARAPI_(void) ComputeConstantSize();

        //@Override
        CARAPI NewDrawable(
            /* [out] */ IDrawable** drawable);

        //@Override
        CARAPI NewDrawableEx(
            /* [in] */ IResources* res,
            /* [out] */ IDrawable** drawable);

        //@Override
        CARAPI_(void) GrowArray(
            /* [in] */ Int32 oldSize,
            /* [in] */ Int32 newSize);

    private:
        AutoPtr<ArrayOf<Int32> > mMipmapHeights;
    };

public:
    MipmapDrawable();

    /**
     * Adds a Drawable to the list of available mipmap Drawables. The Drawable actually used when
     * this MipmapDrawable is drawn is determined from its bounds.
     *
     * This method has no effect if drawable is null.
     *
     * @param drawable The Drawable that will be added to list of available mipmap Drawables.
     */

    CARAPI AddDrawable(
        /* [in] */ IDrawable* drawable);

    // overrides from Drawable
    //@Override
    virtual CARAPI Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

    //@Override
    virtual CARAPI_(AutoPtr<IDrawable>) Mutate();

protected:
    CARAPI_(void) OnBoundsChange(
        /* [in] */ IRect* bounds);

private:
    CARAPI_(void) OnDrawableAdded();

    CARAPI Init(
        /* [in] */ MipmapContainerState* state,
        /* [in] */ IResources* res);

private:
    AutoPtr<MipmapContainerState> mMipmapContainerState;
    Boolean mMutated;
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__MIPMAPDRAWABLE_H__

