#include "elastos/droid/text/style/TabStopSpan.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

/*****************************TabStopSpan::Standard*****************************/
PInterface TabStopSpan::Standard::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(ITabStopSpan*)this;
    }
    else if (riid == EIID_ITabStopSpan) {
        return (ITabStopSpan*)this;
    }
    return NULL;
}

UInt32 TabStopSpan::Standard::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TabStopSpan::Standard::Release()
{
    return ElRefBase::Release();
}

ECode TabStopSpan::Standard::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (object == (IInterface*)(ITabStopSpan*)this) {
        *iID = EIID_ITabStopSpan;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

TabStopSpan::Standard::Standard(
    /* [in] */ Int32 where)
{
    mTab = where;
}

ECode TabStopSpan::Standard::GetTabStop(
    /* [out] */ Int32* ret)
{
    *ret = mTab;
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos