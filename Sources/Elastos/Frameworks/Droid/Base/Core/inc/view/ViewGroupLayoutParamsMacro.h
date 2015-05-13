#ifndef __VIEWGROUPLAYOUTPARAMSMACRO_H__
#define __VIEWGROUPLAYOUTPARAMSMACRO_H__

#include "ext/frameworkext.h"
using Elastos::Droid::View::IView;
using Elastos::Droid::View::Animation::IAnimationParameters;
using Elastos::Droid::Graphics::ICanvas;

#define IVIEWGROUPLP_METHODS_DECL()                                     \
CARAPI SetWidth(                                                        \
    /* [in] */ Int32 width);                                            \
                                                                        \
CARAPI SetHeight(                                                       \
    /* [in] */ Int32 height);                                           \
                                                                        \
CARAPI GetWidth(                                                        \
    /* [out] */ Int32* width);                                          \
                                                                        \
CARAPI GetHeight(                                                       \
    /* [out] */ Int32* height);                                         \
                                                                        \
CARAPI OnDebugDraw(                                                     \
    /* [in] */ IView* view,                                             \
    /* [in] */ ICanvas* canvas);                                        \
                                                                        \
CARAPI ResolveLayoutDirection(                                          \
    /* [in] */ Int32 layoutDirection);                                  \
                                                                        \
CARAPI SetLayoutAnimationParameters(                                    \
    /* [in] */ IAnimationParameters* ap);                               \
                                                                        \
CARAPI GetLayoutAnimationParameters(                                    \
    /* [out] */ IAnimationParameters** ap);

#define IVIEWGROUPMARGINLP_METHODS_DECL()                               \
CARAPI GetLeftMargin(                                                   \
    /* [out] */ Int32* leftMargin);                                     \
                                                                        \
CARAPI SetLeftMargin(                                                   \
    /* [in] */ Int32 leftMargin);                                       \
                                                                        \
CARAPI GetTopMargin(                                                    \
    /* [out] */ Int32* topMargin);                                      \
                                                                        \
CARAPI SetTopMargin(                                                    \
    /* [in] */ Int32 topMargin);                                        \
                                                                        \
CARAPI GetRightMargin(                                                  \
    /* [out] */ Int32* rightMargin);                                    \
                                                                        \
CARAPI SetRightMargin(                                                  \
    /* [in] */ Int32 rightMargin);                                      \
                                                                        \
CARAPI GetBottomMargin(                                                 \
    /* [out] */ Int32* bottomMargin);                                   \
                                                                        \
CARAPI SetBottomMargin(                                                 \
    /* [in] */ Int32 bottomMargin);                                     \
                                                                        \
CARAPI SetMargins(                                                      \
    /* [in] */ Int32 left,                                              \
    /* [in] */ Int32 top,                                               \
    /* [in] */ Int32 right,                                             \
    /* [in] */ Int32 bottom);                                           \
                                                                        \
CARAPI GetMargins(                                                      \
    /* [out] */ Int32* left,                                            \
    /* [out] */ Int32* top,                                             \
    /* [out] */ Int32* right,                                           \
    /* [out] */ Int32* bottom);                                         \
                                                                        \
CARAPI SetMarginsRelative(                                              \
    /* [in] */ Int32 start,                                             \
    /* [in] */ Int32 top,                                               \
    /* [in] */ Int32 end,                                               \
    /* [in] */ Int32 bottom);                                           \
                                                                        \
CARAPI SetMarginStart(                                                  \
    /* [in] */ Int32 start);                                            \
                                                                        \
CARAPI GetMarginStart(                                                  \
    /* [out] */ Int32* startMargin);                                    \
                                                                        \
CARAPI SetMarginEnd(                                                    \
    /* [in] */ Int32 end);                                              \
                                                                        \
CARAPI GetMarginEnd(                                                    \
    /* [out] */ Int32* endMargin);                                      \
                                                                        \
CARAPI IsMarginRelative(                                                \
    /* [out] */ Boolean* set);                                          \
                                                                        \
CARAPI SetLayoutDirection(                                              \
    /* [in] */ Int32 layoutDirection);                                  \
                                                                        \
CARAPI GetLayoutDirection(                                              \
    /* [out] */ Int32* layoutDirection);                                \
                                                                        \
CARAPI IsLayoutRtl(                                                     \
    /* [out] */ Boolean* rtl);


#define IVIEWGROUPLP_METHODS_IMPL(className, superClass)                \
ECode className::SetWidth(                                              \
    /* [in] */ Int32 width)                                             \
{                                                                       \
    return superClass::SetWidth(width);                                 \
}                                                                       \
                                                                        \
ECode className::SetHeight(                                             \
    /* [in] */ Int32 height)                                            \
{                                                                       \
    return superClass::SetHeight(height);                               \
}                                                                       \
                                                                        \
ECode className::GetWidth(                                              \
    /* [out] */ Int32* width)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(width);                                           \
    *width = superClass::GetWidth();                                    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetHeight(                                             \
    /* [out] */ Int32* height)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(height);                                          \
    *height = superClass::GetHeight();                                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetLayoutAnimationParameters(                          \
    /* [in] */ IAnimationParameters* ap)                                \
{                                                                       \
    return superClass::SetLayoutAnimationParameters(ap);                \
}                                                                       \
                                                                        \
ECode className::GetLayoutAnimationParameters(                          \
    /* [out] */ IAnimationParameters** ap)                              \
{                                                                       \
    VALIDATE_NOT_NULL(ap);                                              \
    return superClass::GetLayoutAnimationParameters(ap);                \
}                                                                       \
                                                                        \
ECode className::OnDebugDraw(                                           \
    /* [in] */ IView* view,                                             \
    /* [in] */ ICanvas* canvas)                                         \
{                                                                       \
    return superClass::OnDebugDraw(view, canvas);                       \
}                                                                       \
                                                                        \
ECode className::ResolveLayoutDirection(                                \
    /* [in] */ Int32 layoutDirection)                                   \
{                                                                       \
    return superClass::ResolveLayoutDirection(layoutDirection);         \
}


#define IVIEWGROUPMARGINLP_METHODS_IMPL(className, superClass)          \
ECode className::GetLeftMargin(                                         \
    /* [out] */ Int32* leftMargin)                                      \
{                                                                       \
    return superClass::GetLeftMargin(leftMargin);                       \
}                                                                       \
                                                                        \
ECode className::SetLeftMargin(                                         \
    /* [in] */ Int32 leftMargin)                                        \
{                                                                       \
    return superClass::SetLeftMargin(leftMargin);                       \
}                                                                       \
                                                                        \
ECode className::GetTopMargin(                                          \
    /* [out] */ Int32* topMargin)                                       \
{                                                                       \
    return superClass::GetTopMargin(topMargin);                         \
}                                                                       \
                                                                        \
ECode className::SetTopMargin(                                          \
    /* [in] */ Int32 topMargin)                                         \
{                                                                       \
    return superClass::SetTopMargin(topMargin);                         \
}                                                                       \
                                                                        \
ECode className::GetRightMargin(                                        \
    /* [out] */ Int32* rightMargin)                                     \
{                                                                       \
    return superClass::GetRightMargin(rightMargin);                     \
}                                                                       \
                                                                        \
ECode className::SetRightMargin(                                        \
    /* [in] */ Int32 rightMargin)                                       \
{                                                                       \
    return superClass::SetRightMargin(rightMargin);                     \
}                                                                       \
                                                                        \
ECode className::GetBottomMargin(                                       \
    /* [out] */ Int32* bottomMargin)                                    \
{                                                                       \
    return superClass::GetBottomMargin(bottomMargin);                   \
}                                                                       \
                                                                        \
ECode className::SetBottomMargin(                                       \
    /* [in] */ Int32 bottomMargin)                                      \
{                                                                       \
    return superClass::SetBottomMargin(bottomMargin);                   \
}                                                                       \
                                                                        \
ECode className::SetMargins(                                            \
    /* [in] */ Int32 left,                                              \
    /* [in] */ Int32 top,                                               \
    /* [in] */ Int32 right,                                             \
    /* [in] */ Int32 bottom)                                            \
{                                                                       \
    return superClass::SetMargins(left, top, right, bottom);            \
}                                                                       \
                                                                        \
ECode className::GetMargins(                                            \
    /* [out] */ Int32* left,                                            \
    /* [out] */ Int32* top,                                             \
    /* [out] */ Int32* right,                                           \
    /* [out] */ Int32* bottom)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(left);                                            \
    VALIDATE_NOT_NULL(top);                                             \
    VALIDATE_NOT_NULL(right);                                           \
    VALIDATE_NOT_NULL(bottom);                                          \
    return superClass::GetMargins(left, top, right, bottom);            \
}                                                                       \
                                                                        \
ECode className::SetMarginsRelative(                                    \
    /* [in] */ Int32 start,                                             \
    /* [in] */ Int32 top,                                               \
    /* [in] */ Int32 end,                                               \
    /* [in] */ Int32 bottom)                                            \
{                                                                       \
    return superClass::SetMarginsRelative(start, top, end, bottom);     \
}                                                                       \
                                                                        \
ECode className::SetMarginStart(                                        \
    /* [in] */ Int32 start)                                             \
{                                                                       \
    return superClass::SetMarginStart(start);                           \
}                                                                       \
                                                                        \
ECode className::GetMarginStart(                                        \
    /* [out] */ Int32* startMargin)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(startMargin);                                     \
    *startMargin = superClass::GetMarginStart();                        \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetMarginEnd(                                          \
    /* [in] */ Int32 end)                                               \
{                                                                       \
    return SetMarginEnd(end);                                           \
}                                                                       \
                                                                        \
ECode className::GetMarginEnd(                                          \
    /* [out] */ Int32* endMargin)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(endMargin);                                       \
    *endMargin = superClass::GetMarginEnd();                            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsMarginRelative(                                      \
    /* [out] */ Boolean* set)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(set);                                             \
    *set = superClass::IsMarginRelative();                              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetLayoutDirection(                                    \
    /* [in] */ Int32 layoutDirection)                                   \
{                                                                       \
    return superClass::SetLayoutDirection(layoutDirection);             \
}                                                                       \
                                                                        \
ECode className::GetLayoutDirection(                                    \
    /* [out] */ Int32* layoutDirection)                                 \
{                                                                       \
    VALIDATE_NOT_NULL(layoutDirection);                                 \
    *layoutDirection = superClass::GetLayoutDirection();                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsLayoutRtl(                                           \
    /* [out] */ Boolean* rtl)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(rtl);                                             \
    *rtl = superClass::IsLayoutRtl();                                   \
                                                                        \
    return NOERROR;                                                     \
}


#endif //__VIEWGROUPLAYOUTPARAMSMACRO_H__
