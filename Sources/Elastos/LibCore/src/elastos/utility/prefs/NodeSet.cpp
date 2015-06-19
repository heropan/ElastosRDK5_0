
#include "NodeSet.h"
#include "CArrayList.h"

using Elastos::Utility::CArrayList;
using Org::W3c::Dom::EIID_INodeList;

namespace Elastos {
namespace Utility {
namespace Prefs {

extern "C" const InterfaceID EIID_NodeSet =
        { 0x0a518aeb, 0x50fc, 0x4479, { 0xbe, 0x53, 0x64, 0x6e, 0x35, 0x79, 0x81, 0x6c } };

CAR_INTERFACE_IMPL(NodeSet, Object, INodeList);

NodeSet::NodeSet(
    /* [in] */ IIterator* nodes)
{
    CArrayList::New((IArrayList**)&mList);
    Boolean hasNext = FALSE;
    while(nodes->HasNext(&hasNext), hasNext) {
        AutoPtr<IInterface> value;
        nodes->GetNext((IInterface**)&value);
        IList::Probe(mList)->Add(value);
    }
}

ECode NodeSet::GetLength(
    /* [out] */ Int32* len)
{
    VALIDATE_NOT_NULL(len);
    return ICollection::Probe(mList)->GetSize(len);
}

ECode NodeSet::Item(
    /* [in] */ Int32 index,
    /* [out] */ INode** node)
{
    VALIDATE_NOT_NULL(node);
    AutoPtr<IInterface> result;
    ECode ec = IList::Probe(mList)->Get(index, (IInterface**)&result);
    if (FAILED(ec)) {
        *node = NULL;
        return NOERROR;
    }

    *node = INode::Probe(result);
    REFCOUNT_ADD(*node);
    return ec;
}

ECode NodeSet::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = String("Elastos.Utility.Prefs.NodeSet");
    return NOERROR;
}

} // namespace Prefs
} // namespace Utility
} // namespace Elastos
