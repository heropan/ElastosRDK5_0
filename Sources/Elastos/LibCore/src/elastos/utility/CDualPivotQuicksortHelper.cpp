
#include "CDualPivotQuicksortHelper.h"
#include "DualPivotQuicksort.h"

namespace Elastos {
namespace Utility {

ECode CDualPivotQuicksortHelper::SortInt32(
    /* [in] */ ArrayOf<Int32>* a)
{
    return DualPivotQuicksort::Sort(a);
}

ECode CDualPivotQuicksortHelper::SortInt32(
    /* [in] */ ArrayOf<Int32>* a,
    /* [in] */ Int32 fromIndex,
    /* [in] */ Int32 toIndex)
{
    return DualPivotQuicksort::Sort(a, fromIndex, toIndex);
}

ECode CDualPivotQuicksortHelper::SortInt64(
    /* [in] */ ArrayOf<Int64>* a)
{
    return DualPivotQuicksort::Sort(a);
}

ECode CDualPivotQuicksortHelper::SortInt64(
    /* [in] */ ArrayOf<Int64>* a,
    /* [in] */ Int32 fromIndex,
    /* [in] */ Int32 toIndex)
{
    return DualPivotQuicksort::Sort(a, fromIndex, toIndex);
}

ECode CDualPivotQuicksortHelper::SortInt16(
    /* [in] */ ArrayOf<Int16>* a)
{
    return DualPivotQuicksort::Sort(a);
}

ECode CDualPivotQuicksortHelper::SortInt16(
    /* [in] */ ArrayOf<Int16>* a,
    /* [in] */ Int32 fromIndex,
    /* [in] */ Int32 toIndex)
{
    return DualPivotQuicksort::Sort(a, fromIndex, toIndex);
}

ECode CDualPivotQuicksortHelper::SortChar(
    /* [in] */ ArrayOf<Char32>* a)
{
    return DualPivotQuicksort::Sort(a);
}

ECode CDualPivotQuicksortHelper::SortChar(
    /* [in] */ ArrayOf<Char32>* a,
    /* [in] */ Int32 fromIndex,
    /* [in] */ Int32 toIndex)
{
    return DualPivotQuicksort::Sort(a, fromIndex, toIndex);
}

ECode CDualPivotQuicksortHelper::SortByte(
    /* [in] */ ArrayOf<Byte>* a)
{
    return DualPivotQuicksort::Sort(a);
}

ECode CDualPivotQuicksortHelper::SortByte(
    /* [in] */ ArrayOf<Byte>* a,
    /* [in] */ Int32 fromIndex,
    /* [in] */ Int32 toIndex)
{
    return DualPivotQuicksort::Sort(a, fromIndex, toIndex);
}

ECode CDualPivotQuicksortHelper::SortFloat(
    /* [in] */ ArrayOf<Float>* a)
{
    return DualPivotQuicksort::Sort(a);
}

ECode CDualPivotQuicksortHelper::SortFloat(
    /* [in] */ ArrayOf<Float>* a,
    /* [in] */ Int32 fromIndex,
    /* [in] */ Int32 toIndex)
{
    return DualPivotQuicksort::Sort(a, fromIndex, toIndex);
}

ECode CDualPivotQuicksortHelper::SortDouble(
    /* [in] */ ArrayOf<Double>* a)
{
    return DualPivotQuicksort::Sort(a);
}

ECode CDualPivotQuicksortHelper::SortDouble(
    /* [in] */ ArrayOf<Double>* a,
    /* [in] */ Int32 fromIndex,
    /* [in] */ Int32 toIndex)
{
    return DualPivotQuicksort::Sort(a, fromIndex, toIndex);
}

} // namespace Utility
} // namespace Elastos
