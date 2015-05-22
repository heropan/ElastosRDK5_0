
#ifndef __MESSAGEDIGEST_H__
#define __MESSAGEDIGEST_H__

#ifdef ELASTOS_CORELIBRARY
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif

#include <cmdef.h>
#include "MessageDigestMacro.h"
#include "MessageDigestSpi.h"

using Elastos::IO::IByteBuffer;
using Elastos::Core::ICloneable;

namespace Elastos {
namespace Security {

class MessageDigest
    : public MessageDigestSpi {
public:
    static CARAPI GetInstance(
        /* [in] */ const String& algorithm,
        /* [out] */ IMessageDigest** instance);

    static CARAPI GetInstanceEx(
        /* [in] */ const String& algorithm,
        /* [in] */ const String& provider,
        /* [out] */ IMessageDigest** instance);

    static CARAPI GetInstanceEx2(
        /* [in] */ const String& algorithm,
        /* [in] */ IProvider* provider,
        /* [out] */ IMessageDigest** instance);

    static CARAPI IsEqual(
        /* [in] */ ArrayOf<Byte>* digesta,
        /* [in] */ ArrayOf<Byte>* digestb,
        /* [out] */ Boolean* result);

    CARAPI Reset();

    CARAPI Update(
        /* [in] */ Byte arg0);

    CARAPI UpdateEx(
        /* [in] */ ArrayOf<Byte>* input,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 len);

    CARAPI UpdateEx2(
        /* [in] */ ArrayOf<Byte>* input);

    CARAPI Digest(
        /* [out, callee] */ ArrayOf<Byte>** hashValue);

    CARAPI DigestEx(
        /* [in] */ ArrayOf<Byte>* buf,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 len,
        /* [out] */ Int32* number);

    CARAPI DigestEx2(
        /* [in] */ ArrayOf<Byte>* input,
        /* [out, callee] */ ArrayOf<Byte>** hashValue);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI GetAlgorithm(
        /* [out] */ String* algorithm) const;

    CARAPI GetProvider(
        /* [out, callee] */ IProvider** provider) const;

    CARAPI GetDigestLength(
        /* [out] */ Int32* length) const;

    CARAPI UpdateEx3(
        /* [in] */ IByteBuffer* input);

protected:
    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
protected:
    /**
     * Constructs a new instance of {@code MessageDigest} with the name of
     * the algorithm to use.
     *
     * @param algorithm
     *            the name of algorithm to use
     */
    MessageDigest(
        /* [in] */ const String& algorithm);


protected:
    //Todo later, related to apache...
    // Used to access common engine functionality
    //static const Engine ENGINE = new Engine("MessageDigest");

    // The provider
    AutoPtr<IProvider> mProvider;

    // The algorithm.
    String mAlgorithm;

private:
    class MessageDigestImpl;
};

} // namespace Security
} // namespace Elastos

#endif // __MESSAGEDIGEST_H__
