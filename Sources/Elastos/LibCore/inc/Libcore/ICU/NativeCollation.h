
#ifndef __NATIVECOLLATION_H__
#define __NATIVECOLLATION_H__

#include <elastos.h>
#include <eltypes.h>
#include <cmdef.h>
#include <unicode/unistr.h>
#include <elastos/HashMap.h>
#include <elastos/Mutex.h>
#include "Elastos.Core_server.h"

using Elastos::Utility::HashMap;
using Elastos::Core::Threading::Mutex;

namespace Libcore {
namespace ICU {

class NativeCollation
{
public:
    // Collator.
    static CARAPI_(void) CloseCollator(
        /* [in] */ Int32 collatoraddress);

    static CARAPI_(Int32) Compare(
        /* [in] */ Int32 collatoraddress,
        /* [in] */ const String& source,
        /* [in] */ const String& target);

    static CARAPI GetAttribute(
        /* [in] */ Int32 collatoraddress,
        /* [in] */ Int32 type,
        /* [out] */ Int32* result);

    static CARAPI GetCollationElementIterator(
        /* [in] */ Int32 collatoraddress,
        /* [in] */ const String& source,
        /* [out] */ Int32* coleitr);

    static CARAPI_(String) GetRules(
        /* [in] */ Int32 collatoraddress);

    static CARAPI_(AutoPtr<ArrayOf<Byte> >) GetSortKey(
        /* [in] */ Int32 collatoraddress,
        /* [in] */ const String& source);

    static CARAPI OpenCollator(
        /* [in] */ const String& locale,
        /* [out] */ Int32* address);

    static CARAPI OpenCollatorFromRules(
        /* [in] */ const String& rules,
        /* [in] */ Int32 normalizationmode,
        /* [in] */ Int32 collationstrength,
        /* [out] */ Int32* address);

    static CARAPI SafeClone(
        /* [in] */ Int32 collatoraddress,
        /* [out] */ Int32* address);

    static CARAPI SetAttribute(
        /* [in] */ Int32 collatoraddress,
        /* [in] */ Int32 type,
        /* [in] */ Int32 value);

    // CollationElementIterator.
    static CARAPI_(void) CloseElements(
        /* [in] */ Int32 address);

    static CARAPI_(Int32) GetMaxExpansion(
        /* [in] */ Int32 address,
        /* [in] */ Int32 order);

    static CARAPI_(Int32) GetOffset(
        /* [in] */ Int32 address);

    static CARAPI Next(
        /* [in] */ Int32 address,
        /* [out] */ Int32* result);

    static CARAPI Previous(
        /* [in] */ Int32 address,
        /* [out] */ Int32* result);

    static CARAPI_(void) Reset(
        /* [in] */ Int32 address);

    static CARAPI SetOffset(
        /* [in] */ Int32 address,
        /* [in] */ Int32 offset);

    static CARAPI SetText(
        /* [in] */ Int32 address,
        /* [in] */ const String& source);

private:
    static HashMap<Int32, UnicodeString*> sUstrMap;
    static Mutex sUstrMapLock;
};

} // namespace ICU
} // namespace Libcore

#endif //__NATIVECOLLATION_H__
