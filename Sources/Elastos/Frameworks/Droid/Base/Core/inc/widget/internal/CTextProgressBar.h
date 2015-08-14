#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_CTEXTPROGRESSBAR_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_CTEXTPROGRESSBAR_H__

#include "_CTextProgressBar.h"
#include "widget/internal/TextProgressBar.h"



namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CarClass(CTextProgressBar) , public TextProgressBar
{
public:
    IRELATIVELAYOUT_METHODS_DECL()
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CTextProgressBar();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI SetDurationBase(
        /* [in] */ Int64 durationBase);

    CARAPI OnChronometerTick(
        /* [in] */ IChronometer* chronometer);
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif
