// wuweizuo automatic build .cpp file from .java file.

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

TouchDevice::TouchDevice()
{
}

Int32 TouchDevice::MaxTouchPoints(
    /* in */ IContext* context)
{
	AutoPtr<IPackageManager> packageManager;
	context->GetPackageManager((IPackageManager**)&packageManager);

	Boolean hasSystemFeature;
	packageManager->HasSystemFeature(IPackageManager::FEATURE_TOUCHSCREEN_MULTITOUCH_JAZZHAND, &hasSystemFeature);

    Boolean hasSystemFeature1;
	packageManager->HasSystemFeature(IPackageManager::FEATURE_TOUCHSCREEN_MULTITOUCH_DISTINCT, &hasSystemFeature1);

	Boolean hasSystemFeature2;
	packageManager->HasSystemFeature(IPackageManager::FEATURE_TOUCHSCREEN_MULTITOUCH, &hasSystemFeature2);

	Boolean hasSystemFeature3;
	packageManager->HasSystemFeature(IPackageManager::FEATURE_TOUCHSCREEN, &hasSystemFeature3);

	if (hasSystemFeature) {
        return 5;
    }
 	else if (hasSystemFeature1) {
        return 2;
    }
 	else if (hasSystemFeature2) {
        return 2;
    }
 	else if (hasSystemFeature3) {
        return 1;
    }
    else {
        return 0;
    }
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
