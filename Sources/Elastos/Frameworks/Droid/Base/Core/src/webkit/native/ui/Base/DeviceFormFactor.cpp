
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Base {

const Int32 DeviceFormFactor::MINIMUM_TABLET_WIDTH_DP;
Boolean DeviceFormFactor::sIsTablet = FALSE;

Boolean DeviceFormFactor::IsTablet(
    /* in */ IContext* context)
{
    if (NULL == sIsTablet) {
        AutoPtr<IResources> resources;
        context->GetResources((IResources**)&resources);

        AutoPtr<IConfiguration> configuration;
        resources->GetConfiguration((IConfiguration**)&configuration);

        Int32 minimumScreenWidthDp = configuration->smallestScreenWidthDp;
        sIsTablet = minimumScreenWidthDp >= MINIMUM_TABLET_WIDTH_DP;
    }
    return sIsTablet;
}

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

