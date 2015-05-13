
#ifndef __SQLDATEMACRO_H__
#define __SQLDATEMACRO_H__

#define SQLDATE_METHODS_DECL()                                  \
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
    CARAPI EqualsEx(                                            \
        /* [in] */ Elastos::Utility::IDate * indate,            \
        /* [out] */ Boolean * value);                           \
                                                                \
    CARAPI Equals(                                              \
        /* [in] */ IInterface * indate,                         \
        /* [out] */ Boolean * value);                           \
                                                                \
    CARAPI GetDate(                                             \
        /* [out] */ Int32 * value);                             \
                                                                \
    CARAPI GetDay(                                              \
        /* [out] */ Int32 * value);                             \
                                                                \
    CARAPI GetMonth(                                            \
        /* [out] */ Int32 * value);                             \
                                                                \
    CARAPI GetTime(                                             \
        /* [out] */ Int64 * value);                             \
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
    CARAPI SetMonth(                                            \
        /* [in] */ Int32 month);                                \
                                                                \
    CARAPI SetYear(                                             \
        /* [in] */ Int32 year);                                 \
                                                                \
    CARAPI ToGMTString(                                         \
        /* [out] */ String * str);                              \
                                                                \
    CARAPI ToLocaleString(                                      \
        /* [out] */ String * str);

#define SQLDATE_METHODS_IMPL(className, superClass)             \
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
ECode className::EqualsEx(                                      \
        /* [in] */ Elastos::Utility::IDate * indate,            \
        /* [out] */ Boolean * value)                            \
{                                                               \
    *value = superClass::EqualsEx(indate);                      \
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
ECode className::GetMonth(                                      \
        /* [out] */ Int32 * value)                              \
{                                                               \
    *value = superClass::GetMonth();                            \
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
ECode className::SetMonth(                                      \
        /* [in] */ Int32 month)                                 \
{                                                               \
    return superClass::SetMonth(month);                         \
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


#endif  //__SQLDATEMACRO_H__
