
#ifndef __ELASTOS_DROID_GRAPHICS_DRAWABLE_LAYERDRAWABLE_H__
#define __ELASTOS_DROID_GRAPHICS_DRAWABLE_LAYERDRAWABLE_H__

#include "graphics/drawable/Drawable.h"


namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

class LayerDrawable : public Drawable
{
public:
    class ChildDrawable : public ElRefBase
    {
    public:
        ChildDrawable()
            : mInsetL(0)
            , mInsetT(0)
            , mInsetR(0)
            , mInsetB(0)
            , mId(0)
        {}

        AutoPtr<IDrawable> mDrawable;
        Int32 mInsetL;
        Int32 mInsetT;
        Int32 mInsetR;
        Int32 mInsetB;
        Int32 mId;
    };

    class LayerState
        : public Drawable::ConstantState
    {
    public:
        LayerState(
            /* [in] */ LayerState* orig,
            /* [in] */ LayerDrawable* owner,
            /* [in] */ IResources* res);

        virtual ~LayerState();

        CARAPI NewDrawable(
            /* [out] */ IDrawable** drawable);

        CARAPI NewDrawable(
            /* [in] */ IResources* res,
            /* [out] */ IDrawable** drawable);

        CARAPI GetChangingConfigurations(
            /* [out] */ Int32* config);

        CARAPI_(Int32) GetOpacity();

        CARAPI_(Boolean) IsStateful();

        virtual CARAPI_(Boolean) CanConstantState();

    public:
        Int32 mNum;
        AutoPtr< ArrayOf<ChildDrawable*> > mChildren;

        Int32 mChangingConfigurations;
        Int32 mChildrenChangingConfigurations;

    private:
        Boolean mHaveOpacity;
        Int32 mOpacity;

        Boolean mHaveStateful;
        Boolean mStateful;

        Boolean mCheckedConstantState;
        Boolean mCanConstantState;
    };

public:
    LayerDrawable();

    /**
     * Create a new layer drawable with the specified list of layers and the specified
     * constant state.
     *
     * @param layers The list of layers to add to this drawable.
     * @param state The constant drawable state.
     */
    LayerDrawable(
        /* [in] */ ArrayOf<IDrawable*>* layers,
        /* [in] */ LayerState* state = NULL);

    LayerDrawable(
        /* [in] */ LayerState* state,
        /* [in] */ IResources* res);

    virtual ~LayerDrawable();

    CARAPI Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

    /**
     * Look for a layer with the given id, and returns its {@link Drawable}.
     *
     * @param id The layer ID to search for->
     * @return The {@link Drawable} of the layer that has the given id in the hierarchy or NULL.
     */
    virtual CARAPI_(AutoPtr<IDrawable>) FindDrawableByLayerId(
        /* [in] */ Int32 id);

    /**
     * Sets the ID of a layer->
     *
     * @param index The index of the layer which will received the ID.
     * @param id The ID to assign to the layer->
     */
    virtual CARAPI SetId(
        /* [in] */ Int32 index,
        /* [in] */ Int32 id);

    /**
     * Returns the number of layers contained within this.
     * @return The number of layers.
     */
    virtual CARAPI_(Int32) GetNumberOfLayers();

    /**
     * Returns the drawable at the specified layer index.
     *
     * @param index The layer index of the drawable to retrieve.
     *
     * @return The {@link android.graphics.drawable.Drawable} at the specified layer index.
     */
    virtual CARAPI_(AutoPtr<IDrawable>) GetDrawable(
        /* [in] */ Int32 index);

    /**
     * Returns the id of the specified layer.
     *
     * @param index The index of the layer.
     *
     * @return The id of the layer or {@link android.view.View#NO_ID} if the layer has no id.
     */
    virtual CARAPI_(Int32) GetId(
        /* [in] */ Int32 index);

    /**
     * Sets (or replaces) the {@link Drawable} for the layer with the given id.
     *
     * @param id The layer ID to search for->
     * @param drawable The replacement {@link Drawable}.
     * @return Whether the {@link Drawable} was replaced (could return FALSE if
     *         the id was not found).
     */
    virtual CARAPI_(Boolean) SetDrawableByLayerId(
        /* [in] */ Int32 id,
        /* [in] */ IDrawable* drawable);

    /** Specify modifiers to the bounds for the drawable[index]->
        left += l
        top += t;
        right -= r;
        bottom -= b;
    */
    virtual CARAPI SetLayerInset(
        /* [in] */ Int32 index,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    //@Override
    CARAPI InvalidateDrawable(
        /* [in] */ IDrawable* who);

    //@Override
    CARAPI ScheduleDrawable(
        /* [in] */ IDrawable* who,
        /* [in] */ IRunnable* what,
        /* [in] */ Int64 when);

    //@Override
    CARAPI UnscheduleDrawable(
        /* [in] */ IDrawable* who,
        /* [in] */ IRunnable* what);

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    //@Override
    CARAPI_(Int32) GetChangingConfigurations();

    //@Override
    CARAPI_(Boolean) GetPadding(
        /* [in] */ IRect* padding);

    //@Override
    CARAPI_(Boolean) SetVisible(
        /* [in] */ Boolean visible,
        /* [in] */ Boolean restart);

    //@Override
    CARAPI SetDither(
        /* [in] */ Boolean dither);

    //@Override
    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    //@Override
    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

    /**
     * Sets the opacity of this drawable directly, instead of collecting the states from
     * the layers
     *
     * @param opacity The opacity to use, or {@link PixelFormat#UNKNOWN PixelFormat.UNKNOWN}
     * for the default behavior
     *
     * @see PixelFormat#UNKNOWN
     * @see PixelFormat#TRANSLUCENT
     * @see PixelFormat#TRANSPARENT
     * @see PixelFormat#OPAQUE
     */
    virtual CARAPI SetOpacity(
        /* [in] */ Int32 opacity);

    //@Override
    CARAPI_(Int32) GetOpacity();

    //@Override
    CARAPI_(Boolean) IsStateful();

    //@Override
    CARAPI_(Int32) GetIntrinsicWidth();

    //@Override
    CARAPI_(Int32) GetIntrinsicHeight();

    //@Override
    CARAPI_(AutoPtr<IDrawableConstantState>) GetConstantState();

    //@Override
    CARAPI_(AutoPtr<IDrawable>) Mutate();

    //@Override
    CARAPI SetLayoutDirection(
        /* [in] */ Int32 layoutDirection);

protected:
    CARAPI_(AutoPtr<LayerState>) CreateConstantState(
        /* [in] */ LayerState* state,
        /* [in] */ IResources* res);

    //@Override
    CARAPI_(Boolean) OnStateChange(
        /* [in] */ const ArrayOf<Int32>* state);

    //@Override
    CARAPI_(Boolean) OnLevelChange(
        /* [in] */ Int32 level);

    //@Override
    CARAPI_(void) OnBoundsChange(
        /* [in] */ IRect* bounds);

    CARAPI Init(
        /* [in] */ ArrayOf<IDrawable*>* layers,
        /* [in] */ LayerState* state = NULL);

    CARAPI Init(
        /* [in] */ LayerState* state,
        /* [in] */ IResources* res);

private:
    CARAPI AddLayer(
        /* [in] */ IDrawable* layer,
        /* [in] */ Int32 id,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI_(Boolean) ReapplyPadding(
        /* [in] */ Int32 i,
        /* [in] */ ChildDrawable* r);

    CARAPI_(void) EnsurePadding();

public:
    AutoPtr<LayerState> mLayerState;

private:
    Int32 mOpacityOverride;
    AutoPtr< ArrayOf<Int32> > mPaddingL;
    AutoPtr< ArrayOf<Int32> > mPaddingT;
    AutoPtr< ArrayOf<Int32> > mPaddingR;
    AutoPtr< ArrayOf<Int32> > mPaddingB;

    AutoPtr<IRect> mTmpRect;
    Boolean mMutated;
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_DRAWABLE_LAYERDRAWABLE_H__
