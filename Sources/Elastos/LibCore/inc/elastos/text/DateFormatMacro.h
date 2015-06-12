#ifndef __ELASTOS_TEXT_DATEFORMATMACRO_H__
#define __ELASTOS_TEXT_DATEFORMATMACRO_H__

#define DATEFORMAT_METHODS_DECL()                                                                    \
                                                                                                     \
    CARAPI FormatObject(                                                                             \
        /* [in] */ IInterface* object,                                                               \
        /* [out] */ String* value);                                                                  \
                                                                                                     \
    CARAPI FormatToCharacterIterator(                                                                \
        /* [in] */ IInterface* object,                                                               \
        /* [out] */ IAttributedCharacterIterator** charactorIterator);                         \
                                                                                                     \
    CARAPI ParseObject(                                                                              \
        /* [in] */ const String& string,                                                             \
        /* [out] */ IInterface** object);                                                            \
                                                                                                     \
    CARAPI FormatDate(                                                                               \
        /* [in] */ IDate* date,                                                                      \
        /* [out] */ String* result);                                                                 \
                                                                                                     \
    CARAPI GetCalendar(                                                                              \
        /* [out] */ ICalendar** calendar);                                                           \
                                                                                                     \
    CARAPI GetNumberFormat(                                                                          \
        /* [out] */ INumberFormat** format);                                                         \
                                                                                                     \
    CARAPI GetTimeZone(                                                                              \
        /* [out] */ ITimeZone** timezone);                                                           \
                                                                                                     \
    CARAPI IsLenient(                                                                                \
        /* [out] */ Boolean* isLenient);                                                             \
                                                                                                     \
    CARAPI Parse(                                                                                    \
        /* [in] */ const String& string,                                                             \
        /* [out] */ IDate** date);                                                                   \
                                                                                                     \
    CARAPI SetCalendar(                                                                              \
        /* [in] */ ICalendar* cal);                                                                  \
                                                                                                     \
    CARAPI SetLenient(                                                                               \
        /* [in] */ Boolean value);                                                                   \
                                                                                                     \
    CARAPI SetNumberFormat(                                                                          \
        /* [in] */ INumberFormat* format);                                                           \
                                                                                                     \
    CARAPI SetTimeZone(                                                                              \
        /* [in] */ ITimeZone* timezone);

#define DATEFORMAT_METHODS_IMPL(className, superClass)                                               \
                                                                                                     \
    ECode className::FormatObject(                                                                   \
        /* [in] */ IInterface* object,                                                               \
        /* [out] */ String* value)                                                                   \
    {                                                                                                \
        return superClass::FormatObject(object,value);                                               \
    }                                                                                                \
                                                                                                     \
    ECode className::FormatToCharacterIterator(                                                      \
        /* [in] */ IInterface* object,                                                               \
        /* [out] */ IAttributedCharacterIterator** charactorIterator)                          \
    {                                                                                                \
        return superClass::FormatToCharacterIterator(object,charactorIterator);                      \
    }                                                                                                \
                                                                                                     \
    ECode className::ParseObject(                                                                    \
        /* [in] */ const String& string,                                                             \
        /* [out] */ IInterface** object)                                                             \
    {                                                                                                \
        return superClass::ParseObject(string,object);                                               \
    }                                                                                                \
                                                                                                     \
    ECode className::FormatDate(                                                                     \
        /* [in] */ IDate* date,                                                                      \
        /* [out] */ String* result)                                                                  \
    {                                                                                                \
        return superClass::FormatDate(date,result);                                                  \
    }                                                                                                \
                                                                                                     \
    ECode className::GetCalendar(                                                                    \
        /* [out] */ ICalendar** calendar)                                                            \
    {                                                                                                \
        return superClass::GetCalendar(calendar);                                                    \
    }                                                                                                \
                                                                                                     \
    ECode className::GetNumberFormat(                                                                \
        /* [out] */ INumberFormat** format)                                                          \
    {                                                                                                \
        return superClass::GetNumberFormat(format);                                                  \
    }                                                                                                \
                                                                                                     \
    ECode className::GetTimeZone(                                                                    \
        /* [out] */ ITimeZone** timezone)                                                            \
    {                                                                                                \
        return superClass::GetTimeZone(timezone);                                                    \
    }                                                                                                \
                                                                                                     \
    ECode className::IsLenient(                                                                      \
        /* [out] */ Boolean* isLenient)                                                              \
    {                                                                                                \
        return superClass::IsLenient(isLenient);                                                     \
    }                                                                                                \
                                                                                                     \
    ECode className::Parse(                                                                          \
        /* [in] */ const String& string,                                                             \
        /* [out] */ IDate** date)                                                                    \
    {                                                                                                \
        return superClass::Parse(string,date);                                                       \
    }                                                                                                \
                                                                                                     \
    ECode className::SetCalendar(                                                                    \
        /* [in] */ ICalendar* cal)                                                                   \
    {                                                                                                \
        return superClass::SetCalendar(cal);                                                         \
    }                                                                                                \
                                                                                                     \
    ECode className::SetLenient(                                                                     \
        /* [in] */ Boolean value)                                                                    \
    {                                                                                                \
        return superClass::SetLenient(value);                                                        \
    }                                                                                                \
                                                                                                     \
    ECode className::SetNumberFormat(                                                                \
        /* [in] */ INumberFormat* format)                                                            \
    {                                                                                                \
        return superClass::SetNumberFormat(format);                                                  \
    }                                                                                                \
                                                                                                     \
    ECode className::SetTimeZone(                                                                    \
        /* [in] */ ITimeZone* timezone)                                                              \
    {                                                                                                \
        return superClass::SetTimeZone(timezone);                                                    \
    }                                                                                                \

#endif  //__ELASTOS_TEXT_DATEFORMATMACRO_H__