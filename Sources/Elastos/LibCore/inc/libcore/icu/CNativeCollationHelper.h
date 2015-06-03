
#ifndef __ICU_CNATIVECOLLATIONHELPER_H__
#define __ICU_CNATIVECOLLATIONHELPER_H__

#include "_CNativeCollationHelper.h"

namespace Libcore {
namespace ICU {

CarClass(CNativeCollationHelper)
{
public:
    CARAPI CloseCollator(
        /* [in] */ Int32 address);

    CARAPI Compare(
        /* [in] */ Int32 address,
        /* [in] */ const String& source,
        /* [in] */ const String& target ,
        /* [out] */ Int32 * value);

    CARAPI GetAttribute(
        /* [in] */ Int32 address,
        /* [in] */ Int32 type,
        /* [out] */ Int32 * value);

    CARAPI GetCollationElementIterator(
        /* [in] */ Int32 address,
        /* [in] */ const String& source,
        /* [out] */ Int32 * value);

    CARAPI GetRules(
        /* [in] */ Int32 address,
        /* [out] */ String * str);

    CARAPI GetSortKey(
        /* [in] */ Int32 address,
        /* [in] */ const String& source,
        /* [out,callee] */ ArrayOf<Byte> ** outarray);

    CARAPI OpenCollator(
        /* [in] */ const String& locale,
        /* [out] */ Int32 * value);

    CARAPI OpenCollatorFromRules(
        /* [in] */ const String& rules,
        /* [in] */ Int32 normalizationMode,
        /* [in] */ Int32 collationStrength,
        /* [out] */ Int32 * value);

    CARAPI SafeClone(
        /* [in] */ Int32 address,
        /* [out] */ Int32 * value);

    CARAPI SetAttribute(
        /* [in] */ Int32 address,
        /* [in] */ Int32 type,
        /* [in] */ Int32 value);

    CARAPI CloseElements(
        /* [in] */ Int32 address);

    CARAPI GetMaxExpansion(
        /* [in] */ Int32 address,
        /* [in] */ Int32 order,
        /* [out] */ Int32 * value);

    CARAPI GetOffset(
        /* [in] */ Int32 address,
        /* [out] */ Int32 * value);

    CARAPI Next(
        /* [in] */ Int32 address,
        /* [out] */ Int32 * value);

    CARAPI Previous(
        /* [in] */ Int32 address,
        /* [out] */ Int32 * value);

    CARAPI Reset(
        /* [in] */ Int32 address);

    CARAPI SetOffset(
        /* [in] */ Int32 address,
        /* [in] */ Int32 offset);

    CARAPI SetText(
        /* [in] */ Int32 address,
        /* [in] */ const String& source);
};

} // namespace ICU
} // namespace Libcore

#endif //__ICU_CNATIVECOLLATIONHELPER_H__
