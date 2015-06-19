#ifndef __ELASTOS_TEXT_DECIMALFORMATSYMBOLS_H__
#define __ELASTOS_TEXT_DECIMALFORMATSYMBOLS_H__

#include <elastos/core/Object.h>

using Elastos::Utility::ILocale;
using Elastos::Utility::ICurrency;

namespace Elastos {
namespace Text {

class DecimalFormatSymbols : public Object
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ILocale* locale);

    static CARAPI GetInstance(
        /* [out] */ IDecimalFormatSymbols** instance);

    static CARAPI GetInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ IDecimalFormatSymbols** instance);

    static CARAPI GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    //@Override
    virtual CARAPI Clone(
        /* [out] */ IDecimalFormatSymbols** object);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* res);

    //@Override
    //public String toString();

    virtual CARAPI GetCurrency(
        /* [out] */ ICurrency** currency);

    virtual CARAPI GetInternationalCurrencySymbol(
        /* [out] */ String* symbol);

    virtual CARAPI GetCurrencySymbol(
        /* [out] */ String* symbol);

    virtual CARAPI GetDecimalSeparator(
        /* [out] */ Char32* separator);

    virtual CARAPI GetDigit(
        /* [out] */ Char32* digit);

    virtual CARAPI GetGroupingSeparator(
        /* [out] */ Char32* separator);

    virtual CARAPI GetInfinity(
        /* [out] */ String* infinity);

    virtual CARAPI GetMinusSign(
        /* [out] */ Char32* minusSign);

    virtual CARAPI GetMonetaryDecimalSeparator(
        /* [out] */ Char32* separator);

    virtual CARAPI GetNaN(
            /* [out] */ String* nan);

    virtual CARAPI GetPatternSeparator(
        /* [out] */ Char32* separator);

    virtual CARAPI GetPercent(
            /* [out] */ Char32* percent);

    virtual CARAPI GetPerMill(
        /* [out] */ Char32* perMill);

    virtual CARAPI GetZeroDigit(
        /* [out] */ Char32* zeroDigit);

    virtual CARAPI GetExponentSeparator(
        /* [out] */ String* separator);

    // @Override
    // public int hashCode();

    virtual CARAPI SetCurrency(
        /* [in] */ ICurrency* currency);

    virtual CARAPI SetInternationalCurrencySymbol(
        /* [in] */ const String& value);

    virtual CARAPI SetCurrencySymbol(
        /* [in] */ const String& value);

    virtual CARAPI SetDecimalSeparator(
        /* [in] */ Char32 value);

    virtual CARAPI SetDigit(
        /* [in] */ Char32 value);

    virtual CARAPI SetGroupingSeparator(
            /* [in] */ Char32 value);

    virtual CARAPI SetInfinity(
        /* [in] */ const String& value);

    virtual CARAPI SetMinusSign(
        /* [in] */ Char32 value);

    virtual CARAPI SetMonetaryDecimalSeparator(
        /* [in] */ Char32 value);

    virtual CARAPI SetNaN(
        /* [in] */ const String& value);

    virtual CARAPI SetPatternSeparator(
        /* [in] */ Char32 value);

    virtual CARAPI SetPercent(
        /* [in] */ Char32 value);

    virtual CARAPI SetPerMill(
        /* [in] */ Char32 value);

    virtual CARAPI SetZeroDigit(
        /* [in] */ Char32 value);

    virtual CARAPI SetExponentSeparator(
        /* [in] */ const String& value);

/*    private static ObjectStreamField[] serialPersistentFields = {
        new ObjectStreamField("currencySymbol", String.class),
        new ObjectStreamField("decimalSeparator", char.class),
        new ObjectStreamField("digit", char.class),
        new ObjectStreamField("exponential", char.class),
        new ObjectStreamField("exponentialSeparator", String.class),
        new ObjectStreamField("groupingSeparator", char.class),
        new ObjectStreamField("infinity", String.class),
        new ObjectStreamField("intlCurrencySymbol", String.class),
        new ObjectStreamField("minusSign", char.class),
        new ObjectStreamField("monetarySeparator", Char.class),
        new ObjectStreamField("NaN", String.class),
        new ObjectStreamField("patternSeparator", char.class),
        new ObjectStreamField("percent", char.class),
        new ObjectStreamField("perMill", char.class),
        new ObjectStreamField("serialVersionOnStream", int.class),
        new ObjectStreamField("zeroDigit", char.class),
        new ObjectStreamField("locale", Locale.class),
    };*/

    // private void writeObject(ObjectOutputStream stream);

    // private void readObject(ObjectInputStream stream);

private:
    Char32 mZeroDigit;

    Char32 mDigit;

    Char32 mDecimalSeparator;

    Char32 mGroupingSeparator;

    Char32 mPatternSeparator;

    Char32 mPercent;

    Char32 mPerMill;

    Char32 mMonetarySeparator;

    String mMinusSign;

    String mInfinity, mNaN, mCurrencySymbol, mIntlCurrencySymbol;

    mutable AutoPtr<ICurrency> mCurrency;

    mutable AutoPtr<ILocale> mLocale;

    mutable String mExponentSeparator;
};

} // namespace Text
} // namespace Elastos

#endif //__ELASTOS_TEXT_DECIMALFORMATSYMBOLS_H__
