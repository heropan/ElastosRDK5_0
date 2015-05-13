#include "ext/frameworkext.h"
#include "view/CPointerProperties.h"
#include <elastos/Math.h>
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace View {

CPointerProperties::CPointerProperties()
    : mId(0)
    , mToolType(0)
{
}

ECode CPointerProperties::constructor()
{
    Clear();
    return NOERROR;
}

ECode CPointerProperties::constructor(
    /* [in] */ IPointerProperties* other)
{
    VALIDATE_NOT_NULL(other);
    other->GetId(&mId);
    other->GetToolType(&mToolType);
    return NOERROR;
}

ECode CPointerProperties::SetId(
    /* [in] */ Int32 id)
{
    mId = id;
    return NOERROR;
}

ECode CPointerProperties::GetId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mId;
    return NOERROR;
}

ECode CPointerProperties::SetToolType(
    /* [in] */ Int32 type)
{
    mToolType = type;
    return NOERROR;
}

ECode CPointerProperties::GetToolType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mToolType;
    return NOERROR;
}

ECode CPointerProperties::Clear()
{
    mId = IMotionEvent::INVALID_POINTER_ID;
    mToolType = IMotionEvent::TOOL_TYPE_UNKNOWN;
    return NOERROR;
}

ECode CPointerProperties::CopyFrom(
    /* [in] */ IPointerProperties* other)
{
    VALIDATE_NOT_NULL(other);
    other->GetId(&mId);
    other->GetToolType(&mToolType);
    return NOERROR;
}

ECode CPointerProperties::EqualsEx(
    /* [in] */ IPointerProperties* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    if (other == NULL) {
        return NOERROR;
    }

    Int32 id, toolType;
    other->GetId(&id);
    other->GetToolType(&toolType);
    *result = (mId == id && mToolType == toolType);
    return NOERROR;
}

ECode CPointerProperties::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* result)
{
    return EqualsEx(IPointerProperties::Probe(other), result);
}

ECode CPointerProperties::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    *hash = (mId | (mToolType << 8));
    return NOERROR;
}

}
}
}
