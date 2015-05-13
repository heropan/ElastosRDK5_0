#ifndef __IMAGEVIEWMACRO_H__
#define __IMAGEVIEWMACRO_H__

using Elastos::Droid::Widget::ImageViewScaleType;

#define IIMAGEVIEW_METHODS_DECL()                                              \
    CARAPI GetAdjustViewBounds(                                                 \
        /* [out] */ Boolean* res);                                              \
                                                                                \
    CARAPI SetAdjustViewBounds(                                                 \
        /* [in] */ Boolean adjustViewBounds);                                   \
                                                                                \
    CARAPI GetMaxWidth(                                                         \
        /* [out] */ Int32* width);                                              \
                                                                                \
    CARAPI SetMaxWidth(                                                         \
        /* [in] */ Int32 maxWidth);                                             \
                                                                                \
    CARAPI GetMaxHeight(                                                        \
        /* [out] */ Int32* height);                                             \
                                                                                \
    CARAPI SetMaxHeight(                                                        \
        /* [in] */ Int32 maxHeight);                                            \
                                                                                \
    CARAPI GetDrawable(                                                         \
        /* [out] */ IDrawable** drawable);                                      \
                                                                                \
    CARAPI SetImageResource(                                                    \
        /* [in] */ Int32 resId);                                                \
                                                                                \
    CARAPI SetImageURI(                                                         \
        /* [in] */ IUri* uri);                                                  \
                                                                                \
    CARAPI SetImageDrawable(                                                    \
        /* [in] */ IDrawable* drawable);                                        \
                                                                                \
    CARAPI SetImageBitmap(                                                      \
        /* [in] */ IBitmap* bm);                                                \
                                                                                \
    CARAPI SetImageState(                                                       \
        /* [in] */ ArrayOf<Int32>* state,                                       \
        /* [in] */ Boolean mg);                                                 \
                                                                                \
    CARAPI SetImageLevel(                                                       \
        /* [in] */ Int32 level);                                                \
                                                                                \
    CARAPI SetScaleType(                                                        \
        /* [in] */ ImageViewScaleType scaleType);                               \
                                                                                \
    CARAPI GetScaleType(                                                        \
        /* [out] */ ImageViewScaleType* scaleType);                             \
                                                                                \
    CARAPI GetImageMatrix(                                                      \
        /* [out] */ IMatrix** martix);                                          \
                                                                                \
    CARAPI SetImageMatrix(                                                      \
        /* [in] */ IMatrix* matrix);                                            \
                                                                                \
    CARAPI GetCropToPadding(                                                    \
        /* [out] */ Boolean* padding);                                          \
                                                                                \
    CARAPI SetCropToPadding(                                                    \
        /* [in] */ Boolean cropToPadding);                                      \
                                                                                \
    CARAPI SetBaseline(                                                         \
        /* [in] */ Int32 baseline);                                             \
                                                                                \
    CARAPI SetBaselineAlignBottom(                                              \
        /* [in] */ Boolean aligned);                                            \
                                                                                \
    CARAPI GetBaselineAlignBottom(                                              \
        /* [out] */ Boolean* aligned);                                          \
                                                                                \
    CARAPI SetColorFilter(                                                      \
        /* [in] */ Int32 color);                                                \
                                                                                \
    CARAPI SetColorFilterEx(                                                    \
        /* [in] */ Int32 color,                                                 \
        /* [in] */ PorterDuffMode mode);                                        \
                                                                                \
    CARAPI ClearColorFilter();                                                  \
                                                                                \
    CARAPI GetColorFilter(                                                      \
        /* [out] */ IColorFilter** filter);                                     \
                                                                                \
    CARAPI SetColorFilterEx2(                                                   \
        /* [in] */ IColorFilter* cf);                                           \
                                                                                \
    CARAPI GetImageAlpha(                                                       \
        /* [out] */ Int32* alpha);                                              \
                                                                                \
    CARAPI SetImageAlpha(                                                       \
        /* [in] */ Int32 alpha);



#define IIMAGEVIEW_METHODS_IMPL(className, superClass)                         \
ECode className::GetAdjustViewBounds(                                           \
    /* [out] */ Boolean* res)                                                   \
{                                                                               \
    VALIDATE_NOT_NULL(res);                                                     \
    *res = superClass::GetAdjustViewBounds();                                   \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetAdjustViewBounds(                                           \
    /* [in] */ Boolean adjustViewBounds)                                        \
{                                                                               \
    return superClass::SetAdjustViewBounds(adjustViewBounds);                  \
}                                                                               \
                                                                                \
ECode className::GetMaxWidth(                                                   \
    /* [out] */ Int32* width)                                                   \
{                                                                               \
    VALIDATE_NOT_NULL(width);                                                   \
    *width = superClass::GetMaxWidth();                                         \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetMaxWidth(                                                   \
    /* [in] */ Int32 maxWidth)                                                  \
{                                                                               \
    return superClass::SetMaxWidth(maxWidth);                                  \
}                                                                               \
                                                                                \
ECode className::GetMaxHeight(                                                  \
    /* [out] */ Int32* height)                                                  \
{                                                                               \
    VALIDATE_NOT_NULL(height);                                                  \
    *height = superClass::GetMaxHeight();                                       \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetMaxHeight(                                                  \
    /* [in] */ Int32 maxHeight)                                                 \
{                                                                               \
    return superClass::SetMaxHeight(maxHeight);                                \
}                                                                               \
                                                                                \
ECode className::GetDrawable(                                                   \
    /* [out] */ IDrawable** drawable)                                           \
{                                                                               \
    VALIDATE_NOT_NULL(drawable);                                                \
    AutoPtr<IDrawable> cf = superClass::GetDrawable();                          \
    *drawable = cf;                                                             \
    INTERFACE_ADDREF(*drawable);                                                \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetImageResource(                                              \
    /* [in] */ Int32 resId)                                                     \
{                                                                               \
    return superClass::SetImageResource(resId);                                \
}                                                                               \
                                                                                \
ECode className::SetImageURI(                                                   \
    /* [in] */ IUri* uri)                                                       \
{                                                                               \
    return superClass::SetImageURI(uri);                                       \
}                                                                               \
                                                                                \
ECode className::SetImageDrawable(                                              \
    /* [in] */ IDrawable* drawable)                                             \
{                                                                               \
    return superClass::SetImageDrawable(drawable);                             \
}                                                                               \
                                                                                \
ECode className::SetImageBitmap(                                                \
    /* [in] */ IBitmap* bm)                                                     \
{                                                                               \
    return superClass::SetImageBitmap(bm);                                     \
}                                                                               \
                                                                                \
ECode className::SetImageState(                                                 \
    /* [in] */ ArrayOf<Int32>* state,                                           \
    /* [in] */ Boolean mg)                                                      \
{                                                                               \
    return superClass::SetImageState(state, mg);                               \
}                                                                               \
                                                                                \
ECode className::SetImageLevel(                                                 \
    /* [in] */ Int32 level)                                                     \
{                                                                               \
    return superClass::SetImageLevel(level);                                   \
}                                                                               \
                                                                                \
ECode className::SetScaleType(                                                  \
    /* [in] */ ImageViewScaleType scaleType)                                    \
{                                                                               \
    return superClass::SetScaleType(scaleType);                                \
}                                                                               \
                                                                                \
ECode className::GetScaleType(                                                  \
    /* [out] */ ImageViewScaleType* scaleType)                                  \
{                                                                               \
    VALIDATE_NOT_NULL(scaleType);                                               \
    *scaleType = superClass::GetScaleType();                                    \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::GetImageMatrix(                                                \
    /* [out] */ IMatrix** martix)                                               \
{                                                                               \
    VALIDATE_NOT_NULL(martix);                                                  \
    AutoPtr<IMatrix> cf = superClass::GetImageMatrix();                         \
    *martix = cf;                                                               \
    INTERFACE_ADDREF(*martix);                                                  \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetImageMatrix(                                                \
    /* [in] */ IMatrix* matrix)                                                 \
{                                                                               \
    return superClass::SetImageMatrix(matrix);                                 \
}                                                                               \
                                                                                \
ECode className::GetCropToPadding(                                              \
    /* [out] */ Boolean* padding)                                               \
{                                                                               \
    VALIDATE_NOT_NULL(padding);                                                 \
    *padding = superClass::GetCropToPadding();                                  \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetCropToPadding(                                              \
    /* [in] */ Boolean cropToPadding)                                           \
{                                                                               \
    return superClass::SetCropToPadding(cropToPadding);                        \
}                                                                               \
                                                                                \
ECode className::SetBaseline(                                                   \
    /* [in] */ Int32 baseline)                                                  \
{                                                                               \
    return superClass::SetBaseline(baseline);                                  \
}                                                                               \
                                                                                \
ECode className::SetBaselineAlignBottom(                                        \
    /* [in] */ Boolean aligned)                                                 \
{                                                                               \
    return superClass::SetBaselineAlignBottom(aligned);                        \
}                                                                               \
                                                                                \
ECode className::GetBaselineAlignBottom(                                        \
    /* [out] */ Boolean* aligned)                                               \
{                                                                               \
    VALIDATE_NOT_NULL(aligned);                                                 \
    *aligned = superClass::GetBaselineAlignBottom();                            \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetColorFilter(                                                \
    /* [in] */ Int32 color)                                                     \
{                                                                               \
    return superClass::SetColorFilter(color);                                  \
}                                                                               \
                                                                                \
ECode className::SetColorFilterEx(                                              \
    /* [in] */ Int32 color,                                                     \
    /* [in] */ PorterDuffMode mode)                                             \
{                                                                               \
    return superClass::SetColorFilter(color, mode);                            \
}                                                                               \
                                                                                \
ECode className::ClearColorFilter()                                             \
{                                                                               \
    return superClass::ClearColorFilter();                                     \
}                                                                               \
                                                                                \
ECode className::GetColorFilter(                                                \
    /* [out] */ IColorFilter** filter)                                          \
{                                                                               \
    VALIDATE_NOT_NULL(filter);                                                  \
    AutoPtr<IColorFilter> cf = superClass::GetColorFilter();                    \
    *filter = cf;                                                               \
    INTERFACE_ADDREF(*filter);                                                  \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetColorFilterEx2(                                             \
    /* [in] */ IColorFilter* cf)                                                \
{                                                                               \
    return superClass::SetColorFilter(cf);                                     \
}                                                                               \
                                                                                \
ECode className::GetImageAlpha(                                                 \
    /* [out] */ Int32* alpha)                                                   \
{                                                                               \
    VALIDATE_NOT_NULL(alpha);                                                   \
    *alpha = superClass::GetImageAlpha();                                       \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetImageAlpha(                                                 \
    /* [in] */ Int32 alpha)                                                     \
{                                                                               \
    return superClass::SetImageAlpha(alpha);                                   \
}


#endif // __IMAGEVIEWMACRO_H__
