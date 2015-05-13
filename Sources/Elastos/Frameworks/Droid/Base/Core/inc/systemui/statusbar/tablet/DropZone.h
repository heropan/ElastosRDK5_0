#ifndef __DropZone_H_
#define __DropZone_H_

#include "view/View.h"
#include "animation/AnimatorListenerAdapter.h"

using Elastos::Droid::Animation::AnimatorListenerAdapter;
using Elastos::Droid::View::IDragEvent;
using Elastos::Droid::Animation::IAnimator;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Tablet {

class DropZone : public Elastos::Droid::View::View
{
public:
    class MyListener
        : public AnimatorListenerAdapter
    {
    public:
        MyListener(
            /* [in] */ DropZone* host);

        virtual CARAPI OnAnimationEnd(
            /* [in] */ IAnimator* animation);
    private:
        DropZone* mHost;
    };

    DropZone();

    DropZone(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    ECode SetPocket(
        /* [in] */ IShirtPocket* p);

    virtual CARAPI OnAttachedToWindow();

    virtual CARAPI_(Boolean) OnDragEvent(
        /* [in] */ IDragEvent* event);
private:
    // Drag API notes: we must be visible to receive drag events
    void Show(
        /* [in] */ Boolean animate);

    void Hide(
        /* [in] */ Boolean animate);

protected:
    AutoPtr<IShirtPocket> mPocket;
};

}// namespace Tablet
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__DropZone_H_
