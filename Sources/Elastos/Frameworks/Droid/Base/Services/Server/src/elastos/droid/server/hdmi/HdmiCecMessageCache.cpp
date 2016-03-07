
#include "elastos/droid/server/hdmi/HdmiCecMessageCache.h"
#include <Elastos.Droid.Utility.h>

namespace Elastos {
namespace Droid {
namespace Server {
namespace Hdmi {

HdmiCecMessageCache::HdmiCecMessageCache()
{
#if 0 // TODO: Translate codes below
    AutoPtr<ISparseArray> mCache = new SparseArray<>();
#endif
}

ECode HdmiCecMessageCache::constructor()
{
    return NOERROR;
}

ECode HdmiCecMessageCache::GetMessage(
    /* [in] */ Int32 address,
    /* [in] */ Int32 opcode,
    /* [out] */ IHdmiCecMessage** result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AutoPtr<IInterface> obj;
        mCache->Get(address, (IInterface**)&obj);
        SparseArray<HdmiCecMessage> messages = I::Probe(obj);
        if (messages == NULL) {
            *result = NULL;
            return NOERROR;
        }
        obj = NULL;
        messages->Get(opcode, (IInterface**)&obj);
        return I::Probe(obj);

#endif
}

ECode HdmiCecMessageCache::FlushMessagesFrom(
    /* [in] */ Int32 address)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mCache->Remove(address);

#endif
}

ECode HdmiCecMessageCache::FlushAll()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mCache->Clear();

#endif
}

ECode HdmiCecMessageCache::CacheMessage(
    /* [in] */ IHdmiCecMessage* message)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Int32 opcode;
        message->GetOpcode(&opcode);
        if (!IsCacheable(opcode)) {
            return NOERROR;
        }
        Int32 source;
        message->GetSource(&source);
        AutoPtr<IInterface> obj;
        mCache->Get(source, (IInterface**)&obj);
        SparseArray<HdmiCecMessage> messages = I::Probe(obj);
        if (messages == NULL) {
            messages = new SparseArray<>();
            mCache->Put(source, messages);
        }
        messages->Put(opcode, message);

#endif
}

ECode HdmiCecMessageCache::IsCacheable(
    /* [in] */ Int32 opcode,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return CACHEABLE_OPCODES.Contains(opcode);

#endif
}

AutoPtr<IFastImmutableArraySet> HdmiCecMessageCache::InitCACHEABLE_OPCODES()
{
    AutoPtr<IFastImmutableArraySet> rev;
#if 0 // TODO: Translate codes below
     = new FastImmutableArraySet<>(new Integer[] {
                        Constants::MESSAGE_SET_OSD_NAME,
                        Constants::MESSAGE_REPORT_PHYSICAL_ADDRESS,
                        Constants::MESSAGE_DEVICE_VENDOR_ID,
                        Constants::MESSAGE_CEC_VERSION,
                }
#endif
    return rev;
}

} // namespace Hdmi
} // namespace Server
} // namespace Droid
} // namespace Elastos
