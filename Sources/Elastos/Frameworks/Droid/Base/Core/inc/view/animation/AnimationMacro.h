
#ifndef __ANIMATIONMACRO_H__
#define __ANIMATIONMACRO_H__

#define IANIMATION_METHODS_DECL()                                       \
    CARAPI Clone(                                                       \
        /* [out] */ IAnimation** animation);                            \
                                                                        \
    CARAPI Reset();                                                     \
                                                                        \
    CARAPI Cancel();                                                    \
                                                                        \
    CARAPI Detach();                                                    \
                                                                        \
    CARAPI IsInitialized(                                               \
        /* [out] */ Boolean* isInitialized);                            \
                                                                        \
    CARAPI Initialize(                                                  \
        /* [in] */ Int32 width,                                         \
        /* [in] */ Int32 height,                                        \
        /* [in] */ Int32 parentWidth,                                   \
        /* [in] */ Int32 parentHeight);                                 \
                                                                        \
    CARAPI SetInterpolator(                                             \
        /* [in] */ IContext* context,                                   \
        /* [in] */ Int32 resID);                                        \
                                                                        \
    CARAPI SetInterpolatorEx(                                           \
        /* [in] */ IInterpolator* i);                                   \
                                                                        \
    CARAPI SetStartOffset(                                              \
        /* [in] */ Int64 startOffset);                                  \
                                                                        \
    CARAPI SetDuration(                                                 \
        /* [in] */ Int64 durationMillis);                               \
                                                                        \
    CARAPI RestrictDuration(                                            \
        /* [in] */ Int64 durationMillis);                               \
                                                                        \
    CARAPI ScaleCurrentDuration(                                        \
        /* [in] */ Float scale);                                        \
                                                                        \
    CARAPI SetListenerHandler(                                          \
        /* [in] */ IHandler* handler);                                  \
                                                                        \
    CARAPI SetBackgroundColor(                                          \
        /* [in] */ Int32 bg);                                           \
                                                                        \
    CARAPI GetBackgroundColor(                                          \
        /* [out] */ Int32* bg);                                         \
                                                                        \
    CARAPI SetStartTime(                                                \
        /* [in] */ Int64 startTimeMillis);                              \
                                                                        \
    CARAPI Start();                                                     \
                                                                        \
    CARAPI StartNow();                                                  \
                                                                        \
    CARAPI SetRepeatMode(                                               \
        /* [in] */ Int32 repeatMode);                                   \
                                                                        \
    CARAPI SetRepeatCount(                                              \
        /* [in] */ Int32 repeatCount);                                  \
                                                                        \
    CARAPI IsFillEnabled(                                               \
        /* [out] */ Boolean* isFillEnabled);                            \
                                                                        \
    CARAPI SetFillEnabled(                                              \
        /* [in] */ Boolean fillEnabled);                                \
                                                                        \
    CARAPI SetFillBefore(                                               \
        /* [in] */ Boolean fillBefore);                                 \
                                                                        \
    CARAPI SetFillAfter(                                                \
        /* [in] */ Boolean fillAfter);                                  \
                                                                        \
    CARAPI SetZAdjustment(                                              \
        /* [in] */ Int32 zAdjustment);                                  \
                                                                        \
    CARAPI SetDetachWallpaper(                                          \
        /* [in] */ Boolean detachWallpaper);                            \
                                                                        \
    CARAPI GetInterpolator(                                             \
        /* [out] */ IInterpolator** interpolator);                      \
                                                                        \
    CARAPI GetStartTime(                                                \
        /* [out] */ Int64* time);                                       \
                                                                        \
    CARAPI GetDuration(                                                 \
        /* [out] */ Int64* time);                                       \
                                                                        \
    CARAPI GetStartOffset(                                              \
        /* [out] */ Int64* startOffset);                                \
                                                                        \
    CARAPI GetRepeatMode(                                               \
        /* [out] */ Int32* mode);                                       \
                                                                        \
    CARAPI GetRepeatCount(                                              \
        /* [out] */ Int32* count);                                      \
                                                                        \
    CARAPI GetFillBefore(                                               \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI GetFillAfter(                                                \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI GetZAdjustment(                                              \
        /* [out] */ Int32* zAdjustment);                                \
                                                                        \
    CARAPI GetDetachWallpaper(                                          \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI WillChangeTransformationMatrix(                              \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI WillChangeBounds(                                            \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI HasAlpha(                                                    \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI SetAnimationListener(                                        \
        /* [in] */ IAnimationListener* listener);                       \
                                                                        \
    CARAPI ComputeDurationHint(                                         \
        /* [out] */ Int64* hint);                                       \
                                                                        \
    CARAPI GetTransformation(                                           \
        /* [in] */ Int64 currentTime,                                   \
        /* [in] */ ITransformation* outTransformation,                  \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI GetTransformationEx(                                         \
        /* [in] */ Int64 currentTime,                                   \
        /* [in] */ ITransformation* outTransformation,                  \
        /* [in] */ Float scale,                                         \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI HasStarted(                                                  \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI HasEnded(                                                    \
        /* [out] */ Boolean* result);                                   \
    CARAPI GetInvalidateRegion(                                         \
        /* [in] */ Int32 left,                                          \
        /* [in] */ Int32 top,                                           \
        /* [in] */ Int32 right,                                         \
        /* [in] */ Int32 bottom,                                        \
        /* [in] */ IRectF* invalidate,                                  \
        /* [in] */ ITransformation* transformation);                    \
                                                                        \
    CARAPI InitializeInvalidateRegion(                                  \
        /* [in] */ Int32 left,                                          \
        /* [in] */ Int32 top,                                           \
        /* [in] */ Int32 right,                                         \
        /* [in] */ Int32 bottom);

#define IANIMATION_METHODS_IMPL(className, superClass)                  \
ECode className::Clone(                                                 \
    /* [out] */ IAnimation** animation)                                 \
{                                                                       \
    VALIDATE_NOT_NULL(animation);                                       \
    AutoPtr<IAnimation> temp = superClass::Clone();                     \
    *animation = temp;                                                  \
    if(*animation)                                                      \
        (*animation)->AddRef();                                         \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::Reset()                                                \
{                                                                       \
    return superClass::Reset();                                         \
}                                                                       \
                                                                        \
ECode className::Cancel()                                               \
{                                                                       \
    return superClass::Cancel();                                        \
}                                                                       \
                                                                        \
ECode className::Detach()                                               \
{                                                                       \
    return superClass::Detach();                                        \
}                                                                       \
                                                                        \
ECode className::IsInitialized(                                         \
    /* [out] */ Boolean* isInitialized)                                 \
{                                                                       \
    VALIDATE_NOT_NULL(isInitialized);                                   \
    *isInitialized = superClass::IsInitialized();                       \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::Initialize(                                            \
    /* [in] */ Int32 width,                                             \
    /* [in] */ Int32 height,                                            \
    /* [in] */ Int32 parentWidth,                                       \
    /* [in] */ Int32 parentHeight)                                      \
{                                                                       \
    return superClass::Initialize(width, height, parentWidth, parentHeight);\
}                                                                       \
                                                                        \
ECode className::SetInterpolator(                                       \
    /* [in] */ IContext* context,                                       \
    /* [in] */ Int32 resID)                                             \
{                                                                       \
    return superClass::SetInterpolator(context, resID);                 \
}                                                                       \
                                                                        \
ECode className::SetInterpolatorEx(                                     \
    /* [in] */ IInterpolator* i)                                        \
{                                                                       \
    return superClass::SetInterpolator(i);                              \
}                                                                       \
                                                                        \
ECode className::SetStartOffset(                                        \
    /* [in] */ Int64 startOffset)                                       \
{                                                                       \
    return superClass::SetStartOffset(startOffset);                     \
}                                                                       \
                                                                        \
ECode className::SetDuration(                                           \
    /* [in] */ Int64 durationMillis)                                    \
{                                                                       \
    return superClass::SetDuration(durationMillis);                     \
}                                                                       \
                                                                        \
ECode className::RestrictDuration(                                      \
    /* [in] */ Int64 durationMillis)                                    \
{                                                                       \
    return superClass::RestrictDuration(durationMillis);                \
}                                                                       \
                                                                        \
ECode className::ScaleCurrentDuration(                                  \
    /* [in] */ Float scale)                                             \
{                                                                       \
    return superClass::ScaleCurrentDuration(scale);                     \
}                                                                       \
                                                                        \
ECode className::SetListenerHandler(                                    \
        /* [in] */ IHandler* handler)                                   \
{                                                                       \
    return superClass::SetListenerHandler(handler);                     \
}                                                                       \
                                                                        \
ECode className::SetBackgroundColor(                                    \
        /* [in] */ Int32 bg)                                            \
{                                                                       \
    return superClass::SetBackgroundColor(bg);                          \
}                                                                       \
                                                                        \
ECode className::GetBackgroundColor(                                    \
        /* [out] */ Int32* bg)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(bg);                                              \
    superClass::GetBackgroundColor(bg);                                 \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetStartTime(                                          \
    /* [in] */ Int64 startTimeMillis)                                   \
{                                                                       \
    return superClass::SetStartTime(startTimeMillis);                   \
}                                                                       \
                                                                        \
ECode className::Start()                                                \
{                                                                       \
    return superClass::Start();                                         \
}                                                                       \
                                                                        \
ECode className::StartNow()                                             \
{                                                                       \
    return superClass::StartNow();                                      \
}                                                                       \
                                                                        \
ECode className::SetRepeatMode(                                         \
    /* [in] */ Int32 repeatMode)                                        \
{                                                                       \
    return superClass::SetRepeatMode(repeatMode);                       \
}                                                                       \
                                                                        \
ECode className::SetRepeatCount(                                        \
    /* [in] */ Int32 repeatCount)                                       \
{                                                                       \
    return superClass::SetRepeatCount(repeatCount);                     \
}                                                                       \
                                                                        \
ECode className::IsFillEnabled(                                         \
    /* [out] */ Boolean* isFillEnabled)                                 \
{                                                                       \
    VALIDATE_NOT_NULL(isFillEnabled);                                   \
    *isFillEnabled = superClass::IsFillEnabled();                       \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetFillEnabled(                                        \
    /* [in] */ Boolean fillEnabled)                                     \
{                                                                       \
    return superClass::SetFillEnabled(fillEnabled);                     \
}                                                                       \
                                                                        \
ECode className::SetFillBefore(                                         \
    /* [in] */ Boolean fillBefore)                                      \
{                                                                       \
    return superClass::SetFillBefore(fillBefore);                       \
}                                                                       \
                                                                        \
ECode className::SetFillAfter(                                          \
    /* [in] */ Boolean fillAfter)                                       \
{                                                                       \
    return superClass::SetFillAfter(fillAfter);                         \
}                                                                       \
                                                                        \
ECode className::SetZAdjustment(                                        \
    /* [in] */ Int32 zAdjustment)                                       \
{                                                                       \
    return superClass::SetZAdjustment(zAdjustment);                     \
}                                                                       \
                                                                        \
ECode className::SetDetachWallpaper(                                    \
    /* [in] */ Boolean detachWallpaper)                                 \
{                                                                       \
    return superClass::SetDetachWallpaper(detachWallpaper);             \
}                                                                       \
                                                                        \
ECode className::GetInterpolator(                                       \
    /* [out] */ IInterpolator** interpolator)                           \
{                                                                       \
    VALIDATE_NOT_NULL(interpolator);                                    \
    AutoPtr<IInterpolator> temp = superClass::GetInterpolator();        \
    *interpolator = temp;                                               \
    if (*interpolator) {                                                \
        (*interpolator)->AddRef();                                      \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetStartTime(                                          \
    /* [out] */ Int64* time)                                            \
{                                                                       \
    VALIDATE_NOT_NULL(time);                                            \
    *time = superClass::GetStartTime();                                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetDuration(                                           \
    /* [out] */ Int64* time)                                            \
{                                                                       \
    VALIDATE_NOT_NULL(time);                                            \
    *time = superClass::GetDuration();                                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetStartOffset(                                        \
    /* [out] */ Int64* startOffset)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(startOffset);                                     \
    *startOffset = superClass::GetStartOffset();                        \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetRepeatMode(                                         \
    /* [out] */ Int32* mode)                                            \
{                                                                       \
    VALIDATE_NOT_NULL(mode);                                            \
    *mode = superClass::GetRepeatMode();                                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetRepeatCount(                                        \
    /* [out] */ Int32* count)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(count);                                           \
    *count = superClass::GetRepeatCount();                              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetFillBefore(                                         \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::GetFillBefore();                              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetFillAfter(                                          \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::GetFillAfter();                               \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetZAdjustment(                                        \
    /* [out] */ Int32* zAdjustment)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(zAdjustment);                                     \
    *zAdjustment = superClass::GetZAdjustment();                        \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetDetachWallpaper(                                    \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::GetDetachWallpaper();                         \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::WillChangeTransformationMatrix(                        \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::WillChangeTransformationMatrix();             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::WillChangeBounds(                                      \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::WillChangeBounds();                           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::HasAlpha(                                              \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::HasAlpha();                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetAnimationListener(                                  \
    /* [in] */ IAnimationListener* listener)                            \
{                                                                       \
    return superClass::SetAnimationListener(listener);                  \
}                                                                       \
                                                                        \
ECode className::ComputeDurationHint(                                   \
    /* [out] */ Int64* hint)                                            \
{                                                                       \
    VALIDATE_NOT_NULL(hint);                                            \
    *hint = superClass::ComputeDurationHint();                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetTransformation(                                     \
    /* [in] */ Int64 currentTime,                                       \
    /* [in] */ ITransformation* outTransformation,                      \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::GetTransformation(currentTime, outTransformation);\
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetTransformationEx(                                   \
    /* [in] */ Int64 currentTime,                                       \
    /* [in] */ ITransformation* outTransformation,                      \
    /* [in] */ Float scale,                                             \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::GetTransformationEx(currentTime, outTransformation, scale); \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::HasStarted(                                            \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::HasStarted();                                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::HasEnded(                                              \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::HasEnded();                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetInvalidateRegion(                                   \
    /* [in] */ Int32 left,                                              \
    /* [in] */ Int32 top,                                               \
    /* [in] */ Int32 right,                                             \
    /* [in] */ Int32 bottom,                                            \
    /* [in] */ IRectF* invalidate,                                      \
    /* [in] */ ITransformation* transformation)                         \
{                                                                       \
    return superClass::GetInvalidateRegion(                             \
            left, top, right, bottom, invalidate, transformation);      \
}                                                                       \
                                                                        \
ECode className::InitializeInvalidateRegion(                            \
        /* [in] */ Int32 left,                                          \
        /* [in] */ Int32 top,                                           \
        /* [in] */ Int32 right,                                         \
        /* [in] */ Int32 bottom)                                        \
{                                                                       \
    return superClass::InitializeInvalidateRegion(                      \
            left, top, right, bottom);                                  \
}

#endif  //__ANIMATIONMACRO_H__
