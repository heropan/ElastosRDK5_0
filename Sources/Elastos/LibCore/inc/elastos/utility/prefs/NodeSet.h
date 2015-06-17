#ifndef __UTILITY_PREFS_NODESET_H__
#define __UTILITY_PREFS_NODESET_H__

#include "Object.h"

using Elastos::Core::Object;
using Org::W3c::Dom::INodeList;

namespace Elastos {
namespace Utility {
namespace Prefs {

class NodeSet
    : public Object
    , public INodeList
{
public:
    CAR_INTERFACE_DECL();

    NodeSet(
        /* [in] */ IIterator* nodes);

    CARAPI GetLength(
        /* [out] */ Int32* len);

    CARAPI Item(
        /* [in] */ Int32 index,
        /* [out] */ INode** node);

    CARAPI ToString(
        /* [out] */ String* info);

protected:
    AutoPtr<IArrayList> mList;
};

} // namespace Prefs
} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_PREFS_NODESET_H__
