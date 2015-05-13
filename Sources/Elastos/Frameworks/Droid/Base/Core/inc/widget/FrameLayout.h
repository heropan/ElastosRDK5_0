
#ifndef __FRAMELAYOUT_H__
#define __FRAMELAYOUT_H__


#include "view/ViewGroup.h"
#include "widget/CLinearLayoutLayoutParams.h"

using Elastos::Droid::View::ViewGroup;

namespace Elastos{
namespace Droid{
namespace Widget{

class FrameLayout : public ViewGroup
{
public:
    FrameLayout();

    FrameLayout(
        /* [in] */ IContext* context);

    FrameLayout(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    FrameLayout(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    virtual CARAPI_(Int32) GetForegroundGravity();

    virtual CARAPI SetForegroundGravity(
        /* [in] */ Int32 foregroundGravity);

    virtual CARAPI SetForeground(
        /* [in] */ IDrawable* drawable);

    virtual CARAPI_(AutoPtr<IDrawable>) GetForeground();

    virtual CARAPI_(Boolean) GetMeasureAllChildren();

    virtual CARAPI SetMeasureAllChildren(
        /* [in] */ Boolean measureAll);

    virtual CARAPI_(Boolean) GetConsiderGoneChildrenWhenMeasuring();


    virtual CARAPI JumpDrawablesToCurrentState();

    virtual CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI_(Boolean) GatherTransparentRegion(
        /* [in] */ IRegion* region);

    CARAPI GetConsiderGoneChildrenWhenMeasuring(
            /* [out] */ Boolean* measureAll);

    virtual CARAPI GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IViewGroupLayoutParams** params);

    virtual CARAPI_(Boolean) ShouldDelayChildPressedState();

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

protected:
    virtual CARAPI_(Boolean) VerifyDrawable(
        /* [in] */ IDrawable* who);

    virtual CARAPI DrawableStateChanged();

    virtual CARAPI GenerateDefaultLayoutParams(
        /* [out] */ IViewGroupLayoutParams** params);

    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    virtual CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    virtual CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 oldw,
        /* [in] */ Int32 oldh);

    virtual CARAPI_(Boolean) CheckLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

    virtual CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [int] */ IViewGroupLayoutParams* p);

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

private:
    CARAPI_(Int32) GetPaddingLeftWithForeground();

    CARAPI_(Int32) GetPaddingRightWithForeground();

    CARAPI_(Int32) GetPaddingTopWithForeground();

    CARAPI_(Int32) GetPaddingBottomWithForeground();

protected:
    Boolean mForegroundInPadding;

private:
    static Int32 DEFAULT_CHILD_GRAVITY; // Gravity.TOP | Gravity.START;

    Boolean mMeasureAllChildren;

    AutoPtr<IDrawable> mForeground;

    Int32 mForegroundPaddingLeft;

    Int32 mForegroundPaddingTop;

    Int32 mForegroundPaddingRight;

    Int32 mForegroundPaddingBottom;

    AutoPtr<CRect> mSelfBounds;
    AutoPtr<CRect> mOverlayBounds;

    Int32 mForegroundGravity;

    Boolean mForegroundBoundsChanged;

    List<AutoPtr<IView> > mMatchParentChildren;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__FRAMELAYOUT_H__
