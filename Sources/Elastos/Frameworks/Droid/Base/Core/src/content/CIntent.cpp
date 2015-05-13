
#include "content/CIntent.h"

namespace Elastos {
namespace Droid {
namespace Content {

IINTENT_METHODS_IMPL(CIntent, Intent)

PInterface CIntent::Probe(
    /* [in] */ REIID riid)
{
    return _CIntent::Probe(riid);
}

ECode CIntent::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return Intent::ReadFromParcel(source);
}

ECode CIntent::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return Intent::WriteToParcel(dest);
}

ECode CIntent::constructor()
{
    return Intent::Init();
}

ECode CIntent::constructor(
    /* [in] */ IIntent* intent)
{
    return Intent::Init(intent);
}

ECode CIntent::constructor(
    /* [in] */ IIntent* intent,
    /* [in] */ Boolean all)
{
    return Intent::Init(intent, all);
}

ECode CIntent::constructor(
    /* [in] */ const String& action)
{
    return Intent::Init(action);
}

ECode CIntent::constructor(
    /* [in] */ const String& action,
    /* [in] */ IUri* uri)
{
    return Intent::Init(action, uri);
}

ECode CIntent::constructor(
    /* [in] */ IContext* packageContext,
    /* [in] */ const ClassID& clsId)
{
    return Intent::Init(packageContext, clsId);
}

ECode CIntent::constructor(
    /* [in] */ const String& action,
    /* [in] */ IUri* uri,
    /* [in] */ IContext* packageContext,
    /* [in] */ IClassInfo* cls)
{
    return Intent::Init(action, uri, packageContext, cls);
}

} // Content
} // Droid
} // Elastos
