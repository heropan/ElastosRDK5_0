
#ifndef __ANIMATIONSET_H__
#define __ANIMATIONSET_H__

#include "view/animation/Animation.h"
#include <elastos/List.h>

using Elastos::Utility::List;
using Elastos::Droid::Graphics::IRectF;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

class AnimationSet : public Animation
{
public:

    AnimationSet(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    AnimationSet(
        /* [in] */ Boolean shareInterpolator);

    CARAPI SetFillAfter(
        /* [in] */ Boolean fillAfter);

    CARAPI SetFillBefore(
        /* [in] */ Boolean fillBefore);

    CARAPI SetRepeatMode(
        /* [in] */ Int32 repeatMode);

    CARAPI SetStartOffset(
        /* [in] */ Int64 startOffset);

    CARAPI SetDuration(
        /* [in] */ Int64 durationMillis);

    CARAPI AddAnimation(
        /* [in] */ IAnimation* a);

    CARAPI SetStartTime(
        /* [in] */ Int64 startTimeMillis);

    CARAPI_(Int64) GetStartTime();

    CARAPI RestrictDuration(
        /* [in] */ Int64 durationMillis);

    CARAPI_(Int64) GetDuration();

    CARAPI_(Int64) ComputeDurationHint();

    CARAPI InitializeInvalidateRegion(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI_(Boolean) GetTransformation(
        /* [in] */ Int64 currentTime,
        /* [in] */ ITransformation* t);

    CARAPI ScaleCurrentDuration(
        /* [in] */  Float scale);

    CARAPI Initialize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 parentWidth,
        /* [in] */ Int32 parentHeight);

    CARAPI Reset();

    CARAPI RestoreChildrenStartOffset();

    CARAPI GetAnimations(
        /* [out] */ IObjectContainer** animations);

    CARAPI_(Boolean) WillChangeTransformationMatrix();

    CARAPI_(Boolean) WillChangeBounds();

    CARAPI_(Boolean) HasAlpha();

protected:
    AnimationSet();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Boolean shareInterpolator);

    virtual CARAPI_(AutoPtr<IAnimation>) GetCloneInstance();

    CARAPI_(AutoPtr<IAnimation>) Clone();

private:
    CARAPI_(void) SetFlag(
        /* [in] */ Int32 mask,
        /* [in] */ Boolean value);

    CARAPI_(void) InitInternal();

private:
    static const Int32 PROPERTY_FILL_AFTER_MASK         = 0x1;
    static const Int32 PROPERTY_FILL_BEFORE_MASK        = 0x2;
    static const Int32 PROPERTY_REPEAT_MODE_MASK        = 0x4;
    static const Int32 PROPERTY_START_OFFSET_MASK       = 0x8;
    static const Int32 PROPERTY_SHARE_INTERPOLATOR_MASK = 0x10;
    static const Int32 PROPERTY_DURATION_MASK           = 0x20;
    static const Int32 PROPERTY_MORPH_MATRIX_MASK       = 0x40;
    static const Int32 PROPERTY_CHANGE_BOUNDS_MASK      = 0x80;

private:
    Int32 mFlags;
    List<AutoPtr<IAnimation> > mAnimations;
    AutoPtr<ITransformation> mTempTransformation;
    Int64 mLastEnd;
    AutoPtr<ArrayOf<Int64> > mStoredOffsets;
    Boolean mDirty;
    Boolean mHasAlpha;
};


}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif //__ANIMATIONSET_H__
