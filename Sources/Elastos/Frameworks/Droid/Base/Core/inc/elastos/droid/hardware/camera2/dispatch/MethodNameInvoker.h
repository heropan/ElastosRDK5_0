
#ifndef __ELASTOS_DROID_HARDWARE_CAMERA2_DISPATCH_METHODNAMEINVOKER_H__
#define __ELASTOS_DROID_HARDWARE_CAMERA2_DISPATCH_METHODNAMEINVOKER_H__

#include "Elastos.Droid.Core_server.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Utility::Concurrent::IConcurrentHashMap;
using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Camera2 {
namespace Dispatch {

class MethodNameInvoker
    : public Object
    , public IMethodNameInvoker
{
public:
    CAR_INTERFACE_DECL()

    /**
     * Create a dispatcher that does nothing when dispatched to.
     */
    MethodNameInvoker();

    virtual ~MethodNameInvoker() {}

    CARAPI constructor();

    /**
     * Create a new method name invoker.
     *
     * @param target destination dispatch type, invokes will be redirected to this dispatcher
     * @param targetClass destination dispatch class, the invoked methods will be from this class
     */
    CARAPI constructor(
        /* [in] */ IDispatchable* target,
        /* [in] */ IClassInfo* targetClass);

    /**
     * Invoke a method by its name.
     *
     * <p>If more than one method exists in {@code targetClass}, the first method with the right
     * number of arguments will be used, and later calls will all use that method.</p>
     *
     * @param methodName
     *          The name of the method, which will be matched 1:1 to the destination method
     * @param params
     *          Variadic parameter list.
     * @return
     *          The same kind of value that would normally be returned by calling {@code methodName}
     *          statically.
     *
     * @throws IllegalArgumentException if {@code methodName} does not exist on the target class
     * @throws Throwable will rethrow anything that the target method would normally throw
     */
    //@SuppressWarnings("unchecked")
    CARAPI Invoke(
        /* [in] */ const String& methodName,
        /* [in] */ ArrayOf<IInterface*>* params);

private:
    AutoPtr<IDispatchable> mTarget;
    AutoPtr<IClassInfo> mTargetClass;
    AutoPtr<IConcurrentHashMap> mMethods;
};

} // namespace Dispatch
} // namespace Camera2
} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_HARDWARE_CAMERA2_DISPATCH_METHODNAMEINVOKER_H__
