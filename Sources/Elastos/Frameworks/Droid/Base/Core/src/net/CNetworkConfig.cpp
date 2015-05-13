
#include "ext/frameworkext.h"
#include "net/CNetworkConfig.h"
#include <Elastos.Core.h>
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * input string from config.xml resource.  Uses the form:
 * [Connection name],[ConnectivityManager connection type],
 * [associated radio-type],[priority],[dependencyMet]
 */
ECode CNetworkConfig::constructor(
    /* [in] */ const String& init)
{
    AutoPtr<ArrayOf<String> > fragments;
    StringUtils::Split(init, String(","), (ArrayOf<String>**)&fragments);
    mName = (*fragments)[0].Trim().ToLowerCase();
    mType = StringUtils::ParseInt32((*fragments)[1]);
    mRadio = StringUtils::ParseInt32((*fragments)[2]);
    mPriority = StringUtils::ParseInt32((*fragments)[3]);
    mRestoreTime = StringUtils::ParseInt32((*fragments)[4]);
    mDependencyMet = (*fragments)[5].EqualsIgnoreCase("TRUE");
    return NOERROR;
}

/**
 * Indicates if this network is supposed to be default-routable
 */
ECode CNetworkConfig::IsDefault(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (mType == mRadio);
    return NOERROR;
}

ECode CNetworkConfig::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode CNetworkConfig::SetName(
    /* [in] */ const String& name)
{
    mName = name;
    return NOERROR;
}

ECode CNetworkConfig::GetType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mType;
    return NOERROR;
}

ECode CNetworkConfig::SetType(
    /* [in] */ Int32 type)
{
    mType = type;
    return NOERROR;
}

ECode CNetworkConfig::GetRadio(
    /* [out] */ Int32* radio)
{
    VALIDATE_NOT_NULL(radio);
    *radio = mRadio;
    return NOERROR;
}

ECode CNetworkConfig::SetRadio(
    /* [in] */ Int32 radio)
{
    mRadio = radio;
    return NOERROR;
}

ECode CNetworkConfig::GetPriority(
    /* [out] */ Int32* priority)
{
    VALIDATE_NOT_NULL(priority);
    *priority = mPriority;
    return NOERROR;
}

ECode CNetworkConfig::SetPriority(
    /* [in] */ Int32 priority)
{
    mPriority = priority;
    return NOERROR;
}

ECode CNetworkConfig::GetDependencyMet(
    /* [out] */ Boolean* dependencyMet)
{
    VALIDATE_NOT_NULL(dependencyMet);
    *dependencyMet = mDependencyMet;
    return NOERROR;
}

ECode CNetworkConfig::SetDependencyMet(
    /* [in] */ Boolean dependencyMet)
{
    mDependencyMet = dependencyMet;
    return NOERROR;
}

ECode CNetworkConfig::GetRestoreTime(
    /* [out] */ Int32* restoretime)
{
    VALIDATE_NOT_NULL(restoretime);
    *restoretime = mRestoreTime;
    return NOERROR;
}

ECode CNetworkConfig::SetRestoreTime(
    /* [in] */ Int32 restoretime)
{
    mRestoreTime = restoretime;
    return NOERROR;
}
} // namespace Net
} // namepsace Droid
} // namespace Elastos
