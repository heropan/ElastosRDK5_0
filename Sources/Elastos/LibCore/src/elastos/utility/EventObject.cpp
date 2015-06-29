
#include "EventObject.h"
#include "StringUtils.h"

using Elastos::Core::StringUtils;
using Elastos::IO::EIID_ISerializable;

namespace Elastos{
namespace Utility{

CAR_INTERFACE_IMPL_2(EventObject, Object, IEventObject, ISerializable);

EventObject::EventObject()
{
}

EventObject::~EventObject()
{
}

EventObject::constructor(
    /* [in] */ IObject* source)
{
    VALIDATE_NOT_NULL(source)
    mSource = source;
}

ECode EventObject::GetSource(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    *object = mSource;
    REFCOUNT_ADD(*object);
    return NOERROR;
}

ECode EventObject::ToString(
    /* [in] */ String* str)
{
    // return getClass().getName() + "[source=" + source + ']';
    VALIDATE_NOT_NULL(str);
    *str = String("this is:") + StringUtils::ToHexString((Int32)this)
            + "[source=" + StringUtils::ToHexString((Int32)mSource.Get()) + ']';
    return NOERROR;
}

} // namespace Utility
} // namespace Elastos
