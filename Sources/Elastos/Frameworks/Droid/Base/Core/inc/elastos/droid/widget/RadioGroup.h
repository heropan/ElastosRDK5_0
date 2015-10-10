#ifndef __ELASTOS_DROID_WIDGET_RADIOGROUP_H__
#define __ELASTOS_DROID_WIDGET_RADIOGROUP_H__

#include "elastos/droid/ext/frameworkext.h"
#include "widget/LinearLayout.h"

using Elastos::Droid::View::IViewGroupOnHierarchyChangeListener;

namespace Elastos {
namespace Droid {
namespace Widget {


class RadioGroup : public LinearLayout
{
private:
    class CheckedStateTracker
        : public ElRefBase
        , public ICompoundButtonOnCheckedChangeListener
    {
    public:
        CAR_INTERFACE_DECL();

        CheckedStateTracker(
            /* [in] */ RadioGroup* host);

        CARAPI OnCheckedChanged(
            /* [in] */ ICompoundButton* buttonView,
            /* [in] */ Boolean isChecked);

    private:
        RadioGroup* mHost;
    };

    /**
     * <p>A pass-through listener acts upon the events and dispatches them
     * to another listener. This allows the table layout to set its own internal
     * hierarchy change listener without preventing the user to setup his.</p>
     */
    class PassThroughHierarchyChangeListener
        : public ElRefBase
        , public IViewGroupOnHierarchyChangeListener
    {
        friend class RadioGroup;
    public:
        CAR_INTERFACE_DECL();

        PassThroughHierarchyChangeListener(
            /* [in] */ RadioGroup* host);

        CARAPI OnChildViewAdded(
            /* [in] */ IView* parent,
            /* [in] */ IView* child);

        CARAPI OnChildViewRemoved(
            /* [in] */ IView* parent,
            /* [in] */ IView* child);

    private:
        AutoPtr<IViewGroupOnHierarchyChangeListener> mOnHierarchyChangeListener;
        RadioGroup* mHost;
    };

public:
    RadioGroup();

    RadioGroup(
        /* [in] */ IContext* context);

    RadioGroup(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SetOnHierarchyChangeListener(
        /* [in] */ IViewGroupOnHierarchyChangeListener* listener);

    using LinearLayout::AddView;

    CARAPI AddView(
        /* [in] */ IView* child,
        /* [in] */ Int32 index,
        /* [in] */ IViewGroupLayoutParams* params);

    virtual CARAPI Check(
        /* [in] */ Int32 id);

    virtual CARAPI_(Int32) GetCheckedRadioButtonId();

    virtual CARAPI ClearCheck();

    virtual CARAPI SetOnCheckedChangeListener(
        /* [in] */ IRadioGroupOnCheckedChangeListener* listener);

    CARAPI GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IViewGroupLayoutParams** params);

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

protected:
    CARAPI OnFinishInflate();

    CARAPI_(Boolean) CheckLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

    CARAPI GenerateDefaultLayoutParams(
        /* [out] */ ILinearLayoutLayoutParams** params);

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

private:
    CARAPI_(void) InitInternal();

    CARAPI_(void) SetCheckedId(
        /* [in] */ Int32 id);

    CARAPI_(void) SetCheckedStateForView(
        /* [in] */ Int32 viewId,
        /* [in] */ Boolean checked);

private:
    // holds the checked id; the selection is empty by default
    Int32 mCheckedId;
    // tracks children radio buttons checked state
    AutoPtr<ICompoundButtonOnCheckedChangeListener> mChildOnCheckedChangeListener;
    // when TRUE, mOnCheckedChangeListener discards events
    Boolean mProtectFromCheckedChange;
    AutoPtr<IRadioGroupOnCheckedChangeListener> mOnCheckedChangeListener;
    AutoPtr<PassThroughHierarchyChangeListener> mPassThroughListener;
};


} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_RADIOGROUP_H__
