
#include "elastos/droid/app/CAccessibilityServiceClientImpl.h"
#include "elastos/droid/app/CActivity.h"
#include "elastos/droid/app/CActivityGroup.h"
#include "elastos/droid/app/CActivityView.h"
#include "elastos/droid/app/CActivityViewActivityContainerCallback.h"
#include "elastos/droid/app/CApplication.h"
#include "elastos/droid/app/CAppImportanceMonitorProcessObserver.h"
#include "elastos/droid/app/CBackStackRecord.h"
#include "elastos/droid/app/CFastBitmapDrawable.h"
#include "elastos/droid/app/CFragmentBreadCrumbs.h"
#include "elastos/droid/app/CFragmentState.h"
#include "elastos/droid/app/CInstrumentation.h"
#include "elastos/droid/app/CJobSchedulerImpl.h"
#include "elastos/droid/app/CKeyguardManagerKeyguardLock.h"
#include "elastos/droid/app/CMediaRouteButton.h"
#include "elastos/droid/app/CNotificationManager.h"
#include "elastos/droid/app/CBuilderRemoteViews.h"
#include "elastos/droid/app/CRemoteInputBuilder.h"
#include "elastos/droid/app/CUiAutomation.h"
#include "elastos/droid/app/CUiAutomationConnection.h"
#include "elastos/droid/app/CVoiceInteractor.h"
#include "elastos/droid/app/CVoiceInteractorCallback.h"
#include "elastos/droid/app/CVoiceInteractorConfirmationRequest.h"
#include "elastos/droid/app/CVoiceInteractorCompleteVoiceRequest.h"
#include "elastos/droid/app/CVoiceInteractorAbortVoiceRequest.h"
#include "elastos/droid/app/CVoiceInteractorCommandRequest.h"

namespace Elastos {
namespace Droid {
namespace App {

CAR_OBJECT_IMPL(CAccessibilityServiceClientImpl)
CAR_OBJECT_IMPL(CActivity)
CAR_OBJECT_IMPL(CActivityGroup)
CAR_OBJECT_IMPL(CActivityView)
CAR_OBJECT_IMPL(CActivityViewActivityContainerCallback)
CAR_OBJECT_IMPL(CApplication)
CAR_OBJECT_IMPL(CAppImportanceMonitorProcessObserver)
CAR_OBJECT_IMPL(CBackStackRecord)
CAR_OBJECT_IMPL(CFastBitmapDrawable)
CAR_OBJECT_IMPL(CFragmentBreadCrumbs)
CAR_OBJECT_IMPL(CFragmentState)
CAR_OBJECT_IMPL(CInstrumentation)
CAR_OBJECT_IMPL(CJobSchedulerImpl)
CAR_OBJECT_IMPL(CKeyguardManagerKeyguardLock)
CAR_OBJECT_IMPL(CMediaRouteButton)
CAR_OBJECT_IMPL(CNotificationManager)
CAR_OBJECT_IMPL(CBuilderRemoteViews)
CAR_OBJECT_IMPL(CRemoteInputBuilder)
CAR_OBJECT_IMPL(CUiAutomation)
CAR_OBJECT_IMPL(CUiAutomationConnection)
CAR_OBJECT_IMPL(CVoiceInteractor)
CAR_OBJECT_IMPL(CVoiceInteractorCallback)
CAR_OBJECT_IMPL(CVoiceInteractorConfirmationRequest)
CAR_OBJECT_IMPL(CVoiceInteractorCompleteVoiceRequest)
CAR_OBJECT_IMPL(CVoiceInteractorAbortVoiceRequest)
CAR_OBJECT_IMPL(CVoiceInteractorCommandRequest)


} // namespace App
} // namespace Droid
} // namespace Elastos