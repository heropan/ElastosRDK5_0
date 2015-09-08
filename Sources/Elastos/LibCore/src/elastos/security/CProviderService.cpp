
#include "CProviderService.h"

namespace Elastos {
namespace Security {

CAR_INTERFACE_IMPL(CProviderService, Object, IProviderService)

CAR_OBJECT_IMPL(CProviderService)

ECode CProviderService::constructor(
    /* [in] */ IProvider* provider,
    /* [in] */ const String& type,
    /* [in] */ const String& algorithm,
    /* [in] */ const String& className,
    /* [in] */ IList* aliases,
    /* [in] */ IMap* attributes)
{
    return NOERROR;
}

ECode CProviderService::AddAlias(
    /* [in] */ const String& alias)
{
    return NOERROR;
}

ECode CProviderService::PutAttribute(
    /* [in] */ const String& name,
    /* [in] */ const String& value)
{
    return NOERROR;
}

ECode CProviderService::GetType(
    /* [out] */ String* type)
{
    return NOERROR;
}

ECode CProviderService::GetAlgorithm(
    /* [out] */ String* algorithm)
{
    return NOERROR;
}

ECode CProviderService::GetProvider(
    /* [out] */ IProvider** provider)
{
    return NOERROR;
}

ECode CProviderService::GetClassName(
    /* [out] */ String* className)
{
    return NOERROR;
}

ECode CProviderService::SetClassName(
    /* [in] */ const String& className)
{
    return NOERROR;
}

ECode CProviderService::GetAttribute(
    /* [in] */ const String& name,
    /* [out] */ String* attrib)
{
    return NOERROR;
}

ECode CProviderService::GetAliases(
    /* [out] */ IList** aliases)
{
    return NOERROR;
}

ECode CProviderService::NewInstance(
    /* [in] */ IInterface* constructorParameter,
    /* [out] */ IInterface** instance)
{
    return NOERROR;
}

ECode CProviderService::SupportsParameter(
    /* [in] */ IInterface* parameter,
    /* [out] */ Boolean* isSupported)
{
    return NOERROR;
}

ECode CProviderService::GetAttributes(
    /* [out] */ IMap** attributes)
{
    return NOERROR;
}

}
}
