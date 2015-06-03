
#ifndef __ICU_NATIVEBREAKITERATOR_H__
#define __ICU_NATIVEBREAKITERATOR_H__

#include <cmdef.h>
#include <elastos.h>
#include <eltypes.h>

#include <Elastos.CoreLibrary_server.h>

using Elastos::ElRefBase;
using Libcore::ICU::INativeBreakIterator;
using Elastos::Text::ICharacterIterator;

namespace Libcore {
namespace ICU {

class NativeBreakIterator
    : public ElRefBase
    , public INativeBreakIterator
{
public:
    CAR_INTERFACE_DECL();

    ~NativeBreakIterator();

    CARAPI Clone(
        /* [out] */ INativeBreakIterator ** outiter);

    CARAPI Equals(
        /* [in] */ IInterface * object,
        /* [out] */ Boolean * value);

    CARAPI GetHashCode(
        /* [out] */ Int32 * value);

    CARAPI Current(
        /* [out] */ Int32 * value);

    CARAPI First(
        /* [out] */ Int32 * value);

    CARAPI Following(
        /* [in] */ Int32 offset,
        /* [out] */ Int32 * value);

    CARAPI GetText(
        /* [out] */ ICharacterIterator ** outiter);

    CARAPI Last(
        /* [out] */ Int32 * value);

    CARAPI Next(
        /* [in] */ Int32 n,
        /* [out] */ Int32 * value);

    CARAPI NextEx(
        /* [out] */ Int32 * value);

    CARAPI Previous(
        /* [out] */ Int32 * value);

    CARAPI SetText(
        /* [in] */ ICharacterIterator * newText);

    CARAPI SetTextEx(
        /* [in] */ const String& newText);

    CARAPI IsBoundary(
        /* [in] */ Int32 offset,
        /* [out] */ Boolean * value);

    CARAPI Preceding(
        /* [in] */ Int32 offset,
        /* [out] */ Int32 * value);

    static CARAPI_(AutoPtr<INativeBreakIterator>) GetCharacterInstance(
        /* [in] */ ILocale* where);

    static CARAPI_(AutoPtr<INativeBreakIterator>) GetLineInstance(
        /* [in] */ ILocale* where);

    static CARAPI_(AutoPtr<INativeBreakIterator>) GetSentenceInstance(
        /* [in] */ ILocale* where);

    static CARAPI_(AutoPtr<INativeBreakIterator>) GetWordInstance(
        /* [in] */ ILocale* where);

private:
    NativeBreakIterator(
        /* [in] */ Int32 address,
        /* [in] */ Int32 type);

    CARAPI_(void) SetText(
        /* [in] */ const String& s,
        /* [in] */ ICharacterIterator* it);

    static CARAPI_(Int32) GetCharacterInstanceImpl(
        /* [in] */ const String& locale);

    static CARAPI_(Int32) GetWordInstanceImpl(
        /* [in] */ const String& locale);

    static CARAPI_(Int32) GetLineInstanceImpl(
        /* [in] */ const String& locale);

    static CARAPI_(Int32) GetSentenceInstanceImpl(
        /* [in] */ const String& locale);

    static CARAPI_(void) CloseBreakIteratorImpl(
        /* [in] */ Int32 address);

    static CARAPI_(void) SetTextImpl(
        /* [in] */ Int32 address,
        /* [in] */ const String& text);

    static CARAPI_(Int32) CloneImpl(
        /* [in] */ Int32 address);

    static CARAPI_(Int32) PrecedingImpl(
        /* [in] */ Int32 address,
        /* [in] */ Int32 offset);

    static CARAPI_(Boolean) IsBoundaryImpl(
        /* [in] */ Int32 address,
        /* [in] */ Int32 offset);

    static CARAPI_(Int32) NextImpl(
        /* [in] */ Int32 address,
        /* [in] */ Int32 n);

    static CARAPI_(Int32) PreviousImpl(
        /* [in] */ Int32 address);

    static CARAPI_(Int32) CurrentImpl(
        /* [in] */ Int32 address);

    static CARAPI_(Int32) FirstImpl(
        /* [in] */ Int32 address);

    static CARAPI_(Int32) FollowingImpl(
        /* [in] */ Int32 address,
        /* [in] */ Int32 offset);

    static CARAPI_(Int32) LastImpl(
        /* [in] */ Int32 address);

private:
    // Acceptable values for the 'type' field.
    static const Int32 BI_CHAR_INSTANCE;
    static const Int32 BI_WORD_INSTANCE;
    static const Int32 BI_LINE_INSTANCE;
    static const Int32 BI_SENT_INSTANCE;

    Int32 mAddress;
    Int32 mType;
    AutoPtr<ICharacterIterator> mCharIter;
};

} // namespace ICU
} // namespace Libcore

#endif //__ICU_NATIVEBREAKITERATOR_H__

