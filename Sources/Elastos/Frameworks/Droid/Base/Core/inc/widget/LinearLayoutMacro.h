#ifndef __LINEARLAYOUTMACRO_H__
#define __LINEARLAYOUTMACRO_H__

#define ILINEARLAYOUT_METHODS_DECL()                                           \
    CARAPI SetShowDividers(                                                     \
        /* [in] */ Int32 showDividers);                                         \
                                                                                \
    CARAPI GetShowDividers(                                                     \
        /* [out] */ Int32* showDividers);                                       \
                                                                                \
    CARAPI GetDividerDrawable(                                                  \
        /* [out] */ IDrawable** divider);                                       \
                                                                                \
    CARAPI SetDividerDrawable(                                                  \
        /* [in] */ IDrawable* divider);                                         \
                                                                                \
    CARAPI SetDividerPadding(                                                   \
        /* [in] */ Int32 padding);                                              \
                                                                                \
    CARAPI GetDividerPadding(                                                   \
        /* [out] */ Int32* padding);                                            \
                                                                                \
    CARAPI GetDividerWidth(                                                     \
        /* [out] */ Int32* width);                                              \
                                                                                \
    CARAPI IsBaselineAligned(                                                   \
        /* [out] */ Boolean* baselineAligned);                                  \
                                                                                \
    CARAPI SetBaselineAligned(                                                  \
        /* [in] */ Boolean baselineAligned);                                    \
                                                                                \
    CARAPI IsMeasureWithLargestChildEnabled(                                    \
        /* [out] */ Boolean* enabled);                                          \
                                                                                \
    CARAPI SetMeasureWithLargestChildEnabled(                                   \
        /* [in] */ Boolean enabled);                                            \
                                                                                \
    CARAPI GetBaselineAlignedChildIndex(                                        \
        /* [out] */ Int32* index);                                              \
                                                                                \
    CARAPI SetBaselineAlignedChildIndex(                                        \
        /* [in] */ Int32 index);                                                \
                                                                                \
    CARAPI GetWeightSum(                                                        \
        /* [out] */ Float* weightSum);                                          \
                                                                                \
    CARAPI SetWeightSum(                                                        \
        /* [in] */ Float weightSum);                                            \
                                                                                \
    CARAPI SetOrientation(                                                      \
        /* [in] */ Int32 orientation);                                          \
                                                                                \
    CARAPI GetOrientation(                                                      \
        /* [out] */ Int32* orientation);                                        \
                                                                                \
    CARAPI SetGravity(                                                          \
        /* [in] */ Int32 gravity);                                              \
                                                                                \
    CARAPI SetHorizontalGravity(                                                \
        /* [in] */ Int32 horizontalGravity);                                    \
                                                                                \
    CARAPI SetVerticalGravity(                                                  \
        /* [in] */ Int32 verticalGravity);

#define ILINEARLAYOUT_METHODS_IMPL(className, superClass)                      \
ECode className::SetShowDividers(                                               \
    /* [in] */ Int32 showDividers)                                              \
{                                                                               \
    return superClass::SetShowDividers(showDividers);                          \
}                                                                               \
                                                                                \
ECode className::GetShowDividers(                                               \
    /* [out] */ Int32* showDividers)                                            \
{                                                                               \
    VALIDATE_NOT_NULL(showDividers);                                            \
    *showDividers = superClass::GetShowDividers();                              \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::GetDividerDrawable(                                            \
    /* [out] */ IDrawable** divider)                                            \
{                                                                               \
    VALIDATE_NOT_NULL(divider);                                                 \
    AutoPtr<IDrawable> d = superClass::GetDividerDrawable();                    \
    *divider = d;                                                               \
    INTERFACE_ADDREF(*divider);                                                 \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetDividerDrawable(                                            \
    /* [in] */ IDrawable* divider)                                              \
{                                                                               \
    return superClass::SetDividerDrawable(divider);                            \
}                                                                               \
                                                                                \
ECode className::SetDividerPadding(                                             \
    /* [in] */ Int32 padding)                                                   \
{                                                                               \
    return superClass::SetDividerPadding(padding);                             \
}                                                                               \
                                                                                \
ECode className::GetDividerPadding(                                             \
    /* [out] */ Int32* padding)                                                 \
{                                                                               \
    VALIDATE_NOT_NULL(padding);                                                 \
    *padding = superClass::GetDividerPadding();                                 \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::GetDividerWidth(                                               \
    /* [out] */ Int32* width)                                                   \
{                                                                               \
    VALIDATE_NOT_NULL(width);                                                   \
    *width = superClass::GetDividerWidth();                                     \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::IsBaselineAligned(                                             \
    /* [out] */ Boolean* baselineAligned)                                       \
{                                                                               \
    VALIDATE_NOT_NULL(baselineAligned);                                         \
    *baselineAligned = superClass::IsBaselineAligned();                         \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetBaselineAligned(                                            \
    /* [in] */ Boolean baselineAligned)                                         \
{                                                                               \
    return superClass::SetBaselineAligned(baselineAligned);                    \
}                                                                               \
                                                                                \
ECode className::IsMeasureWithLargestChildEnabled(                              \
    /* [out] */ Boolean* enabled)                                               \
{                                                                               \
    VALIDATE_NOT_NULL(enabled);                                                 \
    *enabled = superClass::IsMeasureWithLargestChildEnabled();                  \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetMeasureWithLargestChildEnabled(                             \
    /* [in] */ Boolean enabled)                                                 \
{                                                                               \
    return superClass::SetMeasureWithLargestChildEnabled(enabled);             \
}                                                                               \
                                                                                \
ECode className::GetBaselineAlignedChildIndex(                                  \
    /* [out] */ Int32* index)                                                   \
{                                                                               \
    VALIDATE_NOT_NULL(index);                                                   \
    *index = superClass::GetBaselineAlignedChildIndex();                        \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetBaselineAlignedChildIndex(                                  \
    /* [in] */ Int32 index)                                                     \
{                                                                               \
    return superClass::SetBaselineAlignedChildIndex(index);                    \
}                                                                               \
                                                                                \
ECode className::GetWeightSum(                                                  \
    /* [out] */ Float* weightSum)                                               \
{                                                                               \
    VALIDATE_NOT_NULL(weightSum);                                               \
    *weightSum = superClass::GetWeightSum();                                    \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetWeightSum(                                                  \
    /* [in] */ Float weightSum)                                                 \
{                                                                               \
    return superClass::SetWeightSum(weightSum);                                \
}                                                                               \
                                                                                \
ECode className::SetOrientation(                                                \
    /* [in] */ Int32 orientation)                                               \
{                                                                               \
    return superClass::SetOrientation(orientation);                            \
}                                                                               \
                                                                                \
ECode className::GetOrientation(                                                \
    /* [out] */ Int32* orientation)                                             \
{                                                                               \
    VALIDATE_NOT_NULL(orientation);                                             \
    *orientation = superClass::GetOrientation();                                \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetGravity(                                                    \
    /* [in] */ Int32 gravity)                                                   \
{                                                                               \
    return superClass::SetGravity(gravity);                                    \
}                                                                               \
                                                                                \
ECode className::SetHorizontalGravity(                                          \
    /* [in] */ Int32 horizontalGravity)                                         \
{                                                                               \
    return superClass::SetHorizontalGravity(horizontalGravity);                \
}                                                                               \
                                                                                \
ECode className::SetVerticalGravity(                                            \
    /* [in] */ Int32 verticalGravity)                                           \
{                                                                               \
    return superClass::SetVerticalGravity(verticalGravity);                    \
}

#define ILINEARLAYOUTLP_METHODS_DECL()                                         \
    CARAPI GetWeight(                                                           \
        /* [out] */ Float* weight);                                             \
                                                                                \
    CARAPI SetWeight(                                                           \
        /* [in] */ Float weight);                                               \
                                                                                \
    CARAPI GetGravity(                                                          \
        /* [out] */ Int32* gravity);                                            \
                                                                                \
    CARAPI SetGravity(                                                          \
        /* [in] */ Int32 gravity);


#define ILINEARLAYOUTLP_METHODS_IMPL(className, superClass)                    \
ECode className::GetWeight(                                                     \
    /* [out] */ Float* weight)                                                  \
{                                                                               \
    VALIDATE_NOT_NULL(weight);                                                  \
    *weight = superClass::GetWeight();                                          \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetWeight(                                                     \
    /* [in] */ Float weight)                                                    \
{                                                                               \
    return superClass::SetWeight(weight);                                      \
}                                                                               \
                                                                                \
ECode className::GetGravity(                                                    \
    /* [out] */ Int32* gravity)                                                 \
{                                                                               \
    VALIDATE_NOT_NULL(gravity);                                                 \
    *gravity = superClass::GetGravity();                                        \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetGravity(                                                    \
    /* [in] */ Int32 gravity)                                                   \
{                                                                               \
    return superClass::SetGravity(gravity);                                    \
}

#endif //__LINEARLAYOUTMACRO_H__
