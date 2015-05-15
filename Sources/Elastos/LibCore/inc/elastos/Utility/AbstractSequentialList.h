#ifndef __UTILITY_ABSTRACTSEQUENTIALLIST_H__
#define __UTILITY_ABSTRACTSEQUENTIALLIST_H__

#include "cmdef.h"
#include "AbstractList.h"

namespace Elastos {
namespace Utility {

class AbstractSequentialList : public AbstractList
{
public:
    CARAPI AddEx(
        /* [in] */ Int32 location,
        /* [in] */ IInterface* object);

    CARAPI AddAllEx(
        /* [in] */ Int32 location,
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* result);

    CARAPI Get(
        /* [in] */ Int32 location,
        /* [out] */ IInterface** object);

    CARAPI GetIterator(
        /* [out] */ IIterator** result);

    virtual CARAPI GetListIteratorEx(
        /* [in] */ Int32 location,
        /* [out] */ IListIterator** it) = 0;

    CARAPI RemoveEx(
        /* [in] */ Int32 location,
        /* [out] */ IInterface** object);

    CARAPI Set(
        /* [in] */ Int32 location,
        /* [in] */ IInterface* object,
        /* [out] */ IInterface** prevObject);

protected:
    /**
      * Constructs a new instance of this AbstractSequentialList.
    */
    AbstractSequentialList() {}
};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_ABSTRACTSEQUENTIALLIST_H__
