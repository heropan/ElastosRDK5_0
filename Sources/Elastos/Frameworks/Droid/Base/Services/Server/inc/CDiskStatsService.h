#ifndef __CDISKSTATSSERVICE_H__
#define __CDISKSTATSSERVICE_H__

#include "_CDiskStatsService.h"

using Elastos::IO::IFile;
using Elastos::IO::IFileDescriptor;
using Elastos::IO::IPrintWriter;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Server {

CarClass(CDiskStatsService)
{
public:
    CARAPI constructor(
        /* [in] */ IContext* context);

    //@Override
    CARAPI Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    CARAPI_(void) ReportFreeSpace(
        /* [in] */ IFile* path,
        /* [in] */ const String& name,
        /* [in] */ IPrintWriter* pw);

private:
    const static String TAG;
    AutoPtr<IContext> mContext;

};

}//Server
}//Droid
}//Elastos

#endif //__CDISKSTATSSERVICE_H__