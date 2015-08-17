
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

//===============================================================
//                  AndroidCertVerifyResult
//===============================================================
AndroidCertVerifyResult::AndroidCertVerifyResult(
     /* [in] */ Int32 status,
     /* [in] */ Boolean isIssuedByKnownRoot,
     /* [in] */ List< AutoPtr<IX509Certificate> >* certificateChain)
     : mStatus(status)
     , mIsIssuedByKnownRoot(isIssuedByKnownRoot)
     , mCertificateChain(certificateChain)
{
}

AndroidCertVerifyResult::AndroidCertVerifyResult(
    /* [in] */ Int32 status)
    : mStatus(status)
    , mIsIssuedByKnownRoot(FALSE)
    , mCertificateChain(new List< AutoPtr<IX509Certificate> >())
{
}

/*
 * get mStatus previosly stored.
 */
Int32 AndroidCertVerifyResult::GetStatus()
{
    return mStatus;
}

/*
 * get mIsIssuedByKnownRoot previosly stored.
 */
Boolean AndroidCertVerifyResult::IsIssuedByKnownRoot()
{
    return mIsIssuedByKnownRoot;
}

AutoPtr< ArrayOf< AutoPtr< ArrayOf<Byte>  > > > AndroidCertVerifyResult::GetCertificateChainEncoded()
{
    AutoPtr< ArrayOf< AutoPtr< ArrayOf<Byte> > > > verifiedChainArray = AutoPtr< ArrayOf< AutoPtr< ArrayOf<Byte> > > >::Alloc(mCertificateChain->GetLength());
//    try {
        for (Int32 i = 0; i < mCertificateChain->GetLength(); ++i) {
            //alloc for AutoPtr and assignment
            //(*verifiedChainArray)[i] = AutoPtr< ArrayOf<Byte> >::Alloc(mCertificateChain.get(i).getEncoded().getLenght);
            //mCertificateChain.get(i).getEncoded();
        }
//    } catch (CertificateEncodingException e) {
//        return new byte[0][];
//    }
    return verifiedChainArray;
}

} // namespace  Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
