
#include "EventObject.h"
#include "StringUtils.h"

using Elastos::Core::StringUtils;

namespace Elastos{
namespace Utility{

extern "C" const InterfaceID EIID_EventObject =
        { 0x43b4b99c, 0xab5b, 0x4593, { 0x99, 0x8f, 0x57, 0x27, 0x6f, 0x0e, 0x25, 0x73 } };

CAR_INTERFACE_IMPL(EventObject, Object, IEventObject);

EventObject::EventObject(
    /* [in] */ IObject* source)
{
    if (mSource == NULL) {
        // throw new IllegalArgumentException("source == null");
        assert(0 && "source == null");
    }
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
