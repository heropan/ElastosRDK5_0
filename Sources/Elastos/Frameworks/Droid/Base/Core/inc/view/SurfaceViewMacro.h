#ifndef __SURFACEVIEWMACRO_H__
#define __SURFACEVIEWMACRO_H__

#include "ext/frameworkext.h"
using Elastos::Droid::View::IView;
using Elastos::Droid::View::ISurfaceView;
using Elastos::Droid::View::ISurfaceHolder;
using Elastos::Droid::View::IKeyEventCallback;


#define ISURFACEVIEW_METHODS_DECL()                                            \
    CARAPI GetHolder(                                                           \
        /* [out] */ ISurfaceHolder** holder);                                   \
                                                                                \
    CARAPI SetZOrderMediaOverlay(                                               \
        /* [in] */ Boolean isMediaOverlay);                                     \
                                                                                \
    CARAPI SetZOrderOnTop(                                                      \
        /* [in] */ Boolean onTop);                                              \
                                                                                \
    CARAPI SetSecure(                                                           \
        /* [in] */ Boolean isSecure);                                           \
                                                                                \
    CARAPI SetWindowType(                                                       \
        /* [in] */ Int32 type);                                                 \
                                                                                \
    CARAPI IsFixedSize(                                                         \
        /* [out] */ Boolean* size);                                             \
                                                                                \
    CARAPI SetCreateInputConnectionDelegate(                                    \
        /* [in] */ IView* view);                                                \
                                                                                \
    CARAPI SetKeyEventCallbackDelegate(                                         \
        /* [in] */ IKeyEventCallback* cb);

#define ISURFACEVIEW_METHODS_IMPL(className, superClass)                       \
ECode className::GetHolder(                                                     \
    /* [out] */ ISurfaceHolder** holder)                                        \
{                                                                               \
    VALIDATE_NOT_NULL(holder);                                                  \
    AutoPtr<ISurfaceHolder> sh = superClass::GetHolder();                       \
    *holder = sh.Get();                                                         \
    INTERFACE_ADDREF(*holder);                                                  \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetZOrderMediaOverlay(                                         \
    /* [in] */ Boolean isMediaOverlay)                                          \
{                                                                               \
    return superClass::SetZOrderMediaOverlay(isMediaOverlay);                  \
}                                                                               \
                                                                                \
ECode className::SetZOrderOnTop(                                                \
    /* [in] */ Boolean onTop)                                                   \
{                                                                               \
    return superClass::SetZOrderOnTop(onTop);                                  \
}                                                                               \
                                                                                \
ECode className::SetSecure(                                                     \
    /* [in] */ Boolean isSecure)                                                \
{                                                                               \
    return superClass::SetSecure(isSecure);                                    \
}                                                                               \
                                                                                \
ECode className::SetWindowType(                                                 \
    /* [in] */ Int32 type)                                                      \
{                                                                               \
    return superClass::SetWindowType(type);                                    \
}                                                                               \
                                                                                \
ECode className::IsFixedSize(                                                   \
    /* [out] */ Boolean* size)                                                  \
{                                                                               \
    VALIDATE_NOT_NULL(size);                                                    \
    *size = superClass::IsFixedSize();                                          \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetCreateInputConnectionDelegate(                              \
    /* [in] */ IView* view)                                                     \
{                                                                               \
    return superClass::SetCreateInputConnectionDelegate(view);                 \
}                                                                               \
                                                                                \
ECode className::SetKeyEventCallbackDelegate(                                   \
    /* [in] */ IKeyEventCallback* cb)                                           \
{                                                                               \
    return superClass::SetKeyEventCallbackDelegate(cb);                        \
}


#endif //__SURFACEVIEWMACRO_H__
