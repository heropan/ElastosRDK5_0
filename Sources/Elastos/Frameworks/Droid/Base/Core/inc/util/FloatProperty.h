#ifndef __FLOATPROPERTY_H__
#define  __FLOATPROPERTY_H__

#include "Elastos.Droid.Core_server.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Utility {

class FloatProperty
    : public ElRefBase
    , public IFloatProperty
{
public :
    CAR_INTERFACE_DECL()

    FloatProperty(
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
        /* [out] */ ClassID* id);

    CARAPI IsWriteOnly(
        /* [out] */ Boolean* writeOnly);

    CARAPI ForbiddenOperate(
        /* [out] */ Boolean* forbidden);

    CARAPI SetValue(
        /* [in] */ IInterface* obj,
        /* [in] */ Float value) = 0;
private:
    String mName;
    ClassID mClsId;
};

} // namespace Utility
} // namepsace Droid
} // namespace Elastos

#endif
