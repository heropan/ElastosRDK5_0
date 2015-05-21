
#ifndef __CBOOLEAN_H__
#define __CBOOLEAN_H__

#include "_CBoolean.h"

using Elastos::IO::ISerializable;
using Elastos::Core::IComparable;

namespace Elastos {
namespace Core {

CarClass(CBoolean) ,public ISerializable, public IComparable
{
public:
    CARAPI constructor(
        /* [in] */ Boolean value);

    CARAPI GetValue(
        /* [out] */ Boolean* result);

    CARAPI CompareTo(
        /* [in] */ IInterface* other,
        /* [out] */ Int32* result);

    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI ToString(
        /* [out] */ String* result);

private:
    Boolean mValue;
};

} // namespace Core
} // namespace Elastos

#endif //__CBOOLEAN_H__
