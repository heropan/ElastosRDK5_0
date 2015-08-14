
#ifndef __ELASTOS_DROID_TEXT_CALTEREDCHARSEQUENCE_H__
#define __ELASTOS_DROID_TEXT_CALTEREDCHARSEQUENCE_H__

#include "_CAlteredCharSequence.h"

namespace Elastos {
namespace Droid {
namespace Text {

CarClass(CAlteredCharSequence)
{
private:
    //static
    class AlteredSpanned
        : public ElRefBase
        , public ISpanned
        , public IGetChars
        , public IAlteredCharSequence
    {
    public:
        friend class CAlteredCharSequence;
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* Object,
            /* [out] */ InterfaceID* iID);

    public:
        CARAPI GetSpans(
            /*[in]*/ Int32 start,
            /*[in]*/ Int32 end,
            /*[in]*/ const InterfaceID& type,
            /*[out, callee]*/ ArrayOf<IInterface*>** objs);

        CARAPI GetSpanStart(
            /*[in]*/ IInterface* tag,
            /*[out]*/ Int32* start);

        CARAPI GetSpanEnd(
            /*[in]*/ IInterface* tag,
            /*[out]*/ Int32* end);

        CARAPI GetSpanFlags(
            /*[in]*/ IInterface* tag,
            /*[out]*/ Int32* flags);

        CARAPI NextSpanTransition(
            /* [in] */ Int32 start,
            /* [in] */ Int32 limit,
            /* [in] */ const InterfaceID& type,
            /* [out] */ Int32* offset);

    public:
        static AutoPtr<IAlteredCharSequence> Make(
            /* [in] */ ICharSequence* source,
            /* [in] */ ArrayOf<Char32>* sub,
            /* [in] */ Int32 substart,
            /* [in] */ Int32 subend);

        CARAPI_(void) Update(
            /* [in] */ ArrayOf<Char32>* sub,
            /* [in] */ Int32 substart,
            /* [in] */ Int32 subend);

    public:
        CARAPI GetCharAt(
            /*[in]*/ Int32 off,
            /*[out]*/ Char32* c);

        CARAPI GetLength(
            /*[out]*/ Int32* number);

        CARAPI SubSequence(
            /*[in]*/ Int32 start,
            /*[in]*/ Int32 end,
            /*[out]*/ ICharSequence** csq);

        CARAPI GetChars(
            /*[in]*/ Int32 start,
            /*[in]*/ Int32 end,
            /*[in]*/ ArrayOf<Char32>* dest,
            /*[in]*/ Int32 off);

        CARAPI ToString(
                /*[out]*/ String* str);

    private:
        AlteredSpanned(
            /* [in] */ ICharSequence* source,
            /* [in] */ ArrayOf<Char32>* sub,
            /* [in] */ Int32 substart,
            /* [in] */ Int32 subend);

    protected://private:
        Int32 mStart;
        Int32 mEnd;
        AutoPtr< ArrayOf<Char32> > mChars;
        AutoPtr<ICharSequence> mSource;

    private:
        AutoPtr<ISpanned> mSpanned;
    };

public:
    /**
     * Create an AlteredCharSequence whose text (and possibly spans)
     * are mirrored from <code>source</code>, except that the range of
     * offsets <code>substart</code> inclusive to <code>subend</code> exclusive
     * are mirrored instead from <code>sub</code>, beginning at offset 0.
     */
    static AutoPtr<IAlteredCharSequence> Make(
        /* [in] */ ICharSequence* source,
        /* [in] */ ArrayOf<Char32>* sub,
        /* [in] */ Int32 substart,
        /* [in] */ Int32 subend);

public:
    /* package */
    CARAPI_(void) Update(
        /* [in] */ ArrayOf<Char32>* sub,
        /* [in] */ Int32 substart,
        /* [in] */ Int32 subend);

public:
    CARAPI GetCharAt(
        /*[in]*/ Int32 off,
        /*[out]*/ Char32* c);

    CARAPI GetLength(
        /*[out]*/ Int32* number);

    CARAPI SubSequence(
        /*[in]*/ Int32 start,
        /*[in]*/ Int32 end,
        /*[out]*/ ICharSequence** csq);

    CARAPI GetChars(
        /*[in]*/ Int32 start,
        /*[in]*/ Int32 end,
        /*[in]*/ ArrayOf<Char32>* dest,
        /*[in]*/ Int32 off);

    CARAPI ToString(
            /*[out]*/ String* str);
public:
    //private
    constructor(
        /* [in] */ ICharSequence* source,
        /* [in] */ ArrayOf<Char32>* sub,
        /* [in] */ Int32 substart,
        /* [in] */ Int32 subend);

protected:
    constructor();

protected://private:
    Int32 mStart;
    Int32 mEnd;
    AutoPtr< ArrayOf<Char32> > mChars;
    AutoPtr<ICharSequence> mSource;
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_CALTEREDCHARSEQUENCE_H__
