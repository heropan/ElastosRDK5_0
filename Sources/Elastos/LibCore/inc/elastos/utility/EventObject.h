#ifndef __UTILITY_EVENTOBJECT_H__
#define __UTILITY_EVENTOBJECT_H__

#include "Object.h"

using Elastos::Core::Object;

namespace Elastos {
namespace Utility {

class EventObject
    : public Object
    , public IEventObject
{
public:
    CAR_INTERFACE_DECL();

    /**
     * Constructs a new instance of this class.
     *
     * @param source
     *            the object which fired the event.
     */
    EventObject(
        /* [in] */ IObject* source);

    /**
     * Returns the object which fired the event.
     */
    virtual CARAPI GetSource(
        /* [out] */ IObject** object);

    /**
     * Returns the string representation of this {@code EventObject}.
     */
    CARAPI ToString(
        /* [in] */ String* str);

protected:
    /*transient*/ AutoPtr<IObject> mSource;
};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_EVENTOBJECT_H__
