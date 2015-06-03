#ifndef __COLLATOR_H__
#define __COLLATOR_H__

#include "Elastos.CoreLibrary_server.h"
#include <elastos.h>

using Libcore::ICU::ILocale;
using Libcore::ICU::IRuleBasedCollatorICU;
using Elastos::Text::ICollationKey;

namespace Elastos {
namespace Text {

class Collator
{
public:
    virtual CARAPI Compare(
        /* [in] */ IInterface* object1,
        /* [in] */ IInterface* object2,
        /* [out] */ Int32* result);

    virtual CARAPI Compare(
        /* [in] */ const String& string1,
        /* [in] */ const String& string2,
        /* [out] */ Int32* result) = 0;

    //@Override
    virtual CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    virtual CARAPI Equals(
        /* [in] */ const String& string1,
        /* [in] */ const String& string2,
        /* [out] */ Boolean* result);

    static CARAPI GetAvailableLocales(
       /* [out] */ ArrayOf<ILocale*>** locales);

    virtual CARAPI GetCollationKey(
        /* [in] */ const String& string,
        /* [out] */ ICollationKey** key) = 0;

    virtual CARAPI GetDecomposition(
        /* [out] */ Int32* decomposition);

    static CARAPI GetInstance(
       /* [out] */ ICollator** instance);

    static CARAPI GetInstance(
       /* [in] */ ILocale* locale,
       /* [out] */ ICollator** instance);

    virtual CARAPI GetStrength(
        /* [out] */ Int32* strength);

//    @Override
//    public abstract int hashCode();

    virtual CARAPI SetDecomposition(
        /* [in] */ Int32 value);

    virtual CARAPI SetStrength(
        /* [in] */ Int32 value);

    virtual CARAPI GetICUCollator(
        /* [out] */ IRuleBasedCollatorICU** icuCollator);

protected:
    CARAPI Init(
        /* [in] */ IRuleBasedCollatorICU* icuColl);

    CARAPI Init();

private:
    CARAPI DecompositionMode_Java_ICU(
        /* [in] */ Int32 mode,
        /* [out] */ Int32* value);

    CARAPI DecompositionMode_ICU_Java(
        /* [in] */ Int32 mode,
        /* [out] */ Int32* value);

    CARAPI Strength_Java_ICU(
        /* [in] */ Int32 value,
        /* [out] */ Int32* result);

    CARAPI Strength_ICU_Java(
        /* [in] */ Int32 value,
        /* [out] */ Int32* result);

public:
    AutoPtr<IRuleBasedCollatorICU> mICUColl;
};

} // namespace Text
} // namespace Elastos

#endif //__COLLATOR_H__
