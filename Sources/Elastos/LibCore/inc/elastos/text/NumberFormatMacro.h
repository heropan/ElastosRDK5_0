#ifndef __NUMBERFORMATMACRO_H__
#define __NUMBERFORMATMACRO_H__

#define NUMBERFORMAT_METHODS_DECL()                                                 \
                                                                                    \
    CARAPI FormatObject(                                                            \
        /* [in] */ IInterface* object,                                              \
        /* [out] */ String* value);                                                 \
                                                                                    \
    CARAPI FormatObjectEx(                                                          \
        /* [in] */ IInterface* object,                                              \
        /* [in] */ IStringBuffer * buffer,                                          \
        /* [in] */ IFieldPosition* field,                                     \
        /* [out] */ IStringBuffer ** value);                                        \
                                                                                    \
    CARAPI FormatToCharacterIterator(                                               \
        /* [in] */ IInterface* object,                                              \
        /* [out] */ IAttributedCharacterIterator** charactorIterator);        \
                                                                                    \
    CARAPI ParseObject(                                                             \
        /* [in] */ const String& string,                                            \
        /* [out] */ IInterface** object);                                           \
                                                                                    \
    CARAPI ParseObjectEx(                                                           \
        /* [in] */ const String& string,                                            \
        /* [in] */ IParsePosition* position,                                  \
        /* [out] */ IInterface** object);                                           \
                                                                                    \
    CARAPI FormatDouble(                                                            \
        /* [in] */ Double value,                                                    \
        /* [out] */ String* format);                                                \
                                                                                    \
    CARAPI FormatInt64(                                                             \
        /* [in] */ Int64 value,                                                     \
        /* [out] */ String* format);                                                \
                                                                                    \
    CARAPI GetCurrency(                                                             \
        /* [out] */ ICurrency** currency);                                          \
                                                                                    \
    CARAPI GetMaximumFractionDigits(                                                \
        /* [out] */ Int32* max);                                                    \
                                                                                    \
    CARAPI GetMaximumIntegerDigits(                                                 \
        /* [out] */ Int32* max);                                                    \
                                                                                    \
    CARAPI GetMinimumFractionDigits(                                                \
        /* [out] */ Int32* min);                                                    \
                                                                                    \
    CARAPI GetMinimumIntegerDigits(                                                 \
        /* [out] */ Int32* min);                                                    \
                                                                                    \
    CARAPI IsGroupingUsed(                                                          \
        /* [out] */ Boolean* groupingUsed);                                         \
                                                                                    \
    CARAPI IsParseIntegerOnly(                                                      \
        /* [out] */ Boolean* parseIntegerOnly);                                     \
                                                                                    \
    CARAPI Parse(                                                                   \
        /* [in] */ const String& string,                                            \
        /* [out] */ INumber** value);                                               \
                                                                                    \
    CARAPI SetCurrency(                                                             \
        /* [in] */ ICurrency* currency);                                            \
                                                                                    \
    CARAPI SetGroupingUsed(                                                         \
        /* [in] */ Boolean value);                                                  \
                                                                                    \
    CARAPI SetMaximumFractionDigits(                                                \
        /* [in] */ Int32 value);                                                    \
                                                                                    \
    CARAPI SetMaximumIntegerDigits(                                                 \
        /* [in] */ Int32 value);                                                    \
                                                                                    \
    CARAPI SetMinimumFractionDigits(                                                \
        /* [in] */ Int32 value);                                                    \
                                                                                    \
    CARAPI SetMinimumIntegerDigits(                                                 \
        /* [in] */ Int32 value);                                                    \
                                                                                    \
    CARAPI SetParseIntegerOnly(                                                     \
        /* [in] */ Boolean value);                                                  \
                                                                                    \
    CARAPI GetRoundingMode(                                                         \
        /* [out] */ RoundingMode * roundingMode);                                   \
                                                                                    \
    CARAPI SetRoundingMode(                                                         \
        /* [in] */ RoundingMode roundingMode);

#define NUMBERFORMAT_METHODS_IMPL(className, superClass)                            \
                                                                                    \
    ECode className::FormatObject(                                                  \
            /* [in] */ IInterface* object,                                          \
            /* [out] */ String* value)                                              \
    {                                                                               \
        return superClass::FormatObject(object,value);                              \
    }                                                                               \
                                                                                    \
    ECode className::FormatObjectEx(                                                \
            /* [in] */ IInterface* object,                                          \
            /* [in] */ IStringBuffer * buffer,                                      \
            /* [in] */ IFieldPosition* field,                                 \
            /* [out] */ IStringBuffer ** value)                                     \
    {                                                                               \
        return superClass::FormatObjectEx(object,buffer,field,value);               \
    }                                                                               \
                                                                                    \
    ECode className::FormatToCharacterIterator(                                     \
            /* [in] */ IInterface* object,                                          \
            /* [out] */ IAttributedCharacterIterator** charactorIterator)     \
    {                                                                               \
        return superClass::FormatToCharacterIterator(object,charactorIterator);     \
    }                                                                               \
                                                                                    \
    ECode className::ParseObject(                                                   \
            /* [in] */ const String& string,                                        \
            /* [out] */ IInterface** object)                                        \
    {                                                                               \
        return superClass::ParseObject(string,object);                              \
    }                                                                               \
                                                                                    \
    ECode className::ParseObjectEx(                                                 \
            /* [in] */ const String& string,                                        \
            /* [in] */ IParsePosition* position,                              \
            /* [out] */ IInterface** object)                                        \
    {                                                                               \
        return superClass::ParseObjectEx(string,position,object);                   \
    }                                                                               \
                                                                                    \
    ECode className::FormatDouble(                                                  \
            /* [in] */ Double value,                                                \
            /* [out] */ String* format)                                             \
    {                                                                               \
        return superClass::FormatDouble(value,format);                              \
    }                                                                               \
                                                                                    \
    ECode className::FormatInt64(                                                   \
            /* [in] */ Int64 value,                                                 \
            /* [out] */ String* format)                                             \
    {                                                                               \
        return superClass::FormatInt64(value,format);                               \
    }                                                                               \
                                                                                    \
    ECode className::GetCurrency(                                                   \
            /* [out] */ ICurrency** currency)                                       \
    {                                                                               \
        return superClass::GetCurrency(currency);                                   \
    }                                                                               \
                                                                                    \
    ECode className::GetMaximumFractionDigits(                                      \
            /* [out] */ Int32* max)                                                 \
    {                                                                               \
        return superClass::GetMaximumFractionDigits(max);                           \
    }                                                                               \
                                                                                    \
    ECode className::GetMaximumIntegerDigits(                                       \
            /* [out] */ Int32* max)                                                 \
    {                                                                               \
        return superClass::GetMaximumIntegerDigits(max);                            \
    }                                                                               \
                                                                                    \
    ECode className::GetMinimumFractionDigits(                                      \
            /* [out] */ Int32* min)                                                 \
    {                                                                               \
        return superClass::GetMinimumFractionDigits(min);                           \
    }                                                                               \
                                                                                    \
    ECode className::GetMinimumIntegerDigits(                                       \
            /* [out] */ Int32* min)                                                 \
    {                                                                               \
        return superClass::GetMinimumIntegerDigits(min);                            \
    }                                                                               \
                                                                                    \
    ECode className::IsGroupingUsed(                                                \
            /* [out] */ Boolean* groupingUsed)                                      \
    {                                                                               \
        return superClass::IsGroupingUsed(groupingUsed);                            \
    }                                                                               \
                                                                                    \
    ECode className::IsParseIntegerOnly(                                            \
            /* [out] */ Boolean* parseIntegerOnly)                                  \
    {                                                                               \
        return superClass::IsParseIntegerOnly(parseIntegerOnly);                    \
    }                                                                               \
                                                                                    \
    ECode className::Parse(                                                         \
            /* [in] */ const String& string,                                        \
            /* [out] */ INumber** value)                                            \
    {                                                                               \
        return superClass::Parse(string,value);                                     \
    }                                                                               \
    ECode className::SetCurrency(                                                   \
            /* [in] */ ICurrency* currency)                                         \
    {                                                                               \
        return superClass::SetCurrency(currency);                                   \
    }                                                                               \
                                                                                    \
    ECode className::SetGroupingUsed(                                               \
            /* [in] */ Boolean value)                                               \
    {                                                                               \
        return superClass::SetGroupingUsed(value);                                  \
    }                                                                               \
                                                                                    \
    ECode className::SetMaximumFractionDigits(                                      \
            /* [in] */ Int32 value)                                                 \
    {                                                                               \
        return superClass::SetMaximumFractionDigits(value);                         \
    }                                                                               \
                                                                                    \
    ECode className::SetMaximumIntegerDigits(                                       \
            /* [in] */ Int32 value)                                                 \
    {                                                                               \
        return superClass::SetMaximumIntegerDigits(value);                          \
    }                                                                               \
                                                                                    \
    ECode className::SetMinimumFractionDigits(                                      \
            /* [in] */ Int32 value)                                                 \
    {                                                                               \
        return superClass::SetMinimumFractionDigits(value);                         \
    }                                                                               \
                                                                                    \
    ECode className::SetMinimumIntegerDigits(                                       \
            /* [in] */ Int32 value)                                                 \
    {                                                                               \
        return superClass::SetMinimumIntegerDigits(value);                          \
    }                                                                               \
                                                                                    \
    ECode className::SetParseIntegerOnly(                                           \
            /* [in] */ Boolean value)                                               \
    {                                                                               \
        return superClass::SetParseIntegerOnly(value);                              \
    }                                                                               \
                                                                                    \
    ECode className::GetRoundingMode(                                               \
            /* [out] */ RoundingMode * roundingMode)                                \
    {                                                                               \
        return superClass::GetRoundingMode(roundingMode);                           \
    }                                                                               \
                                                                                    \
    ECode className::SetRoundingMode(                                               \
            /* [in] */ RoundingMode roundingMode)                                 \
    {                                                                               \
        return superClass::SetRoundingMode(roundingMode);                           \
    }                                                                               \

#endif  //__NUMBERFORMATMACRO_H__