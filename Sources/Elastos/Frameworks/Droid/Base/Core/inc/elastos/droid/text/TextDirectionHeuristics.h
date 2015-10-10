
#ifndef __ELASTOS_DROID_TEXT_TEXTDIRECTIONHEURISTICS_H__
#define __ELASTOS_DROID_TEXT_TEXTDIRECTIONHEURISTICS_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>
#include <elastos/core/Character.h>

using Elastos::Core::Character;

namespace Elastos {
namespace Droid {
namespace Text {

/**
 * Some objects that implement {@link TextDirectionHeuristic}. Use these with
 * the {@link BidiFormatter#unicodeWrap unicodeWrap()} methods in {@link BidiFormatter}.
 * Also notice that these direction heuristics correspond to the same types of constants
 * provided in the {@link android.view.View} class for {@link android.view.View#setTextDirection
 * setTextDirection()}, such as {@link android.view.View#TEXT_DIRECTION_RTL}.
 * <p>To support versions lower than {@link android.os.Build.VERSION_CODES#JELLY_BEAN_MR2},
 * you can use the support library's {@link android.support.v4.text.TextDirectionHeuristicsCompat}
 * class.
 *
 */
class TextDirectionHeuristics
{
public:
    static AutoPtr<ITextDirectionHeuristic> GetLTR();
    static AutoPtr<ITextDirectionHeuristic> GetRTL();

    static AutoPtr<ITextDirectionHeuristic> GetFIRSTSTRONG_LTR();
    static AutoPtr<ITextDirectionHeuristic> GetFIRSTSTRONG_RTL();

    static AutoPtr<ITextDirectionHeuristic> GetNYRTL_LTR();
    static AutoPtr<ITextDirectionHeuristic> GetLOCALE();

private:
    /** Always decides that the direction is left to right. */
    static AutoPtr<ITextDirectionHeuristic> LTR;

    /** Always decides that the direction is right to left. */
    static AutoPtr<ITextDirectionHeuristic> RTL;

    /**
     * Determines the direction based on the first strong directional character,
     * including bidi format chars, falling back to left to right if it
     * finds none.  This is the default behavior of the Unicode Bidirectional
     * Algorithm.
     */
    static AutoPtr<ITextDirectionHeuristic> FIRSTSTRONG_LTR;

    /**
     * Determines the direction based on the first strong directional character,
     * including bidi format chars, falling back to right to left if it
     * finds none.  This is similar to the default behavior of the Unicode
     * Bidirectional Algorithm, just with different fallback behavior.
     */
    static AutoPtr<ITextDirectionHeuristic> FIRSTSTRONG_RTL;

    /**
     * If the text contains any strong right to left non-format character, determines
     * that the direction is right to left, falling back to left to right if it
     * finds none.
     */
    static AutoPtr<ITextDirectionHeuristic> ANYRTL_LTR;

    /**
     * Force the paragraph direction to the Locale direction. Falls back to left to right.
     */
    static AutoPtr<ITextDirectionHeuristic> LOCALE;

private:

    /**
     * Interface for an algorithm to guess the direction of a paragraph of text.
     *
     */
    class TextDirectionAlgorithm : public Object
    {
    public:
        /**
         * Returns whether the range of text is RTL according to the algorithm.
         *
         */
        virtual CARAPI_(TriState) CheckRtl(
            /* [in] */ ArrayOf<Char32>* text,
            /* [in] */ Int32 start,
            /* [in] */ Int32 count) = 0;

        virtual ~TextDirectionAlgorithm()
        {}
    };

    /**
     * Computes the text direction based on an algorithm.  Subclasses implement
     * {@link #defaultIsRtl} to handle cases where the algorithm cannot determine the
     * direction from the text alone.
     */
    class TextDirectionHeuristicImpl
        : public Object
        , public ITextDirectionHeuristic
    {
    public:
        CAR_INTERFACE_DECL()

        TextDirectionHeuristicImpl(
            /* [in] */ TextDirectionAlgorithm* algorithm)
             : mAlgorithm(algorithm)
        {
        }

        //@Override
        CARAPI IsRtl(
            /* [in] */ ArrayOf<Char32>* chars,
            /* [in] */ Int32 start,
            /* [in] */ Int32 count,
            /* [out] */ Boolean* result);

        CARAPI IsRtl(
            /* [in] */ ICharSequence* chars,
            /* [in] */ Int32 start,
            /* [in] */ Int32 count,
            /* [out] */ Boolean* result);
        {
            VALIDATE_NOT_NULL(result);
            *result = FALSE;

            if (chars == NULL || start < 0 || count < 0 || chars->GetLength() - count < start) {
//                throw new IllegalArgumentException();
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }

            if (mAlgorithm == NULL) {
                *result = DefaultIsRtl();
                return NOERROR;
            }

            *result = DoCheck(chars, start, count);

            return NOERROR;
        }

    protected:

        /**
         * Return true if the default text direction is rtl.
         */
        virtual CARAPI_(Boolean) DefaultIsRtl() = 0;

    private:

        CARAPI_(Boolean) DoCheck(
            /* [in] */ ArrayOf<Char32>* chars,
            /* [in] */ Int32 start,
            /* [in] */ Int32 count);

        AutoPtr<TextDirectionAlgorithm> mAlgorithm;
    };

    class TextDirectionHeuristicInternal
        : public TextDirectionHeuristicImpl
    {
        friend class TextDirectionHeuristics;

    public:
        //@Override
        CARAPI_(Boolean) DefaultIsRtl()
        {
            return mDefaultIsRtl;
        }

        TextDirectionHeuristicInternal(
            /* [in] */ TextDirectionAlgorithm* algorithm,
            /* [in] */ Boolean defaultIsRtl)
            : TextDirectionHeuristicImpl(algorithm)
        {
            mDefaultIsRtl = defaultIsRtl;
        }

    private:
        Boolean mDefaultIsRtl;
    };

    /**
     * Algorithm that uses the first strong directional character to determine the paragraph
     * direction. This is the standard Unicode Bidirectional algorithm.
     */
    class FirstStrong
        : public TextDirectionAlgorithm
    {
    public:

        static CARAPI_(AutoPtr<FirstStrong>) GetInstance()
        {
            if (sInstance == NULL) {
                sInstance = new FirstStrong();
            }

            return sInstance;
        }

        //@Override
        CARAPI_(TriState) CheckRtl(
            /* [in] */ ArrayOf<Char32>* text,
            /* [in] */ Int32 start,
            /* [in] */ Int32 count)
        {
            TriState result = TriState_UNKNOWN;
            for (Int32 i = start, e = start + count; i < e && result == TriState_UNKNOWN; ++i) {
                result = IsRtlTextOrFormat(Character::GetDirectionality((*text)[i]));
            }
            return result;
        }

        virtual ~FirstStrong()
        {
        }

    private:
        FirstStrong()
        {
        }

        static AutoPtr<FirstStrong> sInstance;// = new FirstStrong();
    };

    /**
     * Algorithm that uses the presence of any strong directional non-format
     * character (e.g. excludes LRE, LRO, RLE, RLO) to determine the
     * direction of text.
     *
     */
    class AnyStrong
        : public TextDirectionAlgorithm
    {
    public:
        static CARAPI_(AutoPtr<AnyStrong>) GetRTLInstance()
        {
            if (sRTLInstance == NULL) {
                sRTLInstance = new AnyStrong(TRUE);
            }

            return sRTLInstance;
        }

        static CARAPI_(AutoPtr<AnyStrong>) GetLTRInstance()
        {
            if (sLTRInstance == NULL) {
                sLTRInstance = new AnyStrong(FALSE);
            }

            return sLTRInstance;
        }

        //@Override
        CARAPI_(TriState) CheckRtl(
            /* [in] */ ArrayOf<Char32>* text,
            /* [in] */ Int32 start,
            /* [in] */ Int32 count)
        {
            Boolean haveUnlookedFor = false;
            for (Int32 i = start, e = start + count; i < e; ++i) {
                switch (IsRtlText(Character::GetDirectionality((*text)[i]))) {
                    case TRUE:
                        if (mLookForRtl) {
                            return TriState_TRUE;
                        }
                        haveUnlookedFor = TRUE;
                        break;
                    case FALSE:
                        if (!mLookForRtl) {
                            return TriState_FALSE;
                        }
                        haveUnlookedFor = TRUE;
                        break;
                    default:
                        break;
                }
            }
            if (haveUnlookedFor) {
                return mLookForRtl ? TriState_FALSE : TriState_TRUE;
            }
            return TriState_UNKNOWN;
        }

        virtual ~AnyStrong()
        {
        }

    private:
        AnyStrong(
            /* [in] */ Boolean lookForRtl)
            : mLookForRtl(lookForRtl)
        {
        }

        static AutoPtr<AnyStrong> sRTLInstance;// = new AnyStrong(true);
        static AutoPtr<AnyStrong> sLTRInstance;// = new AnyStrong(false);

        Boolean mLookForRtl;
    };

    /**
     * Algorithm that uses the Locale direction to force the direction of a paragraph.
     */
    class TextDirectionHeuristicLocale
        : public TextDirectionHeuristicImpl
    {
    public:

        TextDirectionHeuristicLocale()
            : TextDirectionHeuristicImpl(NULL)
        {
        }

        ~TextDirectionHeuristicLocale()
        {
        }

        static AutoPtr<TextDirectionHeuristicLocale> GetInstance()
        {
            if (sInstance == NULL) {
                sInstance = new TextDirectionHeuristicLocale();
            }

            return sInstance;
        }

    protected:
        //@Override
        CARAPI_(Boolean) DefaultIsRtl();

    private:
        static AutoPtr<TextDirectionHeuristicLocale> sInstance;// = new TextDirectionHeuristicLocale();
    };

private:

    static Int32 IsRtlText(
        /* [in] */ Int32 directionality);

    static Int32 IsRtlTextOrFormat(
        /* [in] */ Int32 directionality);

private:
    static const Int32 STATE_TRUE;// = 0;
    static const Int32 STATE_FALSE;// = 1;
    static const Int32 STATE_UNKNOWN;// = 2;
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_TEXT_TEXTDIRECTIONHEURISTICS_H__
