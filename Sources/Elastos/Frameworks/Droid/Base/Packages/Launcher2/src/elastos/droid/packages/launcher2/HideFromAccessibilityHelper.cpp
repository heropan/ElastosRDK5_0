
#include "elastos/droid/packages/launcher2/PackageChangedReceiver.h"
#include "Elastos.Droid.Service.h"
#include "R.h"

namespace Elastos {
namespace Droid {
namespace Packages {
namespace Launcher2 {

CAR_INTERFACE_IMPL(HideFromAccessibilityHelper, Object, IViewGroupOnHierarchyChangeListener);

HideFromAccessibilityHelper::HideFromAccessibilityHelper()
    : mHide(FALSE)
    , mOnlyAllApps(FALSE))
{
    CHashMap::New((IHashMap**)&mPreviousValues);
}

ECode HideFromAccessibilityHelper::SetImportantForAccessibilityToNo(
    /* [in] */ IView* v,
    /* [in] */ Boolean onlyAllApps)
{
    mOnlyAllApps = onlyAllApps;
    SetImportantForAccessibilityToNoHelper(v);
    mHide = TRUE;
    return NOERROR;
}

void HideFromAccessibilityHelper::SetImportantForAccessibilityToNoHelper(
    /* [in] */ IView* v)
{
    Int32 mode;
    v->GetImportantForAccessibility(&mode);
    AutoPtr<IInteger32> obj = CoreUtils::Convert(mode);
    mPreviousValues->Put(TO_IINTERFACE(v), TO_IINTERFACE(obj));
    v->SetImportantForAccessibility(IView::IMPORTANT_FOR_ACCESSIBILITY_NO);

    // Call method on children recursively
    if (IViewGroup::Probe(v) != NULL) {
        AutoPtr<IViewGroup> vg = IViewGroup::Probe(v);
        vg->SetOnHierarchyChangeListener(this);
        Int32 count;
        vg->GetChildCount(&count);
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IView> child;
            vg->GetChildAt(i, (IView**)&child);

            if (IncludeView(child)) {
                SetImportantForAccessibilityToNoHelper(child);
            }
        }
    }
}

ECode HideFromAccessibilityHelper::RestoreImportantForAccessibility(
    /* [in] */ IView* v)
{
    if (mHide) {
        RestoreImportantForAccessibilityHelper(v);
    }
    mHide = FALSE;
    return NOERROR;
}

void HideFromAccessibilityHelper::RestoreImportantForAccessibilityHelper(
    /* [in] */ IView* v)
{
    AutoPtr<IInterface> obj;
    mPreviousValues->Get(TO_IINTERFACE(v), (IInterface**)&obj);
    AutoPtr<IInteger32> intObj = IInteger32::Probe(obj);
    Int32 value;
    intObj->GetValue(&value);
    v->SetImportantForAccessibility(value);
    mPreviousValues->Remove(TO_IINTERFACE(v));

    // Call method on children recursively
    if (IViewGroup::Probe(v) != NULL) {
        AutoPtr<IViewGroup> vg = IViewGroup::Probe(v);

        // We assume if a class implements OnHierarchyChangeListener, it listens
        // to changes to any of its children (happens to be the case in Launcher)
        if (IOnHierarchyChangeListener::Probe(vg) != NULL) {
            vg->SetOnHierarchyChangeListener(IOnHierarchyChangeListener::Probe(vg));
        }
        else {
            vg->SetOnHierarchyChangeListener(NULL);
        }
        Int32 count;
        vg->GetChildCount(&count);
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IView> child;
            vg->GetChildAt(i, (IView**)&child);
            if (IncludeView(child)) {
                RestoreImportantForAccessibilityHelper(child);
            }
        }
    }
}

ECode HideFromAccessibilityHelper::OnChildViewAdded(
    /* [in] */ IView* parent,
    /* [in] */ IView* child)
{
    if (mHide && IncludeView(child)) {
        SetImportantForAccessibilityToNoHelper(child);
    }
    return NOERROR;
}

ECode HideFromAccessibilityHelper::OnChildViewRemoved(
    /* [in] */ IView* parent,
    /* [in] */ IView* child)
{
    if (mHide && IncludeView(child)) {
        RestoreImportantForAccessibilityHelper(child);
    }
}

Boolean HideFromAccessibilityHelper::IncludeView(
    /* [in] */ IView* v)
{
    return !HasAncestorOfType(v, ECLSID_CCling) &&
                (!mOnlyAllApps || HasAncestorOfType(v, ECLSID_CAppsCustomizeTabHost));
}

Boolean HideFromAccessibilityHelper::HasAncestorOfType(
    /* [in] */ IView* v,
    /* [in] */ ClassID c)
{
    if (v == NULL) {
        return FALSE;
    }
    else {
        ClassID clsId;
        IObject::Probe(v)->GetClassID(&clsId);
        if (c == clsId) {
            return TRUE;
        }

        AutoPtr<IViewParent> parentParent;
        v->GetParent((IViewParent**)&parentParent);
        if (IViewGroup::Probe(parentParent) == NULL) {
            return FALSE;
        }

        AutoPtr<IViewGroup> vg = IViewGroup::Probe(parentParent);
        return HasAncestorOfType(IView::Probe(vg), c);
    }
}

} // namespace Launcher2
} // namespace Packages
} // namespace Droid
} // namespace Elastos