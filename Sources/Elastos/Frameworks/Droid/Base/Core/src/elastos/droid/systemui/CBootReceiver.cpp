
#include "elastos/droid/systemui/CBootReceiver.h"

using Elastos::Droid::Content::IContentResolver;

namespace Elastos {
namespace Droid {
namespace SystemUI {

//===============================================================
// CBootReceiver::
//===============================================================
String CBootReceiver::TAG = String("SystemUIBootReceiver");

ECode CBootReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // try {
        // Start the load average overlay, if activated
        AutoPtr<IContentResolver> res;
        context->GetContentResolver((IContentResolver**)&res);
        assert(0 && "TODO");
        // if (Settings.Global.GetInt(res, Settings.Global.SHOW_PROCESSES, 0) != 0) {
        //     AutoPtr<IIntent> loadavg;
        //     CIntent::New(context, com.android.systemui.LoadAverageService.class, (IIntent**)&loadavg);
        //     context->StartService(loadavg);
        // }
    // } catch (Exception e) {
    //     Log.e(TAG, "Can't start load average service", e);
    // }
    return NOERROR;
}

} // namespace SystemUI
} // namepsace Droid
} // namespace Elastos