
#include "elastos/droid/internal/utility/ArrayUtils.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

AutoPtr<ArrayOf<Byte> > ArrayUtils::NewUnpaddedByteArray(
    /* [in] */ Int32 minLen)
{
    AutoPtr<ArrayOf<Byte> > array = ArrayOf<Byte>::Alloc(minLen);
    return array;
}

AutoPtr<ArrayOf<Char32> > ArrayUtils::NewUnpaddedChar32Array(
    /* [in] */ Int32 minLen)
{
    AutoPtr<ArrayOf<Char32> > array = ArrayOf<Char32>::Alloc(minLen);
    return array;
}

AutoPtr<ArrayOf<Int32> > ArrayUtils::NewUnpaddedInt32Array(
    /* [in] */ Int32 minLen)
{
    AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(minLen);
    return array;
}

AutoPtr<ArrayOf<Int64> > ArrayUtils::NewUnpaddedInt64Array(
    /* [in] */ Int32 minLen)
{
    AutoPtr<ArrayOf<Int64> > array = ArrayOf<Int64>::Alloc(minLen);
    return array;
}

AutoPtr<ArrayOf<Boolean> > ArrayUtils::NewUnpaddedBooleanArray(
    /* [in] */ Int32 minLen)
{
    AutoPtr<ArrayOf<Boolean> > array = ArrayOf<Boolean>::Alloc(minLen);
    return array;
}

AutoPtr<ArrayOf<Float> > ArrayUtils::NewUnpaddedFloatArray(
    /* [in] */ Int32 minLen)
{
    AutoPtr<ArrayOf<Float> > array = ArrayOf<Float>::Alloc(minLen);
    return array;
}

AutoPtr<ArrayOf<Double> > ArrayUtils::NewUnpaddedDoubleArray(
    /* [in] */ Int32 minLen)
{
    AutoPtr<ArrayOf<Double> > array = ArrayOf<Double>::Alloc(minLen);
    return array;
}

AutoPtr<ArrayOf<IInterface*> > ArrayUtils::NewUnpaddedObjectArray(
    /* [in] */ Int32 minLen)
{
    AutoPtr<ArrayOf<IInterface*> > array = ArrayOf<IInterface*>::Alloc(minLen);
    return array;
}



} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos