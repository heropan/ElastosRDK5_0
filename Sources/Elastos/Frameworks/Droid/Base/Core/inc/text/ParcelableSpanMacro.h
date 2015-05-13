#ifndef __PARCELABLESPANMACRO_H__
#define __PARCELABLESPANMACRO_H__

#define IPARCELABLESPAN_METHODS_DECL()                                  \
    CARAPI DescribeContents(                                            \
        /* [out] */ Int32 * pRet);                                      \
                                                                        \
    CARAPI WriteToParcel(                                               \
        /* [in] */ IParcel * pDest);                                    \
                                                                        \
    CARAPI ReadFromParcel(                                              \
        /* [in] */ IParcel* source);                                    \
                                                                        \
    CARAPI GetSpanTypeId(                                               \
        /* [out] */ Int32 * pRet);                                      \


#define IPARCELABLESPAN_METHODS_IMPL(className, superClass, overRideClass)  \
ECode className::DescribeContents(                                          \
    /* [out] */ Int32 * pRet)                                               \
{                                                                           \
    VALIDATE_NOT_NULL(pRet);                                                \
    *pRet=superClass::DescribeContents();                                   \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::WriteToParcel(                                             \
    /* [in] */ IParcel * pDest)                                             \
{                                                                           \
    return superClass::WriteToParcel(pDest);                                \
}                                                                           \
                                                                            \
ECode className::ReadFromParcel(                                            \
    /* [in] */ IParcel* source)                                             \
{                                                                           \
    return superClass::ReadFromParcel(source);                              \
}                                                                           \
                                                                            \
ECode className::GetSpanTypeId(                                             \
    /* [out] */ Int32 * pRet)                                               \
{                                                                           \
    VALIDATE_NOT_NULL(pRet);                                                \
    *pRet=superClass::GetSpanTypeId();                                      \
    return NOERROR;                                                         \
}                                                                           \


#endif  //__PARCELABLESPANMACRO_H__