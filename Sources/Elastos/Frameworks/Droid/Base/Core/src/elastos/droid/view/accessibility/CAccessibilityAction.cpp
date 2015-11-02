
#include "elastos/droid/view/accessibility/CAccessibilityAction.h"
#include "elastos/droid/view/accessibility/CAccessibilityNodeInfo.h"
#include "elastos/droid/utility/CArraySet.h"
#include <elastos/core/Math.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Core::Math;
using Elastos::Droid::Utility::CArraySet;
using Elastos::Droid::Utility::IArraySet;
using Elastos::Utility::ICollection;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

static CARAPI_(AutoPtr<IAccessibilityAction>) InitACTION(
    /*[in] */ Int32 value)
{
    AutoPtr<CAccessibilityAction> action;
    CAccessibilityAction::NewByFriend(value, NULL, (CAccessibilityAction**)&action);
    return (IAccessibilityAction*)action.Get();
}

const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_FOCUS = InitACTION(IAccessibilityNodeInfo::ACTION_FOCUS);
const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_CLEAR_FOCUS = InitACTION(IAccessibilityNodeInfo::ACTION_CLEAR_FOCUS);
const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_SELECT = InitACTION(IAccessibilityNodeInfo::ACTION_SELECT);
const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_CLEAR_SELECTION = InitACTION(IAccessibilityNodeInfo::ACTION_CLEAR_SELECTION);
const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_CLICK = InitACTION(IAccessibilityNodeInfo::ACTION_CLICK);
const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_LONG_CLICK = InitACTION(IAccessibilityNodeInfo::ACTION_LONG_CLICK);
const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_ACCESSIBILITY_FOCUS = InitACTION(IAccessibilityNodeInfo::ACTION_ACCESSIBILITY_FOCUS);
const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_CLEAR_ACCESSIBILITY_FOCUS = InitACTION(IAccessibilityNodeInfo::ACTION_CLEAR_ACCESSIBILITY_FOCUS);
const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_NEXT_AT_MOVEMENT_GRANULARITY = InitACTION(IAccessibilityNodeInfo::ACTION_NEXT_AT_MOVEMENT_GRANULARITY);
const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_PREVIOUS_AT_MOVEMENT_GRANULARITY = InitACTION(IAccessibilityNodeInfo::ACTION_PREVIOUS_AT_MOVEMENT_GRANULARITY);
const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_NEXT_HTML_ELEMENT = InitACTION(IAccessibilityNodeInfo::ACTION_NEXT_HTML_ELEMENT);
const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_PREVIOUS_HTML_ELEMENT = InitACTION(IAccessibilityNodeInfo::ACTION_PREVIOUS_HTML_ELEMENT);
const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_SCROLL_FORWARD = InitACTION(IAccessibilityNodeInfo::ACTION_SCROLL_FORWARD);
const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_SCROLL_BACKWARD = InitACTION(IAccessibilityNodeInfo::ACTION_SCROLL_BACKWARD);
const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_COPY = InitACTION(IAccessibilityNodeInfo::ACTION_COPY);
const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_PASTE = InitACTION(IAccessibilityNodeInfo::ACTION_PASTE);
const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_CUT = InitACTION(IAccessibilityNodeInfo::ACTION_CUT);
const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_SET_SELECTION = InitACTION(IAccessibilityNodeInfo::ACTION_SET_SELECTION);
const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_EXPAND = InitACTION(IAccessibilityNodeInfo::ACTION_EXPAND);
const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_COLLAPSE = InitACTION(IAccessibilityNodeInfo::ACTION_COLLAPSE);
const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_DISMISS = InitACTION(IAccessibilityNodeInfo::ACTION_DISMISS);
const AutoPtr<IAccessibilityAction> CAccessibilityAction::ACTION_SET_TEXT = InitACTION(IAccessibilityNodeInfo::ACTION_SET_TEXT);

static AutoPtr<IArraySet> InitsStandardActions()
{
    AutoPtr<IArraySet> set;
    CArraySet::New((IArraySet**)&set);
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_FOCUS));
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_CLEAR_FOCUS));
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_SELECT));
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_CLEAR_SELECTION));
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_CLICK));
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_LONG_CLICK));
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_ACCESSIBILITY_FOCUS));
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_CLEAR_ACCESSIBILITY_FOCUS));
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_NEXT_AT_MOVEMENT_GRANULARITY));
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_PREVIOUS_AT_MOVEMENT_GRANULARITY));
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_NEXT_HTML_ELEMENT));
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_PREVIOUS_HTML_ELEMENT));
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_SCROLL_FORWARD));
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_SCROLL_BACKWARD));
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_COPY));
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_PASTE));
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_CUT));
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_SET_SELECTION));
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_EXPAND));
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_COLLAPSE));
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_DISMISS));
    ICollection::Probe(set)->Add(TO_IINTERFACE(CAccessibilityAction::ACTION_SET_TEXT));

    return set;
}

const AutoPtr<IArraySet> CAccessibilityAction::sStandardActions = InitsStandardActions();

CAR_INTERFACE_IMPL(CAccessibilityAction, Object, IAccessibilityAction)

CAR_OBJECT_IMPL(CAccessibilityAction)

CAccessibilityAction::CAccessibilityAction()
    : mActionId(0)
{
}

CAccessibilityAction::~CAccessibilityAction()
{
}

ECode CAccessibilityAction::constructor()
{
    return NOERROR;
}

ECode CAccessibilityAction::constructor(
    /* [in] */ Int32 actionId,
    /* [in] */ ICharSequence* label)
{
    if ((actionId & CAccessibilityNodeInfo::ACTION_TYPE_MASK) == 0
            && Elastos::Core::Math::BitCount(actionId) != 1) {
        // throw new IllegalArgumentException("Invalid standard action id");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mActionId = actionId;
    mLabel = label;

    return NOERROR;
}

ECode CAccessibilityAction::GetId(
    /* [out] */ Int32* actionId)
{
    VALIDATE_NOT_NULL(actionId);
    *actionId = mActionId;
    return NOERROR;
}

ECode CAccessibilityAction::GetLabel(
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    *label = mLabel;
    REFCOUNT_ADD(*label);
    return NOERROR;
}

ECode CAccessibilityAction::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    *hashCode = mActionId;
    return NOERROR;
}

ECode CAccessibilityAction::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (other == NULL) {
        *result = FALSE;
        return NOERROR;
    }

    if (IObject::Probe(other) == THIS_PROBE(IObject)) {
        *result = TRUE;
        return NOERROR;
    }

    AutoPtr<CAccessibilityAction> obj = (CAccessibilityAction*)IAccessibilityAction::Probe(other);
    *result = mActionId == obj->mActionId;
    return NOERROR;
}

ECode CAccessibilityAction::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    String str;
    mLabel->ToString(&str);
    StringBuilder builder;
    builder += "AccessibilityAction: ";
    builder += CAccessibilityNodeInfo::GetActionSymbolicName(mActionId);
    builder += " - ";
    builder += str;
    *info = builder.ToString();

    return NOERROR;
}

} // Accessibility
} // View
} // Droid
} // Elastos
