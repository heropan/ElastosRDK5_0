#include "app/CSearchableInfoHelper.h"
#include "app/CSearchableInfo.h"

namespace Elastos {
namespace Droid {
namespace App {


ECode CSearchableInfoHelper::constructor()
{
    return NOERROR;
}

ECode CSearchableInfoHelper::GetActivityMetaData(
    /* [in] */ IContext* ctx,
    /* [in] */ IActivityInfo* activityInfo,
    /* [in] */ Int32 userId,
    /* [out] */ ISearchableInfo** info)
{
    return CSearchableInfo::GetActivityMetaData(ctx, activityInfo, userId, info);
}

}// namespace App
}// namespace Droid
}// namespace Elastos
