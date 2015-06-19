
#include "NodeChangeEvent.h"

using Elastos::IO::EIID_ISerializable;

namespace Elastos {
namespace Utility {
namespace Prefs {

extern "C" const InterfaceID EIID_NodeChangeEvent =
        { 0x3923b969, 0xe881, 0x407a, { 0xb2, 0xa8, 0x27, 0xb9, 0x47, 0x1d, 0xdd, 0x50 } };

CAR_INTERFACE_IMPL_2(NodeChangeEvent, EventObject, INodeChangeEvent, ISerializable);

NodeChangeEvent::NodeChangeEvent(
    /* [in] */ IPreferences* p,
    /* [in] */ IPreferences* c)
    : EventObject(IObject::Probe(p))
    , mParent(p)
    , mChild(c)
{
}

ECode NodeChangeEvent::GetParent(
    /* [out] */ IPreferences** parent)
{
    VALIDATE_NOT_NULL(parent);
    *parent = mParent;
    REFCOUNT_ADD(*parent);
    return NOERROR;
}

ECode NodeChangeEvent::GetChild(
    /* [out] */ IPreferences** child)
{
    VALIDATE_NOT_NULL(child);
    *child = mChild;
    REFCOUNT_ADD(*child);
    return NOERROR;
}

ECode NodeChangeEvent::WriteObject(
    /* [in] */ IObjectOutputStream* out)/* throws IOException */
{
    return E_NOT_SERIALIZABLE_EXCEPTION;
}

/**
 * This method always throws a <code>NotSerializableException</code>,
 * because this object cannot be serialized,
 */
ECode NodeChangeEvent::ReadObject(
    /* [in] */ IObjectInputStream* in) /*throws IOException, ClassNotFoundException*/
{
    return E_NOT_SERIALIZABLE_EXCEPTION;
}

ECode NodeChangeEvent::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = String("Elastos.Utility.Prefs.NodeChangeEvent");
    return NOERROR;
}

} // namespace Prefs
} // namespace Utility
} // namespace Elastos
