
#ifndef __SQLTIMEMACRO_H__
#define __SQLTIMEMACRO_H__

#define SQLTIME_METHODS_DECL()                                  \
                                                                \
    CARAPI IsAfter(                                             \
        /* [in] */ Elastos::Utility::IDate * indate,            \
        /* [out] */ Boolean * value);                           \
                                                                \
    CARAPI IsBefore(                                            \
        /* [in] */ Elastos::Utility::IDate * indate,            \
        /* [out] */ Boolean * value);                           \
                                                                \
    CARAPI Clone(                                               \
        /* [out] */ Elastos::Utility::IDate ** thedate);        \
                                                                \
    CARAPI CompareTo(                                           \
        /* [in] */ Elastos::Utility::IDate * indate,            \
        /* [out] */ Int32 * value);                             \
                                                                \
    CARAPI Equals(                                            \
        /* [in] */ Elastos::Utility::IDate * indate,            \
        /* [out] */ Boolean * value);                           \
                                                                \
    CARAPI Equals(                                              \
        /* [in] */ IInterface * indate,                         \
        /* [out] */ Boolean * value);                           \
                                                                \
    CARAPI GetHours(                                            \
        /* [out] */ Int32 * value);                             \
                                                                \
    CARAPI GetMinutes(                                          \
        /* [out] */ Int32 * value);                             \
                                                                \
    CARAPI GetSeconds(                                          \
        /* [out] */ Int32 * value);                             \
                                                                \
    CARAPI GetTime(                                             \
        /* [out] */ Int64 * value);                             \
                                                                \
    CARAPI GetTimezoneOffset(                                   \
        /* [out] */ Int32 * value);                             \
                                                                \
    CARAPI SetHours(                                            \
        /* [in] */ Int32 hour);                                 \
                                                                \
    CARAPI SetMinutes(                                          \
        /* [in] */ Int32 minute);                               \
                                                                \
    CARAPI SetSeconds(                                          \
        /* [in] */ Int32 second);                               \
                                                                \
    CARAPI ToGMTString(                                         \
        /* [out] */ String * str);                              \
                                                                \
    CARAPI ToLocaleString(                                      \
        /* [out] */ String * str);

#define SQLTIME_METHODS_IMPL(className, superClass)             \
                                                                \
ECode className::IsAfter(                                       \
        /* [in] */ Elastos::Utility::IDate * indate,            \
        /* [out] */ Boolean * value)                            \
{                                                               \
    *value = superClass::IsAfter(indate);                       \
    return NOERROR;                                             \
}                                                               \
                                                                \
ECode className::IsBefore(                                      \
        /* [in] */ Elastos::Utility::IDate * indate,            \
        /* [out] */ Boolean * value)                            \
{                                                               \
    *value = superClass::IsBefore(indate);                      \
    return NOERROR;                                             \
}                                                               \
                                                                \
ECode className::Clone(                                         \
        /* [out] */ Elastos::Utility::IDate ** thedate)         \
{                                                               \
    *thedate = superClass::Clone();                             \
    return NOERROR;                                             \
}                                                               \
                                                                \
ECode className::CompareTo(                                     \
        /* [in] */ Elastos::Utility::IDate * indate,            \
        /* [out] */ Int32 * value)                              \
{                                                               \
    *value = superClass::CompareTo(indate);                     \
    return NOERROR;                                             \
}                                                               \
                                                                \
ECode className::Equals(                                      \
        /* [in] */ Elastos::Utility::IDate * indate,            \
        /* [out] */ Boolean * value)                            \
{                                                               \
    *value = superClass::Equals(indate);                      \
    return NOERROR;                                             \
}                                                               \
                                                                \
ECode className::Equals(                                        \
        /* [in] */ IInterface * indate,                         \
        /* [out] */ Boolean * value)                            \
{                                                               \
    *value = superClass::Equals(indate);                        \
    return NOERROR;                                             \
}                                                               \
                                                                \
ECode className::GetHours(                                      \
        /* [out] */ Int32 * value)                              \
{                                                               \
    *value = superClass::GetHours();                            \
    return NOERROR;                                             \
}                                                               \
                                                                \
ECode className::GetMinutes(                                    \
        /* [out] */ Int32 * value)                              \
{                                                               \
    *value = superClass::GetMinutes();                          \
    return NOERROR;                                             \
}                                                               \
                                                                \
ECode className::GetSeconds(                                    \
        /* [out] */ Int32 * value)                              \
{                                                               \
    *value = superClass::GetSeconds();                          \
    return NOERROR;                                             \
}                                                               \
                                                                \
ECode className::GetTime(                                       \
        /* [out] */ Int64 * value)                              \
{                                                               \
    *value = superClass::GetTime();                             \
    return NOERROR;                                             \
}                                                               \
                                                                \
ECode className::GetTimezoneOffset(                             \
        /* [out] */ Int32 * value)                              \
{                                                               \
    *value = superClass::GetTimezoneOffset();                   \
    return NOERROR;                                             \
}                                                               \
                                                                \
ECode className::SetHours(                                      \
        /* [in] */ Int32 hour)                                  \
{                                                               \
    return superClass::SetHours(hour);                          \
}                                                               \
                                                                \
ECode className::SetMinutes(                                    \
        /* [in] */ Int32 minute)                                \
{                                                               \
    return superClass::SetMinutes(minute);                      \
}                                                               \
                                                                \
ECode className::SetSeconds(                                    \
        /* [in] */ Int32 second)                                \
{                                                               \
    return superClass::SetSeconds(second);                      \
}                                                               \
                                                                \
ECode className::ToGMTString(                                   \
        /* [out] */ String * str)                               \
{                                                               \
    *str = superClass::ToGMTString();                           \
    return NOERROR;                                             \
}                                                               \
                                                                \
ECode className::ToLocaleString(                                \
        /* [out] */ String * str)                               \
{                                                               \
    *str = superClass::ToLocaleString();                        \
    return NOERROR;                                             \
}                                                               \


#endif  //__SQLTIMEMACRO_H__
