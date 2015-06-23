
#ifndef __CPLUGINLIST_H__
#define __CPLUGINLIST_H__

#include "_CPluginList.h"
#include "ext/frameworkdef.h"
#include "webkit/CPlugin.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Webkit {

class CPlugin;

CarClass(CPluginList)
{
public:
	CARAPI constructor();

	/**
    * Returns the list of plugins as a java.util.List.
    *
    * @hide
    * @deprecated This interface was intended to be used by Gears. Since Gears was
    * deprecated, so is this class.
    */
    //synchronized
    CARAPI GetList(
    	/* [out] */ IObjectContainer** plugins);

	/**
    * Adds a plugin to the list.
    *
    * @hide
    * @deprecated This interface was intended to be used by Gears. Since Gears was
    * deprecated, so is this class.
    */
    //synchronized
    CARAPI AddPlugin(
        /* [in] */ IPlugin* plugin);

    /**
    * Removes a plugin from the list.
    *
    * @hide
    * @deprecated This interface was intended to be used by Gears. Since Gears was
    * deprecated, so is this class.
    */
    //synchronized
    CARAPI RemovePlugin(
        /* [in] */ IPlugin* plugin);

    /**
    * Clears the plugin list.
    *
    * @hide
    * @deprecated This interface was intended to be used by Gears. Since Gears was
    * deprecated, so is this class.
    */
    //synchronized
    CARAPI Clear();

    /**
    * Dispatches the click event to the appropriate plugin.
    *
    * @hide
    * @deprecated This interface was intended to be used by Gears. Since Gears was
    * deprecated, so is this class.
    */
    //synchronized
    CARAPI PluginClicked(
        /* [in] */ IContext* context,
        /* [in] */ Int32 position);

private:
    List< AutoPtr<CPlugin> > mPlugins;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __CPLUGINLIST_H__
