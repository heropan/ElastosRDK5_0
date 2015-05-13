
#include "MessageDigest.h"
#include <cutils/log.h>
#include <cmdef.h>
#include "CSecurity.h"

using Elastos::Core::EIID_ICloneable;

namespace Elastos {
namespace Security {

ECode MessageDigest::GetInstance(
    /* [in] */ const String& algorithm,
    /* [out] */ IMessageDigest** instance)
{
    VALIDATE_NOT_NULL(instance)
    if (algorithm.IsNull()) {
        ALOGE("algorithm == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    //Todo later after apache...
    /*
    Engine.SpiAndProvider sap = ENGINE.getInstance(algorithm, null);
    Object spi = sap.spi;
    Provider provider = sap.provider;
    if (spi instanceof MessageDigest) {
        MessageDigest result = (MessageDigest) spi;
        result.algorithm = algorithm;
        result.provider = provider;
        return result;
    }
    return new MessageDigestImpl((MessageDigestSpi) sap.spi, sap.provider, algorithm);
    */
    return E_NOT_IMPLEMENTED;
}

ECode MessageDigest::GetInstanceEx(
    /* [in] */ const String& algorithm,
    /* [in] */ const String& provider,
    /* [out] */ IMessageDigest** instance)
{
    if (provider.IsNullOrEmpty()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IProvider> p;
    AutoPtr<ISecurity> sec;
    CSecurity::AcquireSingleton((ISecurity**)&sec);
    sec->GetProvider(provider, (IProvider**)&p);
    if (p == NULL) {
        return E_NO_SUCH_PROVIDER_EXCEPTION;
    }
    return GetInstanceEx2(algorithm, p, instance);
}

ECode MessageDigest::GetInstanceEx2(
    /* [in] */ const String& algorithm,
    /* [in] */ IProvider* provider,
    /* [out] */ IMessageDigest** instance)
{
    if (provider == NULL) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (algorithm.IsNull()) {
        ALOGE("algorithm == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    //Todo later after apache...
    /*
        Object spi = ENGINE.getInstance(algorithm, provider, null);
        if (spi instanceof MessageDigest) {
            MessageDigest result = (MessageDigest) spi;
            result.algorithm = algorithm;
            result.provider = provider;
            return result;
        }
        return new MessageDigestImpl((MessageDigestSpi) spi, provider, algorithm);
    */
    return E_NOT_IMPLEMENTED;
}

ECode MessageDigest::IsEqual(
    /* [in] */ ArrayOf<Byte>* digesta,
    /* [in] */ ArrayOf<Byte>* digestb,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (digesta->GetLength() != digestb->GetLength()) {
        *result = FALSE;
        return NOERROR;
    }
    if (digesta->GetLength() != digestb->GetLength()) {
        *result = FALSE;
        return NOERROR;
    }
    for (Int32 i = 0; i < digesta->GetLength(); i++) {
        if ((*digesta)[i] != (*digestb)[i]) {
            *result = FALSE;
            return NOERROR;
        }
    }
    *result = TRUE;
    return NOERROR;
}

ECode MessageDigest::Reset()
{
    return EngineReset();
}

ECode MessageDigest::Update(
    /* [in] */ Byte arg0)
{
    return EngineUpdate(arg0);
}

ECode MessageDigest::UpdateEx(
    /* [in] */ ArrayOf<Byte>* input,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 len)
{
    if (input == NULL ||
        // offset < 0 || len < 0 ||
        // checks for negative values are commented out intentionally
        // see HARMONY-1120 for details
        offset + len > input->GetLength()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return EngineUpdateEx(input, offset, len);
}

ECode MessageDigest::UpdateEx2(
    /* [in] */ ArrayOf<Byte>* input)
{
    if (input == NULL) {
        ALOGE("input == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    return EngineUpdateEx(input, 0, input->GetLength());
}

ECode MessageDigest::Digest(
    /* [out, callee] */ ArrayOf<Byte>** hashValue)
{
    return EngineDigest(hashValue);
}

ECode MessageDigest::DigestEx(
    /* [in] */ ArrayOf<Byte>* buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 len,
    /* [out] */ Int32* number)
{
    if (buf == NULL ||
        // offset < 0 || len < 0 ||
        // checks for negative values are commented out intentionally
        // see HARMONY-1148 for details
        offset + len > buf->GetLength()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return EngineDigestEx(buf, offset, len, number);
}

ECode MessageDigest::DigestEx2(
    /* [in] */ ArrayOf<Byte>* input,
    /* [out, callee] */ ArrayOf<Byte>** hashValue)
{
    UpdateEx2(input);
    return Digest(hashValue);
}

ECode MessageDigest::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    *str = "MESSAGE DIGEST ";
    *str += mAlgorithm;
    return NOERROR;
}

ECode MessageDigest::GetAlgorithm(
    /* [out] */ String* algorithm) const
{
    VALIDATE_NOT_NULL(algorithm)
    *algorithm = mAlgorithm;
    return NOERROR;
}

ECode MessageDigest::GetProvider(
    /* [out, callee] */ IProvider** provider) const
{
    VALIDATE_NOT_NULL(provider)
    *provider = mProvider;
    INTERFACE_ADDREF(*provider)
    return NOERROR;
}

ECode MessageDigest::GetDigestLength(
    /* [out] */ Int32* length) const
{
    VALIDATE_NOT_NULL(length)
    *length = 0;
    Int32 l;
    EngineGetDigestLength(&l);
    if (l != 0) {
        *length = l;
        return NOERROR;
    }
    if (THIS_PROBE(ICloneable) == NULL) {
        *length = 0;
        return NOERROR;
    }
    AutoPtr<IInterface> cloobj;
    FAIL_RETURN(THIS_PROBE(ICloneable)->Clone((PInterface*)&cloobj))
    AutoPtr<ArrayOf<Byte> > hashValue;
    IMessageDigest::Probe(cloobj)->Digest((ArrayOf<Byte>**)&hashValue);
    *length = hashValue->GetLength();
    return NOERROR;
}

ECode MessageDigest::UpdateEx3(
    /* [in] */ IByteBuffer* input)
{
    return EngineUpdate(input);
}

PInterface MessageDigest::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

MessageDigest::MessageDigest(
    /* [in] */ const String& algorithm)
    : mAlgorithm(algorithm)
{}

class MessageDigest::MessageDigestImpl
        : public MessageDigest
        , public IMessageDigest {
    public:
        CAR_INTERFACE_DECL()

        IMESSAGEDIGEST_METHODS_DECL()


        CARAPI Clone(
            /* [out] */ IInterface** object);
    protected:
        // engineReset() implementation
        //@Override
        CARAPI EngineReset();

        // engineDigest() implementation
        //@Override
        CARAPI EngineDigest(
            /* [out, callee] */ ArrayOf<Byte>** ed);

        // engineGetDigestLength() implementation
        //@Override
        CARAPI EngineGetDigestLength(
            /* [out] */ Int32* len);

        // engineUpdate() implementation
        //@Override
        CARAPI EngineUpdate(
            /* [in] */ Byte arg0);

        // engineUpdate() implementation
        //@Override
        CARAPI EngineUpdateEx(
            /* [in] */ ArrayOf<Byte>* arg0,
            /* [in] */ Int32 arg1,
            /* [in] */ Int32 arg2);
    private:
        // MessageDigestImpl ctor
        MessageDigestImpl(
            /* [in] */ IMessageDigestSpi* messageDigestSpi,
            /* [in] */ IProvider* provider,
            /* [in] */ const String& algorithm);
    private:
        // MessageDigestSpi implementation
        AutoPtr<IMessageDigestSpi> mSpiImpl;
};

PInterface MessageDigest::MessageDigestImpl::Probe(
    /* [in] */ REIID riid)
{
    if ( riid == EIID_IInterface) {
        return (IInterface*)(IMessageDigest *)this;
    }
    else if ( riid == EIID_IMessageDigest ) {
        return (IMessageDigest *)this;
    }
    else if ( riid == EIID_ICloneable ) {
        return (ICloneable *)this;
    }
    else if ( riid == EIID_MessageDigestSpi) {
        return reinterpret_cast<PInterface>((MessageDigestSpi*)this);
    }
    return NULL;
}

UInt32 MessageDigest::MessageDigestImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MessageDigest::MessageDigestImpl::Release()
{
    return ElRefBase::Release();
}

ECode MessageDigest::MessageDigestImpl::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (object == (IInterface*)(IMessageDigest *)this) {
        *iid = EIID_IMessageDigest;
    }
    else if (object == (IInterface*)(ICloneable *)this) {
        *iid = EIID_ICloneable;
    }
    else if (object == reinterpret_cast<PInterface>((MessageDigestSpi*)this)) {
        *iid = EIID_MessageDigestSpi;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

IMESSAGEDIGEST_METHODS_IMPL(MessageDigest::MessageDigestImpl, MessageDigest)

ECode MessageDigest::MessageDigestImpl::EngineReset()
{
    return reinterpret_cast<MessageDigestSpi*>(mSpiImpl->Probe(EIID_MessageDigestSpi))->EngineReset();
}

ECode MessageDigest::MessageDigestImpl::EngineDigest(
    /* [out, callee] */ ArrayOf<Byte>** ed)
{
    return reinterpret_cast<MessageDigestSpi*>(mSpiImpl->Probe(EIID_MessageDigestSpi))->EngineDigest(ed);
}

ECode MessageDigest::MessageDigestImpl::EngineGetDigestLength(
    /* [out] */ Int32* len)
{
    return reinterpret_cast<MessageDigestSpi*>(mSpiImpl->Probe(EIID_MessageDigestSpi))->EngineGetDigestLength(len);
}

ECode MessageDigest::MessageDigestImpl::EngineUpdate(
    /* [in] */ Byte arg0)
{
    return reinterpret_cast<MessageDigestSpi*>(mSpiImpl->Probe(EIID_MessageDigestSpi))->EngineUpdate(arg0);
}

ECode MessageDigest::MessageDigestImpl::EngineUpdateEx(
    /* [in] */ ArrayOf<Byte>* arg0,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2)
{
    return reinterpret_cast<MessageDigestSpi*>(mSpiImpl->Probe(EIID_MessageDigestSpi))->EngineUpdateEx(arg0, arg1, arg2);
}

MessageDigest::MessageDigestImpl::MessageDigestImpl(
    /* [in] */ IMessageDigestSpi* messageDigestSpi,
    /* [in] */ IProvider* provider,
    /* [in] */ const String& algorithm)
    : MessageDigest(algorithm)
{
    mProvider = provider;
    mSpiImpl = messageDigestSpi;
}

ECode MessageDigest::MessageDigestImpl::Clone(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    if (ICloneable::Probe(mSpiImpl.Get())) {
        AutoPtr<IInterface> cloobj;
        ICloneable::Probe(mSpiImpl.Get())->Clone((IInterface**)&cloobj);
        AutoPtr<IMessageDigestSpi> spi = IMessageDigestSpi::Probe(cloobj);
        AutoPtr<IProvider> pro;
        GetProvider((IProvider**)&pro);
        String algo;
        GetAlgorithm(&algo);
        *object = (IMessageDigestSpi*)new MessageDigestImpl(spi, pro, algo);
        INTERFACE_ADDREF(*object)
        return NOERROR;
    }
    return E_CLONE_NOT_SUPPORTED_EXCEPTION;
}

} // namespace Security
} // namespace Elastos