
#ifndef __PROGRESSDIALOGMACRO_H__
#define __PROGRESSDIALOGMACRO_H__

#include "app/ProgressDialog.h"
#include "app/AlertDialogMacro.h"

#define IPROGRESSDIALOG_METHODS_DECL()                          \
    CARAPI SetProgress(                                         \
            /* [in] */ Int32 value);                            \
                                                                \
    CARAPI SetSecondaryProgress(                                \
        /* [in] */ Int32 secondaryProgress);                    \
                                                                \
    CARAPI GetProgress(                                         \
        /* [out] */ Int32* value);                              \
                                                                \
    CARAPI GetSecondaryProgress(                                \
        /* [out] */ Int32* value);                              \
                                                                \
    CARAPI GetMax(                                              \
        /* [out] */ Int32* max);                                \
                                                                \
    CARAPI SetMax(                                              \
        /* [in] */ Int32 max);                                  \
                                                                \
    CARAPI IncrementProgressBy(                                 \
        /* [in] */ Int32 diff);                                 \
                                                                \
    CARAPI IncrementSecondaryProgressBy(                        \
        /* [in] */ Int32 diff);                                 \
                                                                \
    CARAPI SetProgressDrawable(                                 \
        /* [in] */ IDrawable* d);                               \
                                                                \
    CARAPI SetIndeterminateDrawable(                            \
        /* [in] */ IDrawable* d);                               \
                                                                \
    CARAPI SetIndeterminate(                                    \
        /* [in] */ Boolean indeterminate);                      \
                                                                \
    CARAPI IsIndeterminate(                                     \
        /* [out] */ Boolean* isIndeterminate);                  \
                                                                \
    CARAPI SetProgressStyle(                                    \
        /* [in] */ Int32 style);                                \
                                                                \
    CARAPI SetProgressNumberFormat(                             \
        /* [in] */ const String& format);                       \
                                                                \
    CARAPI SetProgressPercentFormat(                            \
        /* [in] */ INumberFormat* format);

#define IPROGRESSDIALOG_METHODS_IMPL(className, superClass)     \
ECode className::SetProgress(                                   \
    /* [in] */ Int32 value)                                     \
{                                                               \
    return superClass::SetProgress(value);                      \
}                                                               \
                                                                \
ECode className::SetSecondaryProgress(                          \
    /* [in] */ Int32 secondaryProgress)                         \
{                                                               \
    return superClass::SetSecondaryProgress(secondaryProgress); \
}                                                               \
                                                                \
ECode className::GetProgress(                                   \
    /* [out] */ Int32* value)                                   \
{                                                               \
   return superClass::GetProgress(value);                       \
}                                                               \
                                                                \
ECode className::GetSecondaryProgress(                          \
    /* [out] */ Int32* value)                                   \
{                                                               \
   return superClass::GetSecondaryProgress(value);              \
}                                                               \
                                                                \
ECode className::GetMax(                                        \
    /* [out] */ Int32* max)                                     \
{                                                               \
   return superClass::GetMax(max);                              \
}                                                               \
                                                                \
ECode className::SetMax(                                        \
    /* [in] */ Int32 max)                                       \
{                                                               \
    return superClass::SetMax(max);                             \
}                                                               \
                                                                \
ECode className::IncrementProgressBy(                           \
    /* [in] */ Int32 diff)                                      \
{                                                               \
    return superClass::IncrementProgressBy(diff);               \
}                                                               \
                                                                \
ECode className::IncrementSecondaryProgressBy(                  \
    /* [in] */ Int32 diff)                                      \
{                                                               \
    return superClass::IncrementSecondaryProgressBy(diff);      \
}                                                               \
                                                                \
ECode className::SetProgressDrawable(                           \
    /* [in] */ IDrawable* d)                                    \
{                                                               \
    return superClass::SetProgressDrawable(d);                  \
}                                                               \
                                                                \
ECode className::SetIndeterminateDrawable(                      \
    /* [in] */ IDrawable* d)                                    \
{                                                               \
    return superClass::SetIndeterminateDrawable(d);             \
}                                                               \
                                                                \
ECode className::SetIndeterminate(                              \
    /* [in] */ Boolean indeterminate)                           \
{                                                               \
    return superClass::SetIndeterminate(indeterminate);         \
}                                                               \
                                                                \
ECode className::IsIndeterminate(                               \
    /* [out] */ Boolean* isIndeterminate)                       \
{                                                               \
   return superClass::IsIndeterminate(isIndeterminate);         \
}                                                               \
                                                                \
ECode className::SetProgressStyle(                              \
    /* [in] */ Int32 style)                                     \
{                                                               \
    return superClass::SetProgressStyle(style);                 \
}                                                               \
                                                                \
ECode className::SetProgressNumberFormat(                       \
    /* [in] */ const String& format)                            \
{                                                               \
    return superClass::SetProgressNumberFormat(format);         \
}                                                               \
                                                                \
ECode className::SetProgressPercentFormat(                      \
    /* [in] */ INumberFormat* format)                           \
{                                                               \
    return superClass::SetProgressPercentFormat(format);        \
}

#endif // __PROGRESSDIALOGMACRO_H__
