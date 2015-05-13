#ifndef __CSPANNEDSTRING_H__
#define __CSPANNEDSTRING_H__

#include "_CSpannedString.h"
#include "text/SpannedString.h"

namespace Elastos {
namespace Droid {
namespace Text {

CarClass(CSpannedString), public SpannedString
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ ICharSequence* source);

    CARAPI constructor(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SubSequence(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ICharSequence** res);

    CARAPI GetLength(
        /* [out] */ Int32* number);

    CARAPI GetCharAt(
        /* [in] */ Int32 index,
        /* [out] */ Char32* c);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI GetSpans(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ const InterfaceID& type,
        /* [out, callee] */ ArrayOf<IInterface*>** objs);

    CARAPI GetSpanStart(
        /* [in] */ IInterface* tag,
        /* [out] */ Int32* start);

    CARAPI GetSpanEnd(
        /* [in] */ IInterface* tag,
        /* [out] */ Int32* end);

    CARAPI GetSpanFlags(
        /* [in] */ IInterface* tag,
        /* [out] */ Int32* flags);

    CARAPI NextSpanTransition(
        /* [in] */ Int32 start,
        /* [in] */ Int32 limit,
        /* [in] */ const InterfaceID& type,
        /* [out] */ Int32* offset);

    CARAPI GetChars(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ArrayOf<Char32>* dest,
        /* [in] */ Int32 destoff);

    CARAPI SetSpan(
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 flags);

    CARAPI Remove(
        /* [in] */ IInterface* what);

    static AutoPtr<ISpannedString> ValueOf(
        /* [in] */ ICharSequence* source);
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos


#endif //__CSPANNEDSTRING_H__
