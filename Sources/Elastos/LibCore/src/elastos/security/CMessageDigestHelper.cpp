
#include "CMessageDigestHelper.h"
#include "MessageDigest.h"

namespace Elastos {
namespace Security {

ECode CMessageDigestHelper::GetInstance(
    /* [in] */ const String& algorithm,
    /* [out] */ IMessageDigest** instance)
{
    return MessageDigest::GetInstance(algorithm, instance);
}

ECode CMessageDigestHelper::GetInstanceEx(
    /* [in] */ const String& algorithm,
    /* [in] */ const String& provider,
    /* [out] */ IMessageDigest** instance)
{
    return MessageDigest::GetInstanceEx(algorithm, provider, instance);
}

ECode CMessageDigestHelper::GetInstanceEx2(
    /* [in] */ const String& algorithm,
    /* [in] */ IProvider* provider,
    /* [out] */ IMessageDigest** instance)
{
    return MessageDigest::GetInstanceEx2(algorithm, provider, instance);
}

ECode CMessageDigestHelper::IsEqual(
    /* [in] */ ArrayOf<Byte>* digesta,
    /* [in] */ ArrayOf<Byte>* digestb,
    /* [out] */ Boolean* result)
{
    return MessageDigest::IsEqual(digesta, digestb, result);
}

} // namespace Security
} // namespace Elastos