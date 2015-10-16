
#include "elastos/droid/webkit/CDateSorter.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CDateSorter::constructor(
    /* [in] */ IContext* context)
{
    DateSorter::Init(context);
    return NOERROR;
}

ECode CDateSorter::GetIndex(
    /* [in] */ Int64 time,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = DateSorter::GetIndex(time);
    return NOERROR;
}

ECode CDateSorter::GetLabel(
    /* [in] */ Int32 index,
    /* [out] */ String* lable)
{
    VALIDATE_NOT_NULL(lable);
    *lable = DateSorter::GetLabel(index);
    return NOERROR;
}

ECode CDateSorter::GetBoundary(
    /* [in] */ Int32 index,
    /* [out] */ Int64* boundary)
{
    VALIDATE_NOT_NULL(boundary);
    *boundary = DateSorter::GetBoundary(index);
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos