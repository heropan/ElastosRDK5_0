
#include "cmdef.h"
#include "CParsePosition.h"

namespace Elastos {
namespace Text {

CParsePosition::CParsePosition()
    : mCurrentPosition(0)
    , mErrorIndex(-1)
{}

ECode CParsePosition::constructor(
    /* [in] */ Int32 index)
{
    mCurrentPosition = index;
    return NOERROR;
}

ECode CParsePosition::GetErrorIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    *index = mErrorIndex;
    return NOERROR;
}

ECode CParsePosition::GetIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    *index = mCurrentPosition;
    return NOERROR;
}

ECode CParsePosition::SetErrorIndex(
    /* [in] */ Int32 index)
{
    mErrorIndex = index;
    return NOERROR;
}

ECode CParsePosition::SetIndex(
    /* [in] */ Int32 index)
{
    mCurrentPosition = index;
    return NOERROR;
}

} // namespace Text
} // namespace Elastos
