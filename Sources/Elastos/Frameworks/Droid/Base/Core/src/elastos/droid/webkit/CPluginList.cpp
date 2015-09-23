
#include "ext/frameworkext.h"
#include "webkit/CPluginList.h"
#include <elastos/utility/etl/Algorithm.h>



namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CPluginList::constructor()
{
    return NOERROR;
}

ECode CPluginList::GetList(
    /* [out] */ IObjectContainer** plugins)
{
    VALIDATE_NOT_NULL(plugins);
    AutoLock lock(_m_syncLock);
    FAIL_RETURN(CObjectContainer::New(plugins));
    List< AutoPtr<CPlugin> >::Iterator it;
    for (it = mPlugins.Begin(); it != mPlugins.End(); ++it) {
        (*plugins)->Add((IPlugin*)it->Get());
    }
    return NOERROR;
}

ECode CPluginList::AddPlugin(
    /* [in] */ IPlugin* plugin)
{
    AutoLock lock(_m_syncLock);
    if (Find(mPlugins.Begin(), mPlugins.End(), plugin) == mPlugins.End()) {
        mPlugins.PushBack((CPlugin*)plugin);
    }
    return NOERROR;
}

ECode CPluginList::RemovePlugin(
    /* [in] */ IPlugin* plugin)
{
    AutoLock lock(_m_syncLock);
    mPlugins.Remove((CPlugin*)plugin);
    return NOERROR;
}

ECode CPluginList::Clear()
{
    AutoLock lock(_m_syncLock);
    mPlugins.Clear();
    return NOERROR;
}

ECode CPluginList::PluginClicked(
    /* [in] */ IContext * context,
    /* [in] */ Int32 position)
{
    AutoLock lock(_m_syncLock);
    AutoPtr<CPlugin> plugin = mPlugins[position];
    if (plugin != NULL) {
        plugin->DispatchClickEvent(context);
    }
    return NOERROR;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
