
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_GRIDLAYOUTANIMATIONCONTROLLER_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_GRIDLAYOUTANIMATIONCONTROLLER_H__

#include "view/animation/LayoutAnimationController.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

class GridLayoutAnimationController : public LayoutAnimationController
{
public:
    /**
     * The set of parameters that has to be attached to each view contained in
     * the view group animated by the grid layout animation controller. These
     * parameters are used to compute the start time of each individual view's
     * animation.
     */
    class GridLayoutAnimationParameters
            : public IGridLayoutAnimationParameters
            , public LayoutAnimationController::AnimationParameters
            , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        GridLayoutAnimationParameters();

        CARAPI GetCount(
            /* [out] */ Int32* count);

        CARAPI SetCount(
            /* [in] */ Int32 count);

        CARAPI GetIndex(
            /* [out] */ Int32* index);

        CARAPI SetIndex(
            /* [in] */ Int32 index);
        CARAPI GetColumn(
            /* [out] */ Int32* column);

        CARAPI SetColumn(
            /* [in] */ Int32 column);

        CARAPI GetRow(
            /* [out] */ Int32* row);

        CARAPI SetRow(
            /* [in] */ Int32 row);

        CARAPI GetColumnsCount(
            /* [out] */ Int32* columnCount);

        CARAPI SetColumnsCount(
            /* [in] */ Int32 columnCount);

        CARAPI GetRowsCount(
            /* [out] */ Int32* rowsCount);

        CARAPI SetRowsCount(
            /* [in] */ Int32 rowsCount);
    protected:
        /**
         * The view group's column to which the view belongs.
         */
        Int32 mColumn;

        /**
         * The view group's row to which the view belongs.
         */
        Int32 mRow;

        /**
         * The number of columns in the view's enclosing grid layout.
         */
        Int32 mColumnsCount;

        /**
         * The number of rows in the view's enclosing grid layout.
         */
        Int32 mRowsCount;

        friend class GridLayoutAnimationController;
    };

public:
    GridLayoutAnimationController(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    GridLayoutAnimationController(
        /* [in] */ IAnimation* animation,
        /* [in] */ Float columnDelay = 0.5f,
        /* [in] */ Float rowDelay = 0.5f);

    virtual CARAPI_(Float) GetColumnDelay();

    virtual CARAPI SetColumnDelay(
        /* [in] */ Float columnDelay);

    virtual CARAPI_(Float) GetRowDelay();

    virtual CARAPI SetRowDelay(
        /* [in] */ Float rowDelay);

    virtual CARAPI_(Int32) GetDirection();

    virtual CARAPI SetDirection(
        /* [in] */ Int32 direction);

    virtual CARAPI_(Int32) GetDirectionPriority();

    virtual CARAPI SetDirectionPriority(
        /* [in] */ Int32 directionPriority);

    //@Override
    CARAPI_(Boolean) WillOverlap();

protected:
    GridLayoutAnimationController();

    //@Override
    CARAPI_(Int64) GetDelayForView(
        /* [in] */ IView* view);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IAnimation* animation,
        /* [in] */ Float columnDelay = 0.5f,
        /* [in] */ Float rowDelay = 0.5f);

private:
    CARAPI_(Int32) GetTransformedColumnIndex(
        /* [in] */ GridLayoutAnimationParameters* params);

    CARAPI_(Int32) GetTransformedRowIndex(
        /* [in] */ GridLayoutAnimationParameters* params);

private:
    Float mColumnDelay;
    Float mRowDelay;

    Int32 mDirection;
    Int32 mDirectionPriority;
};

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif //__ELASTOS_DROID_VIEW_ANIMATION_GRIDLAYOUTANIMATIONCONTROLLER_H__
