
#ifndef __SQLTIMESTAMPMACRO_H__
#define __SQLTIMESTAMPMACRO_H__

#define SQLTIMESTAMP_METHODS_DECL()                                  \
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
    CARAPI Equals(                                              \
        /* [in] */ Elastos::Utility::IDate * indate,            \
        /* [out] */ Boolean * value);                           \
                                                                \
    CARAPI GetDate(                                             \
        /* [out] */ Int32 * value);                             \
                                                                \
    CARAPI GetDay(                                              \
        /* [out] */ Int32 * value);                             \
                                                                \
    CARAPI GetHours(                                            \
        /* [out] */ Int32 * value);                             \
                                                                \
    CARAPI GetMinutes(                                          \
        /* [out] */ Int32 * value);                             \
                                                                \
    CARAPI GetMonth(                                            \
        /* [out] */ Int32 * value);                             \
                                                                \
    CARAPI GetSeconds(                                          \
        /* [out] */ Int32 * value);                             \
                                                                \
    CARAPI GetTimezoneOffset(                                   \
        /* [out] */ Int32 * value);                             \
                                                                \
    CARAPI GetYear(                                             \
        /* [out] */ Int32 * value);                             \
                                                                \
    CARAPI SetDate(                                             \
        /* [in] */ Int32 day);                                  \
                                                                \
    CARAPI SetHours(                                            \
        /* [in] */ Int32 hour);                                 \
                                                                \
    CARAPI SetMinutes(                                          \
        /* [in] */ Int32 minute);                               \
                                                                \
    CARAPI SetMonth(                                            \
        /* [in] */ Int32 month);                                \
                                                                \
    CARAPI SetSeconds(                                          \
        /* [in] */ Int32 second);                               \
                                                                \
    CARAPI SetYear(                                             \
        /* [in] */ Int32 year);                                 \
                                                                \
    CARAPI ToGMTString(                                         \
        /* [out] */ String * str);                              \
                                                                \
    CARAPI ToLocaleString(                                      \
        /* [out] */ String * str);

#define SQLTIMESTAMP_METHODS_IMPL(className, superClass)        \
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
ECode className::Equals(                                        \
        /* [in] */ Elastos::Utility::IDate * indate,            \
        /* [out] */ Boolean * value)                            \
{                                                               \
    *value = superClass::Equals(indate);                        \
    return NOERROR;                                             \
}                                                               \
                                                                \
ECode className::GetDate(                                       \
        /* [out] */ Int32 * value)                              \
{                                                               \
    *value = superClass::GetDate();                             \
    return NOERROR;                                             \
}                                                               \
                                                                \
ECode className::GetDay(                                        \
        /* [out] */ Int32 * value)                              \
{                                                               \
    *value = superClass::GetDay();                              \
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
ECode className::GetMonth(                                      \
        /* [out] */ Int32 * value)                              \
{                                                               \
    *value = superClass::GetMonth();                            \
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
ECode className::GetTimezoneOffset(                             \
        /* [out] */ Int32 * value)                              \
{                                                               \
    *value = superClass::GetTimezoneOffset();                   \
    return NOERROR;                                             \
}                                                               \
                                                                \
ECode className::GetYear(                                       \
        /* [out] */ Int32 * value)                              \
{                                                               \
    *value = superClass::GetYear();                             \
    return NOERROR;                                             \
}                                                               \
                                                                \
ECode className::SetDate(                                       \
        /* [in] */ Int32 day)                                   \
{                                                               \
    return superClass::SetDate(day);                            \
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
ECode className::SetMonth(                                      \
        /* [in] */ Int32 month)                                 \
{                                                               \
    return superClass::SetMonth(month);                         \
}                                                               \
                                                                \
ECode className::SetSeconds(                                    \
        /* [in] */ Int32 second)                                \
{                                                               \
    return superClass::SetSeconds(second);                      \
}                                                               \
                                                                \
ECode className::SetYear(                                       \
        /* [in] */ Int32 year)                                  \
{                                                               \
    return superClass::SetYear(year);                           \
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

#endif  //__SQLTIMESTAMPMACRO_H__
