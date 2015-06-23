
#ifndef __LEVELLISTDRAWABLE_H__
#define __LEVELLISTDRAWABLE_H__

#include "graphics/drawable/DrawableContainer.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

class LevelListDrawable : public DrawableContainer
{
protected:
    class LevelListState : public DrawableContainer::DrawableContainerState
    {
    public:
        LevelListState(
            /* [in] */ LevelListState* orig,
            /* [in] */ LevelListDrawable* owner,
            /* [in] */ IResources* res);

        CARAPI_(void) AddLevel(
            /* [in] */ Int32 low,
            /* [in] */ Int32 high,
            /* [in] */ IDrawable* drawable);

        CARAPI_(Int32) IndexOfLevel(
            /* [in] */ Int32 level);

        //@Override
        CARAPI NewDrawable(
            /* [out] */ IDrawable** drawable);

        //@Override
        CARAPI NewDrawable(
            /* [in] */ IResources* res,
            /* [out] */ IDrawable** drawable);

        //@Override
        CARAPI_(void) GrowArray(
            /* [in] */ Int32 oldSize,
            /* [in] */ Int32 newSize);

    public:
        AutoPtr< ArrayOf<Int32> > mLows;
        AutoPtr< ArrayOf<Int32> > mHighs;
    };

public:
    LevelListDrawable();

    virtual CARAPI AddLevel(
        /* [in] */ Int32 low,
        /* [in] */ Int32 high,
        /* [in] */ IDrawable* drawable);

    //@Override
    CARAPI Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

    //@Override
    CARAPI_(AutoPtr<IDrawable>) Mutate();

protected:
    CARAPI Init();

    CARAPI Init(
        /* [in] */ LevelListState* state,
        /* [in] */ IResources* res);

    //@Override
    CARAPI_(Boolean) OnLevelChange(
        /* [in] */ Int32 level);

private:
    LevelListDrawable(
        /* [in] */ LevelListState* state,
        /* [in] */ IResources* res);

private:
    AutoPtr<LevelListState> mLevelListState;
    Boolean mMutated;
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__LEVELLISTDRAWABLE_H__
