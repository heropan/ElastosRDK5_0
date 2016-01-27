#include "Elastos.Droid.Media.h"
#include "elastos/droid/media/Utils.h"
#include "elastos/droid/utility/CPair.h"
#include "elastos/droid/utility/CSize.h"
#include "elastos/droid/utility/CRational.h"
#include <elastos/utility/Arrays.h>
#include <elastos/core/CoreUtils.h>

using Elastos::Droid::Utility::CPair;
using Elastos::Droid::Utility::CSize;
using Elastos::Droid::Utility::CRational;
using Elastos::Droid::Utility::IPair;
using Elastos::Droid::Utility::ISize;
using Elastos::Core::IComparable;

namespace Elastos{
namespace Droid {
namespace Media {
//================================================================================
//                      Utils::InterfaceCompare
//================================================================================
Utils::InterfaceCompare::InterfaceCompare()
{}

Utils::InterfaceCompare::~InterfaceCompare()
{}

CAR_INTERFACE_IMPL(Utils::InterfaceCompare, Object, IComparator)

ECode Utils::InterfaceCompare::Compare(
    /* [in] */ IInterface* lhs,
    /* [in] */ IInterface* rhs,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IRange> lhs_ = IRange::Probe(lhs);
    AutoPtr<IRange> rhs_ = IRange::Probe(rhs);
    AutoPtr<IInterface> lupper;
    lhs_->GetUpper((IInterface**)&lupper);
    AutoPtr<IInterface> llower;
    lhs_->GetLower((IInterface**)&llower);
    AutoPtr<IInterface> rupper;
    rhs_->GetUpper((IInterface**)&rupper);
    AutoPtr<IInterface> rlower;
    rhs_->GetLower((IInterface**)&rlower);
    Int32 vol;
    IComparable::Probe(lupper)->CompareTo(rlower.Get(), &vol);
    if (vol < 0) {
        *result = -1;
        return NOERROR;
    } else if ((llower->CompareTo(rupper, &vol), vol > 0)) {
        *result = 1;
        return NOERROR;
    }
    Slogger::E("Utils::InterfaceCompare", "sample rate ranges must be distinct (%p and %p", lhs, rhs);
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

//================================================================================
//                      Utils
//================================================================================
const String Utils::TAG(String("Utils"));

void Utils::SortDistinctRanges(
    /* [in] */ ArrayOf<IRange*>* ranges)
{
    AutoPtr<IComparator> i = new InterfaceCompare();
    Arrays::Sort(ranges, i.Get());
}

AutoPtr<ArrayOf<IRange*> > Utils::IntersectSortedDistinctRanges(
    /* [in] */ ArrayOf<IRange*>* one,
    /* [in] */ ArrayOf<IRange*>* another)
{
    Int32 ix = 0;
    AutoPtr<IVector> result;
    Int32 length = another->GetLength();
    for (Int32 i = 0; i < length; i++/*range: another*/) {
        AutoPtr<IRange> range = (*another)[i];
        Int32 oneLength;
        AutoPtr<IInterface> oneUpper;
        AutoPtr<IInterface> oneLower;
        AutoPtr<IInterface> rangeLower;
        AutoPtr<IInterface> rangeUpper;
        Int32 ret;
        while ((ix < (oneLength = one->GetLength(), oneLength)) &&
                ((*one)[ix]->GetUpper((IInterface**)&oneUpper),
                    (IComparable::Probe(oneUpper)->CompareTo((range->GetLower((IInterface**)&rangeLower), rangeLower.Get()), &ret), ret < 0))) {
            ++ix;
            oneUpper = NULL;
            rangeLower = NULL;
        }
        AutoPtr<IRange> r;
        while ((ix < (oneLength = one->GetLength(), oneLength)) &&
                ((*one)[ix]->GetUpper((IInterface**)&oneUpper),
                (IComparable::Probe(oneUpper)->CompareTo((range->GetUpper((IInterface**)&rangeUpper), rangeUpper.Get()), &ret), ret < 0))) {
            r = NULL;
            range->Intersect((*one)[ix], (IRange**)&r);
            result->Add(r.Get());
            ++ix;
            oneUpper = NULL;
            rangeUpper = NULL;
        }
        Int32 oLength = one->GetLength();
        if (ix == oLength) {
            break;
        }
        oneLower = NULL;
        rangeUpper = NULL;
        (*one)[ix]->GetLower((IInterface**)&oneLower);
        range->GetUpper((IInterface**)&rangeUpper);
        Int32 value;
        IComparable::Probe(oneLower)->CompareTo(rangeUpper.Get(), &value);
        if (value <= 0) {
            r = NULL;
            range->Intersect((*one)[ix], (IRange**)&r);
            result->Add(r.Get());
        }
    }
    Int32 size;
    result->GetSize(&size);
    AutoPtr<ArrayOf<IRange*> > outarray;
    AutoPtr<ArrayOf<IRange*> > inArray = ArrayOf<IRange*>::Alloc(size);
    return result->ToArray(inArray, (ArrayOf<IInterface *>**)&outarray);
}

Int32 Utils::BinarySearchDistinctRanges(
    /* [in] */ ArrayOf<IRange*>* ranges,
    /* [in] */ IInterface* value)
{
    assert(0 && "TODO");
}

Int32 Utils::Gcd(
    /* [in] */ Int32 a,
    /* [in] */ Int32 b)
{
    if (a == 0 && b == 0) {
        return 1;
    }
    if (b < 0) {
        b = -b;
    }
    if (a < 0) {
        a = -a;
    }
    while (a != 0) {
        Int32 c = b % a;
        b = a;
        a = c;
    }
    return b;
}

AutoPtr<Range<IInteger32> > Utils::FactorRange(
    /* [in] */ Range<IInteger32>* range,
    /* [in] */ Int32 factor)
{
    if (factor == 1) {
        return range;
    }

    AutoPtr<IInterface> obj;
    range->GetLower((IInterface**)&obj);
    AutoPtr<IInteger32> interLower = IInteger32::Probe(obj);
    Int32 lower;
    interLower->GetValue(&lower);

    obj = NULL;
    range->GetUpper((IInterface**)&obj);
    AutoPtr<IInteger32> interUpper = IInteger32::Probe(obj);
    Int32 upper;
    interLower->GetValue(&upper);
    Int32 lv = DivUp(lower, factor);
    Int32 rv = upper / factor;
    AutoPtr<Range<IInteger32> > range = Range::Create(CoreUtils::Convert(lv).Get(), CoreUtils::Convert(rv).Get());
    return range;
}

AutoPtr<Range<IInteger64> > Utils::FactorRange(
    /* [in] */ Range<IInteger64>* range,
    /* [in] */ Int64 factor)
{
    if (factor == 1) {
        return range;
    }

    AutoPtr<IInterface> obj;
    range->GetLower((IInterface**)&obj);
    AutoPtr<IInteger64> interLower = IInteger64::Probe(obj);
    Int64 lower;
    interLower->GetValue(&lower);

    obj = NULL;
    range->GetUpper((IInterface**)&obj);
    AutoPtr<IInteger64> interUpper = IInteger64::Probe(obj);
    Int64 upper;
    interLower->GetValue(&upper);
    Int64 lv = DivUp(lower, factor);
    Int64 rv = upper / factor;
    AutoPtr<Range<IInteger64> > range = Range::Create(CoreUtils::Convert(lv).Get(), CoreUtils::Convert(rv).Get());
    return range;

}

AutoPtr<Range<CRational> > Utils::ScaleRange(
    /* [in] */ Range<CRational> >* range,
    /* [in] */ Int32 num,
    /* [in] */ Int32 den);
{
    if (num == den) {
        return range;
    }
    AutoPtr<CRational> lower = range->GetLower();
    AutoPtr<CRational> upper = range->GetUpper();

    return Range::Create(
            ScaleRatio(lower.Get(), num, den).Get(),
            ScaleRatio(upper.Get(), num, den).Get());
}

AutoPtr<Range<IInteger32> > Utils::AlignRange(
    /* [in] */ Range<IInteger32>* range,
    /* [in] */ Int32 align)
{
    AutoPtr<IInteger32> lower = range->GetLower();
    Int32 iLower;
    lower->GetValue(&iLower);

    AutoPtr<IInteger32> upper = range->GetUpper();
    Int32 iUpper;
    upper->GetValue(&iUpper);

    return range->Intersect(
            DivUp(iLower, align) * align,
            (iUpper / align) * align);
}

Int32 Utils::DivUp(
    /* [in] */ Int32 num,
    /* [in] */ Int32 den)
{
    return (num + den - 1) / den;
}

AutoPtr<Range<IInteger32> > Utils::IntRangeFor(
    /* [in] */ Double v)
{
    return Range::Create(CoreUtils::Convert((Int32)v).Get(),
        CoreUtils::Convert((Int32)Elastos::Core::Math::Ceil(v)).Get());
}

AutoPtr<Range<IInteger64> > Utils::Int64RangeFor(
    /* [in] */ Double v)
{
    return Range::Create(CoreUtils::Convert((Int64)v).Get(), CoreUtils::Convert((Int64)Elastos::Core::Math::Ceil(v)));
}

AutoPtr<ISize> Utils::ParseSize(
    /* [in] */ IInterface* o,
    /* [in] */ ISize* fallback)
{
    // try {
        return Size.parseSize((String) o);
    // } catch (ClassCastException e) {
    // } catch (NumberFormatException e) {
    // } catch (NullPointerException e) {
        // return fallback;
    // }
    Slogger::W(TAG, "could not parse size '%p'", o );
    return fallback;
}

Int32 Utils::ParseIntSafely(
    /* [in] */ IInterface* o,
    /* [in] */ Int32 fallback)
{
    // try {
    AutoPtr<ICharSequence> cs = ICharSequence::Probe(o);
    String s;
    cs->ToString(&s);
    return StringUtils::ParseInt32(s);
    // } catch (ClassCastException e) {
    // } catch (NumberFormatException e) {
    // } catch (NullPointerException e) {
    //     return fallback;
    // }
    // Slogger::W(TAG, "could not parse integer '%p'", o);
    // return fallback;
}

AutoPtr<Range<IInteger32> > Utils::ParseIntRange(
    /* [in] */ IInterface* o,
    /* [in] */ Range<IInteger32>* fallback)
{
    // try {
    AutoPtr<ICharSequence> cs = ICharSequence::Probe(o);
    String s;
    cs->ToString(&s);
    Int32 ix = s.IndexOf('-');
    if (ix >= 0) {
        AutoPtr<Range<IInteger32> > range = Range::Create(
                CoreUtils::Convert(StringUtils::ParseInt32(s.Substring(0, ix), 10)).Get(),
                CoreUtils::Convert(StringUtils::ParseInt32(s.Substring(ix + 1), 10)).Get());
        return range;
    }
    Int32 value = StringUtils::ParseInt32(s);
    AutoPtr<Range<IInteger32> > r = Range::Create(CoreUtils::Convert(value).Get(), CoreUtils::Convert(value).Get());
    return r;
    // } catch (ClassCastException e) {
    // } catch (NumberFormatException e) {
    // } catch (NullPointerException e) {
    //     return fallback;
    // } catch (IllegalArgumentException e) {
    // }
    // Slogger::W(TAG, "could not parse integer range '%p'", o);
    // return fallback;
}

AutoPtr<Range<IInteger64> > Utils::ParseInt64Range(
    /* [in] */ IInterface* o,
    /* [in] */ Range<IInteger64>* fallback)
{
    // try {
    String s = (String)o;
    Int32 ix = s.IndexOf('-');
    if (ix >= 0) {
        AutoPtr<Range<IInteger64> > range = Range::Create(
                CoreUtils::Convert(StringUtils::ParseInt64((s.Substring(0, ix), 10))).Get(),
                CoreUtils::Convert(StringUtils::ParseInt64((s.Substring(ix + 1), 10))).Get());
        return range;
    }
    Int64 value = StringUtils::ParseInt64(s);
    AutoPtr<Range<IInteger64> > r = Range::Create(CoreUtils::Convert(value).Get(), CoreUtils::Convert(value).Get());
    return r;
    // } catch (ClassCastException e) {
    // } catch (NumberFormatException e) {
    // } catch (NullPointerException e) {
    //     return fallback;
    // } catch (IllegalArgumentException e) {
    // }
    // Slogger::W(TAG, "could not parse long range '%p'", o);
    // return fallback;
}

AutoPtr<Range<CRational> > Utils::ParseRationalRange(
    /* [in] */ IInterface* o,
    /* [in] */ Range<CRational>* fallback)
{
    // try {
        String s = (String)o;
        Int32 ix = s.IndexOf('-');
        if (ix >= 0) {
            return Range.create(
                    Rational.parseRational(s.substring(0, ix)),
                    Rational.parseRational(s.substring(ix + 1)));
        }
        Rational value = Rational.parseRational(s);
        return Range.create(value, value);
    // } catch (ClassCastException e) {
    // } catch (NumberFormatException e) {
    // } catch (NullPointerException e) {
    //     return fallback;
    // } catch (IllegalArgumentException e) {
    // }
    // Slogger::W(TAG, "could not parse rational range '%p'", o);
    // return fallback;
}

AutoPtr<IPair> Utils::ParseSizeRange(
    /* [in] */ IInterface* o)
{
    // try {
    String s = (String)o;
    Int32 ix = s.IndexOf('-');
    if (ix >= 0) {
        AutoPtr<ISize> lv;
        CSize::ParseSize(s.Substring(0, ix), (ISize**)&lv);
        AutoPtr<ISize> rv;
        CSize::ParseSize(s.Substring(ix + 1), (ISize**)&rv);
        AutoPtr<IPair> pair;
        CPair::Create(lv.Get(), rv.Get(), (IPair**)&pair);
        return pair;
    }

    AutoPtr<ISize> value;
    CSize::ParseSize(s, (ISize**)&value);
    AutoPtr<IPair> p;
    CPair::Create(value.Get(), value.Get(), (IPair**)&p);
    return p;
    // } catch (ClassCastException e) {
    // } catch (NumberFormatException e) {
    // } catch (NullPointerException e) {
    //     return null;
    // } catch (IllegalArgumentException e) {
    // }
    // Slogger::W(TAG, "could not parse size range '%p'", o);
    // return NULL;
}

Utils::Utils()
{}

AutoPtr<CRational> Utils::ScaleRatio(
    /* [in] */ CRational* ratio,
    /* [in] */ Int32 num,
    /* [in] */ Int32 den)
{
    Int32 common = Gcd(num, den);
    num /= common;
    den /= common;

    Int32 numberator;
    ratio->GetNumerator(&numberator);
    Int32 denominator;
    ratio->GetDenominator(&denominator);
    AutoPtr<CRational> cr;
    CRational::NewByFriend((Int32)(numberator * (Double)num),
        (Int32)(denominator * (Double)den), (CRational**)&cr);
    return cr;
}

Int64 Utils::DivUp(
    /* [in] */ Int64 num,
    /* [in] */ Int64 den)
{
    return (num + den - 1) / den;
}

/**
 * Returns least common multiple
 */
Int64 Utils::Lcm(
    /* [in] */ Int32 a,
    /* [in] */ Int32 b)
{
    return (num + den - 1) / den;
}

} // namespace Elastos
} // namespace Droid
} // namespace Media