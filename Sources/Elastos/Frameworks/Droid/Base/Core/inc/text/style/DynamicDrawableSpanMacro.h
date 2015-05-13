#ifndef __DynamicDrawableSpanMACRO_H__
#define __DynamicDrawableSpanMACRO_H__

#define IDynamicDrawableSpan_METHODS_DECL()                                  \
    CARAPI GetVerticalAlignment(                                             \
        /* [out] */ Int32* ret);                                             \
                                                                             \
    CARAPI GetDrawable(                                                      \
        /* [out] */ IDrawable** ret);                                        \


#define IDynamicDrawableSpan_METHODS_IMPL(className, superClass, overRideClass)  \
ECode className::GetVerticalAlignment(                                           \
    /* [out] */ Int32* ret)                                                      \
{                                                                                \
    VALIDATE_NOT_NULL(ret);                                                      \
    *ret=superClass::GetVerticalAlignment();                                     \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::GetDrawable(                                                    \
    /* [out] */ IDrawable** ret)                                                 \
{                                                                                \
    VALIDATE_NOT_NULL(ret);                                                      \
    AutoPtr<IDrawable> d = superClass::GetDrawable();                            \
    *ret = d;                                                                    \
    INTERFACE_ADDREF(*ret);                                                      \
    return NOERROR;                                                              \
}                                                                                \


#endif  //__DynamicDrawableSpanMACRO_H__