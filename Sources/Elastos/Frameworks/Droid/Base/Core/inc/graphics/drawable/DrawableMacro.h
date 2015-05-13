
#ifndef __DRAWABLEMACRO_H__
#define __DRAWABLEMACRO_H__

using Elastos::Droid::Graphics::PorterDuffMode;
using Elastos::Droid::Graphics::IInsets;
using Elastos::Droid::Graphics::IRegion;
using Elastos::Droid::Graphics::IColorFilter;
using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::Graphics::Drawable::IDrawableConstantState;

#define IDRAWABLE_METHODS_DECL()                                        \
    CARAPI Draw(                                                        \
        /* [in] */ ICanvas* canvas);                                    \
                                                                        \
    CARAPI SetBounds(                                                   \
        /* [in] */ Int32 left,                                          \
        /* [in] */ Int32 top,                                           \
        /* [in] */ Int32 right,                                         \
        /* [in] */ Int32 bottom);                                       \
                                                                        \
    CARAPI SetBoundsEx(                                                 \
        /* [in] */ IRect* bounds);                                      \
                                                                        \
    CARAPI CopyBounds(                                                  \
        /* [in] */ IRect* bounds);                                      \
                                                                        \
    CARAPI CopyBoundsEx(                                                \
        /* [out] */ IRect** rect);                                      \
                                                                        \
    CARAPI GetBounds(                                                   \
        /* [out] */ IRect** rect);                                      \
                                                                        \
    CARAPI SetChangingConfigurations(                                   \
        /* [in] */ Int32 configs);                                      \
                                                                        \
    CARAPI GetChangingConfigurations(                                   \
        /* [out] */ Int32* configs);                                    \
                                                                        \
    CARAPI SetDither(                                                   \
        /* [in] */ Boolean dither);                                     \
                                                                        \
    CARAPI SetFilterBitmap(                                             \
        /* [in] */ Boolean filter);                                     \
                                                                        \
    CARAPI SetCallback(                                                 \
        /* [in] */ IDrawableCallback* cb);                              \
                                                                        \
    CARAPI GetCallback(                                                 \
        /* [out] */ IDrawableCallback** cb);                            \
                                                                        \
    CARAPI InvalidateSelf();                                            \
                                                                        \
    CARAPI ScheduleSelf(                                                \
        /* [in] */ IRunnable* what,                                     \
        /* [in] */ Int32 when);                                         \
                                                                        \
    CARAPI UnscheduleSelf(                                              \
        /* [in] */ IRunnable* what);                                    \
                                                                        \
    CARAPI SetAlpha(                                                    \
        /* [in] */ Int32 alpha);                                        \
                                                                        \
    CARAPI SetColorFilter(                                              \
        /* [in] */ IColorFilter* cf);                                   \
                                                                        \
    CARAPI SetColorFilterEx(                                            \
        /* [in] */ Int32 color,                                         \
        /* [in] */ PorterDuffMode mode);                                \
                                                                        \
    CARAPI ClearColorFilter();                                          \
                                                                        \
    CARAPI IsStateful(                                                  \
        /* [out] */ Boolean* isStateful);                               \
                                                                        \
    CARAPI SetState(                                                    \
        /* [in] */ ArrayOf<Int32>* stateSet,                            \
        /* [out] */ Boolean* isStateful);                               \
                                                                        \
    CARAPI GetState(                                                    \
        /* [out, callee] */ ArrayOf<Int32>** stateSet);                 \
                                                                        \
    CARAPI JumpToCurrentState();                                        \
                                                                        \
    CARAPI GetCurrent(                                                  \
        /* [out] */ IDrawable** drawable);                              \
                                                                        \
    CARAPI SetLevel(                                                    \
        /* [in] */ Int32 level,                                         \
        /* [out] */ Boolean* changed);                                  \
                                                                        \
    CARAPI GetLevel(                                                    \
        /* [out] */ Int32* curLevel);                                   \
                                                                        \
    CARAPI SetVisible(                                                  \
        /* [in] */ Boolean visible,                                     \
        /* [in] */ Boolean restart,                                     \
        /* [out] */ Boolean* isDifferent);                              \
                                                                        \
    CARAPI IsVisible(                                                   \
        /* [out] */ Boolean* visible);                                  \
                                                                        \
    CARAPI GetOpacity(                                                  \
        /* [out] */ Int32* opacity);                                    \
                                                                        \
    CARAPI ResolveOpacity(                                              \
        /* [in] */ Int32 op1,                                           \
        /* [in] */ Int32 op2,                                           \
        /* [out] */ Int32* opacity);                                    \
                                                                        \
    CARAPI GetTransparentRegion(                                        \
        /* [out] */ IRegion** bounds);                                  \
                                                                        \
    CARAPI GetIntrinsicWidth(                                           \
        /* [out] */ Int32* width);                                      \
                                                                        \
    CARAPI GetLayoutDirection(                                          \
        /* [out] */ Int32* direction);                                  \
                                                                        \
    CARAPI SetLayoutDirection(                                          \
        /* [in] */ Int32 direction);                                    \
                                                                        \
    CARAPI GetLayoutInsets(                                             \
        /* [out] */ IInsets** set);                                     \
                                                                        \
    CARAPI GetIntrinsicHeight(                                          \
        /* [out] */ Int32* height);                                     \
                                                                        \
    CARAPI GetMinimumWidth(                                             \
        /* [out] */ Int32* width);                                      \
                                                                        \
    CARAPI GetMinimumHeight(                                            \
        /* [out] */ Int32* height);                                     \
                                                                        \
    CARAPI GetPadding(                                                  \
        /* [in] */ IRect* padding,                                      \
        /* [out] */ Boolean* isPadding);                                \
                                                                        \
    CARAPI Mutate(                                                      \
        /* [out] */ IDrawable** drawable);                              \
                                                                        \
    CARAPI Inflate(                                                     \
        /* [in] */ IResources* r,                                       \
        /* [in] */ IXmlPullParser* parser,                              \
        /* [in] */ IAttributeSet* attrs);                               \
                                                                        \
    CARAPI GetConstantState(                                            \
        /* [out] */ IDrawableConstantState** state);                    \
                                                                        \
    CARAPI SetResId(                                                    \
        /* [in] */ Int32 resId);                                       \
                                                                        \
    CARAPI GetResId(                                                    \
        /* [out] */ Int32* resId);

#define IDRAWABLE_METHODS_IMPL(className, superClass)                   \
ECode className::Draw(                                                  \
    /* [in] */ ICanvas* canvas)                                         \
{                                                                       \
    return superClass::Draw(canvas);                                    \
}                                                                       \
                                                                        \
ECode className::SetBounds(                                             \
    /* [in] */ Int32 left,                                              \
    /* [in] */ Int32 top,                                               \
    /* [in] */ Int32 right,                                             \
    /* [in] */ Int32 bottom)                                            \
{                                                                       \
    return superClass::SetBounds(left, top, right, bottom);             \
}                                                                       \
                                                                        \
ECode className::SetBoundsEx(                                           \
    /* [in] */ IRect* bounds)                                           \
{                                                                       \
    return superClass::SetBounds(bounds);                               \
}                                                                       \
                                                                        \
ECode className::CopyBounds(                                            \
    /* [in] */ IRect* bounds)                                           \
{                                                                       \
    return superClass::CopyBounds(bounds);                                \
}                                                                       \
                                                                        \
ECode className::CopyBoundsEx(                                          \
    /* [out] */ IRect** rect)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(rect);                                            \
    AutoPtr<IRect> temp = superClass::CopyBounds();                     \
    *rect = temp;                                                       \
    if (*rect) {                                                        \
        (*rect)->AddRef();                                              \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetBounds(                                             \
    /* [out] */ IRect** rect)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(rect);                                            \
    AutoPtr<IRect> temp = superClass::GetBounds();                      \
    *rect = temp;                                                       \
    if (*rect) {                                                        \
        (*rect)->AddRef();                                              \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetChangingConfigurations(                             \
    /* [in] */ Int32 configs)                                           \
{                                                                       \
    return superClass::SetChangingConfigurations(configs);              \
}                                                                       \
                                                                        \
ECode className::GetChangingConfigurations(                             \
    /* [out] */ Int32* configs)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(configs);                                         \
    *configs = superClass::GetChangingConfigurations();                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetDither(                                             \
    /* [in] */ Boolean dither)                                          \
{                                                                       \
    return superClass::SetDither(dither);                               \
}                                                                       \
                                                                        \
ECode className::SetFilterBitmap(                                       \
    /* [in] */ Boolean filter)                                          \
{                                                                       \
    return superClass::SetFilterBitmap(filter);                         \
}                                                                       \
                                                                        \
ECode className::SetCallback(                                           \
    /* [in] */ IDrawableCallback* cb)                                   \
{                                                                       \
    return superClass::SetCallback(cb);                                 \
}                                                                       \
                                                                        \
ECode className::GetCallback(                                           \
    /* [out] */ IDrawableCallback** cb)                                 \
{                                                                       \
    VALIDATE_NOT_NULL(cb);                                              \
    AutoPtr<IDrawableCallback> temp = superClass::GetCallback();  \
    *cb = temp;                                                         \
    if (*cb) {                                                         \
        (*cb)->AddRef();                                                \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::InvalidateSelf()                                       \
{                                                                       \
    return superClass::InvalidateSelf();                                \
}                                                                       \
                                                                        \
ECode className::ScheduleSelf(                                          \
    /* [in] */ IRunnable* what,                                         \
    /* [in] */ Int32 when)                                              \
{                                                                       \
    return superClass::ScheduleSelf(what, when);                        \
}                                                                       \
                                                                        \
ECode className::UnscheduleSelf(                                        \
    /* [in] */ IRunnable* what)                                         \
{                                                                       \
    return superClass::UnscheduleSelf(what);                            \
}                                                                       \
                                                                        \
ECode className::SetAlpha(                                              \
    /* [in] */ Int32 alpha)                                             \
{                                                                       \
    return superClass::SetAlpha(alpha);                                 \
}                                                                       \
                                                                        \
ECode className::SetColorFilter(                                        \
    /* [in] */ IColorFilter* cf)                                        \
{                                                                       \
    return superClass::SetColorFilter(cf);                              \
}                                                                       \
                                                                        \
ECode className::SetColorFilterEx(                                      \
    /* [in] */ Int32 color,                                             \
    /* [in] */ PorterDuffMode mode)                                     \
{                                                                       \
    return Drawable::SetColorFilter(color, mode);                       \
}                                                                       \
                                                                        \
ECode className::ClearColorFilter()                                     \
{                                                                       \
    return superClass::ClearColorFilter();                              \
}                                                                       \
                                                                        \
ECode className::IsStateful(                                            \
    /* [out] */ Boolean* isStateful)                                    \
{                                                                       \
    VALIDATE_NOT_NULL(isStateful);                                      \
    *isStateful = superClass::IsStateful();                             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetState(                                              \
    /* [in] */ ArrayOf<Int32>* stateSet,                                \
    /* [out] */ Boolean* isStateful)                                    \
{                                                                       \
    VALIDATE_NOT_NULL(isStateful);                                      \
    *isStateful = superClass::SetState(stateSet);                       \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetState(                                              \
    /* [out, callee] */ ArrayOf<Int32>** stateSet)                      \
{                                                                       \
    VALIDATE_NOT_NULL(stateSet);                                        \
    *stateSet = superClass::GetState();                                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::JumpToCurrentState()                                   \
{                                                                       \
    return superClass::JumpToCurrentState();                            \
}                                                                       \
                                                                        \
ECode className::GetCurrent(                                            \
    /* [out] */ IDrawable** drawable)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(drawable);                                        \
    AutoPtr<IDrawable> temp = superClass::GetCurrent();                 \
    *drawable = temp;                                                   \
    if (*drawable) {                                                    \
        (*drawable)->AddRef();                                          \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetLevel(                                              \
    /* [in] */ Int32 level,                                             \
    /* [out] */ Boolean* changed)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(changed);                                         \
    *changed = superClass::SetLevel(level);                             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLevel(                                              \
    /* [out] */ Int32* curLevel)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(curLevel);                                        \
    *curLevel = superClass::GetLevel();                                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetVisible(                                            \
    /* [in] */ Boolean visible,                                         \
    /* [in] */ Boolean restart,                                         \
    /* [out] */ Boolean* isDifferent)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(isDifferent);                                     \
    *isDifferent = superClass::SetVisible(visible, restart);            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsVisible(                                             \
    /* [out] */ Boolean* visible)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(visible);                                         \
    *visible = superClass::IsVisible();                                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetOpacity(                                            \
    /* [out] */ Int32* opacity)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(opacity);                                         \
    *opacity = superClass::GetOpacity();                                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::ResolveOpacity(                                        \
    /* [in] */ Int32 op1,                                               \
    /* [in] */ Int32 op2,                                               \
    /* [out] */ Int32* opacity)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(opacity);                                         \
    *opacity = superClass::ResolveOpacity(op1, op2);                    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetTransparentRegion(                                  \
    /* [out] */ IRegion** bounds)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(bounds);                                          \
    AutoPtr<IRegion> temp = superClass::GetTransparentRegion();         \
    *bounds = temp;                                                     \
    if (*bounds) {                                                      \
        (*bounds)->AddRef();                                            \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetIntrinsicWidth(                                     \
    /* [out] */ Int32* width)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(width);                                           \
    *width = superClass::GetIntrinsicWidth();                           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLayoutDirection(                                    \
    /* [out] */ Int32* direction)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(direction);                                       \
    *direction = superClass::GetLayoutDirection();                      \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetLayoutDirection(                                    \
    /* [in] */ Int32 direction)                                         \
{                                                                       \
    superClass::SetLayoutDirection(direction);                          \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLayoutInsets(                                       \
    /* [out] */ IInsets** set)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(set);                                             \
    *set = superClass::GetLayoutInsets();                               \
    if (*set != NULL) {                                                 \
        (*set)->AddRef();                                               \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetIntrinsicHeight(                                    \
    /* [out] */ Int32* height)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(height);                                          \
    *height = superClass::GetIntrinsicHeight();                         \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetMinimumWidth(                                       \
    /* [out] */ Int32* width)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(width);                                           \
    *width = superClass::GetMinimumWidth();                             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetMinimumHeight(                                      \
    /* [out] */ Int32* height)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(height);                                          \
    *height = superClass::GetMinimumHeight();                           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetPadding(                                            \
    /* [in] */ IRect* padding,                                          \
    /* [out] */ Boolean* isPadding)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(isPadding);                                       \
    *isPadding = superClass::GetPadding(padding);                       \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::Mutate(                                                \
    /* [out] */ IDrawable** drawable)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(drawable);                                        \
    AutoPtr<IDrawable> temp = superClass::Mutate();                     \
    *drawable = temp;                                                   \
    if (*drawable) {                                                    \
        (*drawable)->AddRef();                                          \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::Inflate(                                               \
    /* [in] */ IResources* r,                                           \
    /* [in] */ IXmlPullParser* parser,                                  \
    /* [in] */ IAttributeSet* attrs)                                    \
{                                                                       \
    return superClass::Inflate(r, parser, attrs);                       \
}                                                                       \
                                                                        \
ECode className::GetConstantState(                                      \
    /* [out] */ IDrawableConstantState** state)                         \
{                                                                       \
    VALIDATE_NOT_NULL(state);                                           \
    AutoPtr<IDrawableConstantState> temp = superClass::GetConstantState();\
    *state = temp;                                                      \
    if (*state) {                                                       \
        (*state)->AddRef();                                             \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetResId(                                              \
    /* [in] */ Int32 resId)                                             \
{                                                                       \
    return superClass::SetResId(resId);                                 \
}                                                                       \
                                                                        \
ECode className::GetResId(                                              \
    /* [out] */ Int32* resId)                                           \
{                                                                       \
    *resId = superClass::GetResId();                                    \
    return NOERROR;                                                     \
}

#define IDRAWABLECALLBACK_METHODS_DECL()                                \
    CARAPI InvalidateDrawable(                                          \
        /* [in] */ IDrawable* drawable);                                \
                                                                        \
    CARAPI ScheduleDrawable(                                            \
        /* [in] */ IDrawable* who,                                      \
        /* [in] */ IRunnable* what,                                     \
        /* [in] */ Int64 when);                                         \
                                                                        \
    CARAPI UnscheduleDrawable(                                          \
        /* [in] */ IDrawable* who,                                      \
        /* [in] */ IRunnable* what);

#define IDRAWABLECALLBACK_METHODS_IMPL(className, superClass)           \
ECode className::InvalidateDrawable(                                    \
    /* [in] */ IDrawable* drawable)                                     \
{                                                                       \
    return superClass::InvalidateDrawable(drawable);                    \
}                                                                       \
                                                                        \
ECode className::ScheduleDrawable(                                      \
    /* [in] */ IDrawable* who,                                          \
    /* [in] */ IRunnable* what,                                         \
    /* [in] */ Int64 when)                                              \
{                                                                       \
    return superClass::ScheduleDrawable(who, what, when);               \
}                                                                       \
                                                                        \
ECode className::UnscheduleDrawable(                                    \
    /* [in] */ IDrawable* who,                                          \
    /* [in] */ IRunnable* what)                                         \
{                                                                       \
    return superClass::UnscheduleDrawable(who, what);                   \
}

#endif //__DRAWABLEMACRO_H__
