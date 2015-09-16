// wuweizuo automatic build .cpp file from .java file.

#include "AndroidCertVerifyResult.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

//=====================================================================
//                       AndroidCertVerifyResult
//=====================================================================
AndroidCertVerifyResult::AndroidCertVerifyResult(
    /* [in] */ Int32 status,
    /* [in] */ Boolean isIssuedByKnownRoot,
    /* [in] */ IList<IX509Certificate*>* certificateChain)
{
    // ==================before translated======================
    // mStatus = status;
    // mIsIssuedByKnownRoot = isIssuedByKnownRoot;
    // mCertificateChain = new ArrayList<X509Certificate>(certificateChain);
}

AndroidCertVerifyResult::AndroidCertVerifyResult(
    /* [in] */ Int32 status)
{
    // ==================before translated======================
    // mStatus = status;
    // mIsIssuedByKnownRoot = false;
    // mCertificateChain = Collections.<X509Certificate>emptyList();
}

Int32 AndroidCertVerifyResult::GetStatus()
{
    // ==================before translated======================
    // return mStatus;
    assert(0);
    return 0;
}

Boolean AndroidCertVerifyResult::IsIssuedByKnownRoot()
{
    // ==================before translated======================
    // return mIsIssuedByKnownRoot;
    assert(0);
    return FALSE;
}

AutoPtr< ArrayOf< AutoPtr< ArrayOf<Byte> > > > AndroidCertVerifyResult::GetCertificateChainEncoded()
{
    // ==================before translated======================
    // byte[][] verifiedChainArray = new byte[mCertificateChain.size()][];
    // try {
    //     for (int i = 0; i < mCertificateChain.size(); i++) {
    //         verifiedChainArray[i] = mCertificateChain.get(i).getEncoded();
    //     }
    // } catch (CertificateEncodingException e) {
    //     return new byte[0][];
    // }
    // return verifiedChainArray;
    assert(0);
    AutoPtr< ArrayOf< AutoPtr< ArrayOf<Byte> > > > empty;
    return empty;
}

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


