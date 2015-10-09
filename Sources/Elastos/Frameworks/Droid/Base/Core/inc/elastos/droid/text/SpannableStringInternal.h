
#ifndef __ELASTOS_DROID_TEXT_SPANNABLESTRINGINTERNAL_H__
#define __ELASTOS_DROID_TEXT_SPANNABLESTRINGINTERNAL_H__

#include "ext/frameworkext.h"

using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Text {

class SpannableStringInternal
{
public:
    SpannableStringInternal(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    virtual ~SpannableStringInternal();

    virtual CARAPI_(Int32) GetLength();

    virtual CARAPI_(Char32) GetCharAt(
        /* [in] */ Int32 i);

    virtual CARAPI ToString(
        /* [out] */ String* str);

    virtual CARAPI GetChars(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ArrayOf<Char32>* dest,
        /* [in] */ Int32 off);

    /* package */
    virtual CARAPI SetSpan(
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 flags);

    /* package */
    virtual CARAPI RemoveSpan(
        /* [in] */ IInterface* what);

    virtual CARAPI_(Int32) GetSpanStart(
        /* [in] */ IInterface* what);

    virtual CARAPI_(Int32) GetSpanEnd(
        /* [in] */ IInterface* what);

    virtual CARAPI_(Int32) GetSpanFlags(
        /* [in] */ IInterface* what);

    virtual CARAPI GetSpans(
        /* [in] */ Int32 queryStart,
        /* [in] */ Int32 queryEnd,
        /* [in] */ const InterfaceID& kind,
        /* [out, callee] */ ArrayOf<IInterface*>** objs);

    virtual CARAPI_(Int32) NextSpanTransition(
        /* [in] */ Int32 start,
        /* [in] */ Int32 limit,
        /* [in] */ const InterfaceID& kind);

    CARAPI Equals(
        /* [in] */ IInterface* o,
        /* [out] */ Boolean* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* result);

public:
    /* package */
    static /*const*/ AutoPtr< ArrayOf<IInterface*> > EMPTY;

protected:
    SpannableStringInternal();

    virtual CARAPI Init(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(Boolean) IsSpanEquals(
        /* [in] */ IInterface* lhs,
        /* [in] */ IInterface* rhs);
private:
    CARAPI_(void) SendSpanAdded(
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(void) SendSpanRemoved(
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(void) SendSpanChanged(
        /* [in] */ IInterface* what,
        /* [in] */ Int32 s,
        /* [in] */ Int32 e,
        /* [in] */ Int32 st,
        /* [in] */ Int32 en);

    static CARAPI_(void) Region(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ String* str);

    CARAPI CheckRange(
        /* [in] */ const char* operation,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

private:
    String mText;
    AutoPtr< ArrayOf<IInterface*> > mSpans;
    AutoPtr< ArrayOf<Int32> > mSpanData;
    Int32 mSpanCount;

    static const Int32 START = 0;
    static const Int32 END = 1;
    static const Int32 FLAGS = 2;
    static const Int32 COLUMNS = 3;
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_TEXT_SPANNABLESTRINGINTERNAL_H__
