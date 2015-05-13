
#include "CCertPathValidatorHelper.h"
#include "CertPathValidator.h"

namespace Elastos {
namespace Security {
namespace Cert {


ECode CCertPathValidatorHelper::GetInstance(
    /* [in] */ const String& algorithm,
    /* [out] */ ICertPathValidator **validator)
{
    return CertPathValidator::GetInstance(algorithm, validator);
}

ECode CCertPathValidatorHelper::GetInstanceEx(
    /* [in] */ const String& algorithm,
    /* [in] */ const String& provider,
    /* [out] */ ICertPathValidator **validator)
{
    return CertPathValidator::GetInstance(algorithm, provider, validator);
}

ECode CCertPathValidatorHelper::GetInstanceEx2(
    /* [in] */ const String& algorithm,
    /* [in] */ IProvider *provider,
    /* [out] */ ICertPathValidator **validator)
{
    return CertPathValidator::GetInstance(algorithm, provider, validator);
}

ECode CCertPathValidatorHelper::GetDefaultType(
    /* [out] */ String *type)
{
    return CertPathValidator::GetDefaultType(type);
}

}
}
}

