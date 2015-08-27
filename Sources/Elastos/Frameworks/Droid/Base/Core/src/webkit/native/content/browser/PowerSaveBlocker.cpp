// wuweizuo automatic build .cpp file from .java file.

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                           PowerSaveBlocker
//=====================================================================
ECode PowerSaveBlocker::ApplyBlock(
    /* [in] */ ViewAndroid* view)
{
    VALIDATE_NOT_NULL(view);
    /*
    view.incrementKeepScreenOnCount();
    */
    return NOERROR;
}

ECode PowerSaveBlocker::RemoveBlock(
    /* [in] */ ViewAndroid* view)
{
    VALIDATE_NOT_NULL(view);
    /*
    view.decrementKeepScreenOnCount();
    */
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


