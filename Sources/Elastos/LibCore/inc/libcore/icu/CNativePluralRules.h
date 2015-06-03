
#ifndef __ICU_CNATIVEPLURALRULES_H__
#define __ICU_CNATIVEPLURALRULES_H__

#include "_CNativePluralRules.h"

namespace Libcore {
namespace ICU {

CarClass(CNativePluralRules)
{
public:
    CARAPI QuantityForInt(
        /* [in] */ Int32 value,
        /* [out] */ Int32 * result);

    static CARAPI_(AutoPtr<INativePluralRules>) ForLocale(
        /* [in] */ ILocale* locale);

    Init(
        /* [in] */ Int32 address);

    ~CNativePluralRules();

private:
    static CARAPI_(void) FinalizeImpl(
        /* [in] */ Int32 address);

    static CARAPI_(Int32) ForLocaleImpl(
        /* [in] */ const String& localeName);

    static CARAPI_(Int32) QuantityForIntImpl(
        /* [in] */ Int32 address,
        /* [in] */ Int32 value);

private:
    Int32 mAddress;
};

} // namespace ICU
} // namespace Libcore

#endif //__ICU_CNATIVEPLURALRULES_H__
