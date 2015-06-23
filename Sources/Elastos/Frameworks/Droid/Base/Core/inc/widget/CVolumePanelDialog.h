
#ifndef __CVOLUMEPANELDIALOG_H__
#define __CVOLUMEPANELDIALOG_H__

#include "_CVolumePanelDialog.h"
#include "widget/VolumePanelDialog.h"




using Elastos::Droid::View::IMotionEvent;

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CVolumePanelDialog), public VolumePanelDialog
{
public:
    IALERTDIALOG_METHODS_DECL();
    IDIALOG_METHODS_DECL();
    IWINDOWCALLBACK_METHODS_DECL();
    IKEYEVENTCALLBACK_METHODS_DECL();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme,
        /* [in] */ IVolumePanel* panel);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI OnCreateContextMenu(
        /* [in] */ IContextMenu* menu,
        /* [in] */ IView* v,
        /* [in] */ IContextMenuInfo* menuInfo);
};

}
}
}

#endif // __CVOLUMEPANELDIALOG_H__
