#ifndef __InputFilterMacro_H__
#define __InputFilterMacro_H__

#define IInputFilter_METHODS_DECL()                                         \
    CARAPI Filter(                                                          \
        /* [in] */ ICharSequence* source,                                   \
        /* [in] */ Int32 start,                                             \
        /* [in] */ Int32 end,                                               \
        /* [in] */ ISpanned* dest,                                          \
        /* [in] */ Int32 dstart,                                            \
        /* [in] */ Int32 dend,                                              \
        /* [out] */ ICharSequence** ret);                                   \

#define IInputFilter_METHODS_IMPL(className, superClass, overRideClass)                 \
ECode className::Filter(                                                                \
    /* [in] */ ICharSequence* source,                                                   \
    /* [in] */ Int32 start,                                                             \
    /* [in] */ Int32 end,                                                               \
    /* [in] */ ISpanned* dest,                                                          \
    /* [in] */ Int32 dstart,                                                            \
    /* [in] */ Int32 dend,                                                              \
    /* [out] */ ICharSequence** ret)                                                    \
{                                                                                       \
    VALIDATE_NOT_NULL(ret);                                                             \
    AutoPtr<ICharSequence> res = superClass::Filter(source, start, end, dest, dstart, dend);  \
    *ret = res;                                                                         \
    INTERFACE_ADDREF(*ret);                                                             \
    return NOERROR;                                                                     \
}                                                                                       \


#endif  //__InputFilterMacro_H__