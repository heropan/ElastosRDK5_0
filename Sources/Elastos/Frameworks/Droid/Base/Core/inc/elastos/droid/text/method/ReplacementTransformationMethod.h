#ifndef __ELASTOS_DROID_TEXT_METHOD_ReplacementTransformationMethod_H__
#define __ELASTOS_DROID_TEXT_METHOD_ReplacementTransformationMethod_H__

#include "Elastos.Droid.Core_server.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Text::IGetChars;
using Elastos::Droid::Text::ISpannable;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

/**
 * This transformation method causes the characters in the {@link #getOriginal}
 * array to be replaced by the corresponding characters in the
 * {@link #getReplacement} array.
 */
//public abstract
class ReplacementTransformationMethod /*implements ITransformationMethod*/
{
private:
    //static
    class ReplacementCharSequence
        : public ElRefBase
        , public IGetChars
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* object,
            /* [in] */ InterfaceID* iid);
    public:
        CARAPI GetLength(
            /* [out] */ Int32* number);

        CARAPI GetCharAt(
            /* [in] */ Int32 i,
            /* [out] */ Char32* cr);

        CARAPI SubSequence(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ ICharSequence** csq);

        CARAPI ToString(
            /* [out] */ String* str);

        CARAPI GetChars(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ ArrayOf<Char32>* dest,
            /* [out] */ Int32 off);
    public:
        ReplacementCharSequence(
            /* [in] */ ICharSequence* source,
            /* [in] */ ArrayOf<Char32>* original,
            /* [in] */ ArrayOf<Char32>* replacement);
    private:
        AutoPtr< ArrayOf<Char32> > mOriginal, mReplacement;
        AutoPtr<ICharSequence> mSource;
    };

    //static
    class SpannedReplacementCharSequence
        : public ReplacementCharSequence
        , public ISpanned
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* object,
            /* [in] */ InterfaceID* iid);
    public:
        CARAPI GetLength(
            /* [out] */ Int32* number);

        CARAPI GetCharAt(
            /* [in] */ Int32 i,
            /* [out] */ Char32* cr);

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
            /* [in] */ Int32 end,
            /* [in] */ const InterfaceID& type,
            /* [out] */ Int32* offset);

        CARAPI SubSequence(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ ICharSequence** csq);

    public:
        SpannedReplacementCharSequence(
            /* [in] */ ISpanned* source,
            /* [in] */ ArrayOf<Char32>* original,
            /* [in] */ ArrayOf<Char32>* replacement);
    private:
        AutoPtr<ISpanned> mSpanned;
    };

public:
    /**
     * Returns a CharSequence that will mirror the contents of the
     * source CharSequence but with the characters in {@link #getOriginal}
     * replaced by ones from {@link #getReplacement}.
     */
    CARAPI_(AutoPtr<ICharSequence>) GetTransformation(
        /* [in] */ ICharSequence* source,
        /* [in] */ IView* v);

    CARAPI_(void) OnFocusChanged(
        /* [in] */ IView* view,
        /* [in] */ ICharSequence* sourceText,
        /* [in] */ Boolean focused,
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

protected:
    /**
     * Returns the list of characters that are to be replaced by other
     * characters when displayed.
     */
    virtual CARAPI_(AutoPtr< ArrayOf<Char32> >) GetOriginal() = 0;
    /**
     * Returns a parallel array of replacement characters for the ones
     * that are to be replaced.
     */
    virtual CARAPI_(AutoPtr< ArrayOf<Char32> >) GetReplacement() = 0;

};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_ReplacementTransformationMethod_H__
