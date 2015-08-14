
#ifndef __ELASTOS_DROID_GRAPHICS_DRAWABLE_DRAWABLECONTAINER_H__
#define __ELASTOS_DROID_GRAPHICS_DRAWABLE_DRAWABLECONTAINER_H__

#include "graphics/drawable/Drawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

class DrawableContainer : public Drawable
{
public:
    /**
     * A ConstantState that can contain several {@link Drawable}s.
     *
     * This class was made public to enable testing, and its visibility may change in a future
     * release.
     */
    class DrawableContainerState
        : public Drawable::ConstantState
    {
    public:
        DrawableContainerState(
            /* [in] */ DrawableContainerState* orig,
            /* [in] */ DrawableContainer* owner,
            /* [in] */ IResources* res);

        virtual ~DrawableContainerState();

        CARAPI GetChangingConfigurations(
            /* [out] */ Int32* configurations);

        CARAPI_(Int32) AddChild(
            /* [in] */ IDrawable* dr);

        CARAPI_(Int32) GetChildCount();

        CARAPI_(AutoPtr< ArrayOf<IDrawable*> >) GetChildren();

        /** A Boolean value indicating whether to use the maximum padding value of
         * all frames in the set (FALSE), or to use the padding value of the frame
         * being shown (TRUE). Default value is FALSE.
         */
        CARAPI_(void) SetVariablePadding(
            /* [in] */ Boolean variable);

        CARAPI_(AutoPtr<IRect>) GetConstantPadding();

        CARAPI_(void) SetConstantSize(
            /* [in] */ Boolean constant);

        CARAPI_(Boolean) IsConstantSize();

        CARAPI_(Int32) GetConstantWidth();

        CARAPI_(Int32) GetConstantHeight();

        CARAPI_(Int32) GetConstantMinimumWidth();

        CARAPI_(Int32) GetConstantMinimumHeight();

        CARAPI_(void) SetEnterFadeDuration(
            /* [in] */ Int32 duration);

        CARAPI_(Int32) GetEnterFadeDuration();

        CARAPI_(void) SetExitFadeDuration(
            /* [in] */ Int32 duration);

        CARAPI_(Int32) GetExitFadeDuration();

        CARAPI_(Int32) GetOpacity();

        CARAPI_(Boolean) IsStateful();

        virtual CARAPI_(void) GrowArray(
            /* [in] */ Int32 oldSize,
            /* [in] */ Int32 newSize);

        //synchronized
        virtual CARAPI_(Boolean) CanConstantState();

    protected:
        CARAPI_(void) ComputeConstantSize();

    public:
        DrawableContainer* mOwner;

        Int32 mChangingConfigurations;
        Int32 mChildrenChangingConfigurations;

        AutoPtr< ArrayOf<IDrawable*> > mDrawables;
        Int32 mNumChildren;

        Boolean mVariablePadding;
        AutoPtr<IRect> mConstantPadding;

        Boolean mConstantSize;
        Boolean mComputedConstantSize;
        Int32 mConstantWidth;
        Int32 mConstantHeight;
        Int32 mConstantMinimumWidth;
        Int32 mConstantMinimumHeight;

        Int32 mOpacity;

        Boolean mHaveStateful;
        Boolean mStateful;

        Boolean mCheckedConstantState;
        Boolean mCanConstantState;

        Boolean mPaddingChecked;

        Boolean mDither;

        Int32         mEnterFadeDuration;
        Int32         mExitFadeDuration;

        Object mLock;
    };

    class _Runnable
        : public ElRefBase
        , public IRunnable
    {
    public:
        _Runnable(
            /* [in] */ DrawableContainer* host);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI Run();

    private:
        DrawableContainer*  mHost;
    };

public:
    DrawableContainer();

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    //@Override
    CARAPI_(Int32) GetChangingConfigurations();

    //@Override
    CARAPI_(Boolean) GetPadding(
        /* [in] */ IRect* padding);

    /**
     * @hide
     */
    //@Override
    CARAPI_(AutoPtr<IInsets>) GetLayoutInsets();

    //@Override
    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    //@Override
    virtual CARAPI SetDither(
        /* [in] */ Boolean dither);

    //@Override
    virtual CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

    /**
     * Change the global fade duration when a new drawable is entering
     * the scene.
     * @param ms The amount of time to fade in milliseconds.
     */
    virtual CARAPI SetEnterFadeDuration(
        /* [in] */ Int32 ms);

    /**
     * Change the global fade duration when a new drawable is leaving
     * the scene.
     * @param ms The amount of time to fade in milliseconds.
     */
    virtual CARAPI SetExitFadeDuration(
        /* [in] */ Int32 ms);

    //@Override
    CARAPI_(Boolean) IsStateful();

    //@Override
    CARAPI JumpToCurrentState();

    //@Override
    CARAPI_(Int32) GetIntrinsicWidth();

    //@Override
    CARAPI_(Int32) GetIntrinsicHeight();

    //@Override
    CARAPI_(Int32) GetMinimumWidth();

    //@Override
    CARAPI_(Int32) GetMinimumHeight();

    virtual CARAPI InvalidateDrawable(
        /* [in] */ IDrawable* who);

    virtual CARAPI ScheduleDrawable(
        /* [in] */ IDrawable* who,
        /* [in] */ IRunnable* what,
        /* [in] */ Int64 when);

    virtual CARAPI UnscheduleDrawable(
        /* [in] */ IDrawable* who,
        /* [in] */ IRunnable* what);

    //@Override
    CARAPI_(Boolean) SetVisible(
        /* [in] */ Boolean visible,
        /* [in] */ Boolean restart);

    //@Override
    CARAPI_(Int32) GetOpacity();

    virtual CARAPI_(Boolean) SelectDrawable(
        /* [in] */ Int32 idx);

    CARAPI_(void) Animate(
        /* [in] */ Boolean schedule);

    //@Override
    CARAPI_(AutoPtr<IDrawable>) GetCurrent();

    //@Override
    CARAPI_(AutoPtr<IDrawableConstantState>) GetConstantState();

    //@Override
    CARAPI_(AutoPtr<IDrawable>) Mutate();

protected:
    //@Override
    CARAPI_(void) OnBoundsChange(
        /* [in] */ IRect* bounds);

    //@Override
    CARAPI_(Boolean) OnStateChange(
        /* [in] */ const ArrayOf<Int32>* state);

    //@Override
    CARAPI_(Boolean) OnLevelChange(
        /* [in] */ Int32 level);

    virtual CARAPI_(void) SetConstantState(
        /* [in] */ DrawableContainerState* state);

private:
    static const Boolean DEBUG = FALSE;
    static const String TAG;

    /**
     * To be proper, we should have a getter for dither (and alpha, etc.)
     * so that proxy classes like this can save/restore their delegates'
     * values, but we don't have getters. Since we do have setters
     * (e.g. setDither), which this proxy forwards on, we have to have some
     * default/initial setting.
     *
     * The initial setting for dither is now true, since it almost always seems
     * to improve the quality at negligible cost.
     */
    static const Boolean DEFAULT_DITHER = TRUE;
    AutoPtr<DrawableContainerState> mDrawableContainerState;
    AutoPtr<IDrawable> mCurrDrawable;
    Int32 mAlpha;
    AutoPtr<IColorFilter> mColorFilter;

    Int32 mCurIndex;
    Boolean mMutated;

    // Animations.
    AutoPtr<IRunnable> mAnimationRunnable;
    Int64 mEnterAnimationEnd;
    Int64 mExitAnimationEnd;
    AutoPtr<IDrawable> mLastDrawable;
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_DRAWABLE_DRAWABLECONTAINER_H__
