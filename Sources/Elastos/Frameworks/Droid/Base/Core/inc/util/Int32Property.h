#ifndef __ELASTOS_DROID_UTILITY_INT32PROPERTY_H__
#define __ELASTOS_DROID_UTILITY_INT32PROPERTY_H__

#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Utility {

class Int32Property
    : public ElRefBase
    , public IInt32Property
{
public :
    CAR_INTERFACE_DECL()

    Int32Property(
        /* [in] */ const String& name);

    CARAPI IsReadOnly(
        /* [out] */ Boolean* readOnly);

    CARAPI Set(
        /* [in] */ IInterface* obj,
        /* [in] */ IInterface* value);

    CARAPI Get(
        /* [in] */ IInterface* obj,
        /* [out] */ IInterface** rst) = 0;

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetType(
        /* [out] */ ClassID* info);

    CARAPI IsWriteOnly(
        /* [out] */ Boolean* writeOnly);

    CARAPI ForbiddenOperate(
        /* [out] */ Boolean* forbidden);

    CARAPI SetValue(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 value) = 0;
private:
    String mName;
    ClassID mClsId;
};

} // namespace Utility
} // namepsace Droid
} // namespace Elastos

#endif
