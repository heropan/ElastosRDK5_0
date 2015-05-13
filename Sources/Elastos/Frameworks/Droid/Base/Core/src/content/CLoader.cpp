
#include "content/CLoader.h"

namespace Elastos {
namespace Droid {
namespace Content {

ECode CLoader::DeliverResult(
    /* [in] */ IInterface* data)
{
    return Loader::DeliverResult(data);
}

ECode CLoader::DeliverCancellation()
{
    return Loader::DeliverCancellation();
}

ECode CLoader::GetContext(
    /* [out] */ IContext** context)
{
    return Loader::GetContext(context);
}

ECode CLoader::GetId(
    /* [out] */ Int32* id)
{
    return Loader::GetId(id);
}

ECode CLoader::RegisterListener(
    /* [in] */ Int32 id,
    /* [in] */ ILoaderOnLoadCompleteListener* listener)
{
    return Loader::RegisterListener(id, listener);
}

ECode CLoader::UnregisterListener(
    /* [in] */ ILoaderOnLoadCompleteListener* listener)
{
    return Loader::UnregisterListener(listener);
}

ECode CLoader::RegisterOnLoadCanceledListener(
    /* [in] */ ILoaderOnLoadCanceledListener* listener)
{
    return Loader::RegisterOnLoadCanceledListener(listener);
}

ECode CLoader::UnregisterOnLoadCanceledListener(
    /* [in] */ ILoaderOnLoadCanceledListener* listener)
{
    return Loader::UnregisterOnLoadCanceledListener(listener);
}

ECode CLoader::IsStarted(
    /* [out] */ Boolean* isStarted)
{
    return Loader::IsStarted(isStarted);
}

ECode CLoader::IsAbandoned(
    /* [out] */ Boolean* isAbandoned)
{
    return Loader::IsAbandoned(isAbandoned);
}

ECode CLoader::IsReset(
    /* [out] */ Boolean* isReset)
{
    return Loader::IsReset(isReset);
}

ECode CLoader::StartLoading()
{
    return Loader::StartLoading();
}

ECode CLoader::CancelLoad(
    /* [out] */ Boolean* succeeded)
{
    return Loader::CancelLoad(succeeded);
}

ECode CLoader::ForceLoad()
{
    return Loader::ForceLoad();
}

ECode CLoader::StopLoading()
{
    return Loader::StopLoading();
}

ECode CLoader::Abandon()
{
    return Loader::Abandon();
}

ECode CLoader::Reset()
{
    return Loader::Reset();
}

ECode CLoader::TakeContentChanged(
    /* [out] */ Boolean* succeeded)
{
    return Loader::TakeContentChanged(succeeded);
}

ECode CLoader::OnContentChanged()
{
    return Loader::OnContentChanged();
}

ECode CLoader::DataToString(
    /* [in] */ IInterface* data,
    /* [out] */ String* str)
{
    return Loader::DataToString(data, str);
}

ECode CLoader::ToString(
    /* [out] */ String* str)
{
    return Loader::ToString(str);
}

ECode CLoader::Dump(
    /* [in] */ const String& prefix,
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* writer,
    /* [in] */ ArrayOf<String>* args)
{
    return Loader::Dump(prefix, fd, writer, args);
}

ECode CLoader::constructor(
    /* [in] */ IContext* context)
{
    return Loader::Init(context);
}

}
}
}

