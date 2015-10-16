
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/view/LayoutInflater.h"
#include "elastos/droid/utility/Xml.h"
#include "elastos/droid/R.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringUtils.h>
#ifdef DROID_CORE
#include "elastos/droid/widget/CBlinkLayout.h"
#endif

using Elastos::Core::StringUtils;
using Elastos::Core::IClassLoader;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Utility::Xml;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Content::Res::IXmlResourceParser;
using Elastos::Droid::Widget::CBlinkLayout;
using Elastos::Droid::Widget::IFrameLayout;

namespace Elastos {
namespace Droid {
namespace View {

#define LAYOUT_INFLATOR_CATCH_EXCEPTION1(expr) \
    do { \
        ECode ec = expr; \
        if (ec == (Int32)E_NO_SUCH_METHOD_EXCEPTION) { \
            Slogger::D(TAG, "LAYOUT_INFLATOR_CATCH_EXCEPTION1: %s, Error: E_NO_SUCH_METHOD_EXCEPTION.", name.string()); \
            /*InflateException ie = new InflateException(attrs.getPositionDescription()*/ \
                  /*+ ": Error inflating class "*/ \
                  /*+ (prefix != null ? (prefix + name) : name));*/ \
            /*ie.initCause(e);*/ \
            return ec; \
        } \
        else if (ec == (Int32)E_CLASS_NOT_FOUND_EXCEPTION) { \
            Slogger::D(TAG, "LAYOUT_INFLATOR_CATCH_EXCEPTION1: %s, Error: E_CLASS_NOT_FOUND_EXCEPTION.", name.string()); \
            /* If loadClass fails, we should propagate the exception. */ \
            return ec; \
        } \
        else if (FAILED(ec)) { \
            Slogger::D(TAG, "LAYOUT_INFLATOR_CATCH_EXCEPTION1: %s, Error: %08x.", name.string(), ec); \
            /*InflateException ie = new InflateException(attrs.getPositionDescription()*/ \
                    /*+ ": Error inflating class "*/ \
                    /*+ (clazz == null ? "<unknown>" : clazz.getName()));*/ \
            /*ie.initCause(e);*/ \
            return ec; \
        } \
    } while(0);


#define LAYOUT_INFLATOR_CATCH_EXCEPTION2(expr) \
    do { \
        ECode ec = expr; \
        if (ec == (Int32)E_XML_PULL_PARSER_EXCEPTION) { \
            Slogger::D(TAG, "LAYOUT_INFLATOR_CATCH_EXCEPTION2: Error: E_XML_PULL_PARSER_EXCEPTION."); \
            /*InflateException ex = new InflateException(e.getMessage());*/ \
            /*ie.initCause(e);*/ \
            mConstructorArgs->Set(0, lastContext); \
            mConstructorArgs->Set(1, NULL); \
            return ec; \
        } \
        else if (ec == (Int32)E_IO_EXCEPTION) { \
            Slogger::D(TAG, "LAYOUT_INFLATOR_CATCH_EXCEPTION2: Error: E_IO_EXCEPTION."); \
            /*InflateException ie = new InflateException(*/ \
                    /*parser.getPositionDescription()*/ \
                    /*+ ": " + e.getMessage());*/ \
            /*ie.initCause(e);*/ \
            mConstructorArgs->Set(0, lastContext); \
            mConstructorArgs->Set(1, NULL); \
            return ec; \
        } \
        else if (FAILED(ec)) { \
            Slogger::D(TAG, "LAYOUT_INFLATOR_CATCH_EXCEPTION2: Error: %08x.", ec); \
            mConstructorArgs->Set(0, lastContext); \
            mConstructorArgs->Set(1, NULL); \
            return ec; \
        } \
    } while(0);


#define LAYOUT_INFLATOR_CATCH_EXCEPTION3(expr) \
    do { \
        ECode ec = expr; \
        if (ec == (Int32)E_INFLATE_EXCEPTION) { \
            Slogger::D(TAG, "LAYOUT_INFLATOR_CATCH_EXCEPTION3: %s, Error: E_INFLATE_EXCEPTION.", name.string()); \
            return ec; \
        } \
        else if (ec == (Int32)E_CLASS_NOT_FOUND_EXCEPTION) { \
            Slogger::D(TAG, "LAYOUT_INFLATOR_CATCH_EXCEPTION3: %s, Error: E_CLASS_NOT_FOUND_EXCEPTION.", name.string()); \
            /*InflateException ie = new InflateException(attrs.getPositionDescription()*/ \
                    /*+ ": Error inflating class " + name);*/ \
            /*ie.initCause(e);*/ \
            return ec;; \
        } \
        else if (FAILED(ec)) { \
            Slogger::D(TAG, "LAYOUT_INFLATOR_CATCH_EXCEPTION3: %s, Error: %08x.", name.string(), ec); \
            /*InflateException ie = new InflateException(attrs.getPositionDescription()*/ \
                    /*+ ": Error inflating class " + name);*/ \
            /*ie.initCause(e);*/ \
            return ec;; \
        } \
    } while(0);

#define FAIL_RETURN_WITH_CLOSE(expr) \
    do { \
        ECode ec = expr; \
        if (FAILED(ec)) { \
            childParser->Close(); \
            return ec; \
        } \
    } while(0);

const String LayoutInflater::TAG("LayoutInflater");
const String LayoutInflater::TAG_MERGE("merge");
const String LayoutInflater::TAG_INCLUDE("include");
const String LayoutInflater::TAG_1995("blink");
const String LayoutInflater::TAG_REQUEST_FOCUS("requestFocus");

/*
 * to be careful: the value of mConstructorSignature should be
 * be consistent with constructor methods of all widgets declared
 * in *.car.
 */
String LayoutInflater::mConstructorSignature("CtxAttrs");

AutoPtr< HashMap<String, AutoPtr<IConstructorInfo> > > LayoutInflater::sConstructorMap = \
    new HashMap<String, AutoPtr<IConstructorInfo> >(30);

//=======================================================================================
// LayoutInflater::FactoryMerger
//=======================================================================================

CAR_INTERFACE_IMPL_2(LayoutInflater::FactoryMerger, ILayoutInflaterFactory, ILayoutInflaterFactory2)

LayoutInflater::FactoryMerger::FactoryMerger(
    /* [in] */ ILayoutInflaterFactory* f1,
    /* [in] */ ILayoutInflaterFactory2* f12,
    /* [in] */ ILayoutInflaterFactory* f2,
    /* [in] */ ILayoutInflaterFactory2* f22)
    : mF1(f1)
    , mF2(f2)
    , mF12(f12)
    , mF22(f22)
{
}

ECode LayoutInflater::FactoryMerger::OnCreateView(
    /* [in] */ const String& name,
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);

    AutoPtr<IView> v;
    ECode ec = mF1->OnCreateView(name, context, attrs, (IView**)&v);
    if (v == NULL) {
        ec = mF2->OnCreateView(name, context, attrs, (IView**)&v);
    }

    *view = v;
    REFCOUNT_ADD(*view);
    return ec;
}

ECode LayoutInflater::FactoryMerger::OnCreateView(
    /* [in] */ IView* parent,
    /* [in] */ const String& name,
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);

    ECode ec = NOERROR;
    AutoPtr<IView> v;
    if (mF12 != NULL) {
        ec = mF12->OnCreateView(parent, name, context, attrs, (IView**)&v);
    }
    else {
        ec = mF1->OnCreateView(name, context, attrs, (IView**)&v);
    }

    if (v == NULL) {
        if (mF22 != NULL) {
            ec = mF22->OnCreateView(parent, name, context, attrs, (IView**)&v);
        }
        else {
            ec = mF2->OnCreateView(name, context, attrs, (IView**)&v);
        }
    }

    *view = v;
    REFCOUNT_ADD(*view);
    return ec;
}

//=======================================================================================
// LayoutInflater
//=======================================================================================
LayoutInflater::LayoutInflater()
    : DEBUG(FALSE)
    , mContext(NULL)
    , mPrivateFactory(NULL)
{
    mConstructorArgs = ArrayOf<IInterface* >::Alloc(2);
}

LayoutInflater::~LayoutInflater()
{
    mConstructorArgs = NULL;
    mFactory = NULL;
    mFactory2 = NULL;
    mFilter = NULL;
    mFilterMap = NULL;
}

ECode LayoutInflater::From(
    /* [in] */ IContext* context,
    /* [out] */ ILayoutInflater** inflater)
{
    VALIDATE_NOT_NULL(inflater);

    ECode ec = context->GetSystemService(
            IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)inflater);
    if (FAILED(ec) || (*inflater == NULL)) {
        Slogger::W(TAG, "LayoutInflater not found.");
        return E_ASSERTION_ERROR;
    }
    return ec;
}

/**
 * Return the context we are running in, for access to resources, class
 * loader, etc.
 */
AutoPtr<IContext> LayoutInflater::GetContext()
{
    return mContext;
}

ECode LayoutInflater::GetFactory(
    /* [out] */ ILayoutInflaterFactory** factory)
{
    VALIDATE_NOT_NULL(factory);
    *factory = mFactory;
    REFCOUNT_ADD(*factory);
    return NOERROR;
}

ECode LayoutInflater::GetFactory2(
    /* [out] */ ILayoutInflaterFactory2** factory)
{
    VALIDATE_NOT_NULL(factory);
    *factory = mFactory2;
    REFCOUNT_ADD(*factory);
    return NOERROR;
}

ECode LayoutInflater::SetFactory(
    /* [in] */ ILayoutInflaterFactory* factory)
{
    if (mFactorySet) {
        Slogger::E(TAG, "A factory has already been set on this LayoutInflater");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (factory == NULL) {
        Slogger::E(TAG, "Given factory can not be null");
        return E_NULL_POINTER_EXCEPTION;
    }

    mFactorySet = TRUE;
    if (mFactory == NULL) {
        mFactory = factory;
    }
    else {
        AutoPtr<ILayoutInflaterFactory> old = mFactory;
        mFactory = new FactoryMerger(factory, NULL, old, mFactory2);
    }
    return NOERROR;
}

ECode LayoutInflater::SetFactory2(
    /* [in] */ ILayoutInflaterFactory2* factory)
{
    if (mFactorySet) {
        Slogger::E(TAG, "A factory has already been set on this LayoutInflater");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (factory == NULL) {
        Slogger::E(TAG, "Given factory can not be null");
        return E_NULL_POINTER_EXCEPTION;
    }

    mFactorySet = TRUE;
    if (mFactory == NULL) {
        mFactory = factory;
        mFactory2 = factory;
    }
    else {
        AutoPtr<ILayoutInflaterFactory> old = mFactory;
        mFactory = new FactoryMerger(factory, factory, old, mFactory2);
    }
    return NOERROR;
}

ECode LayoutInflater::SetPrivateFactory(
    /* [in] */ ILayoutInflaterFactory2* factory)
{
    mPrivateFactory = factory;
    return NOERROR;
}

ECode LayoutInflater::GetFilter(
    /* [out] */ ILayoutInflaterFilter** filter)
{
    VALIDATE_NOT_NULL(filter);
    *filter = mFilter;
    REFCOUNT_ADD(*filter);
    return NOERROR;
}

ECode LayoutInflater::SetFilter(
    /* [in] */ ILayoutInflaterFilter* filter)
{
    mFilter = filter;
    if (filter != NULL) {
        mFilterMap = new HashMap<String, Boolean>();
    }
    return NOERROR;
}

/**
 * Inflate a new view hierarchy from the specified xml resource. Throws
 * {@link InflateException} if there is an error.
 *
 * @param resource ID for an XML layout resource to load (e.g.,
 *        <code>R.layout.main_page</code>)
 * @param root Optional view to be the parent of the generated hierarchy.
 * @return The root View of the inflated hierarchy. If root was supplied,
 *         this is the root View; otherwise it is the root of the inflated
 *         XML file.
 */
ECode LayoutInflater::Inflate(
    /* [in] */ Int32 resource,
    /* [in] */ IViewGroup* root,
    /* [out] */ IView** view)
{
    return Inflate(resource, root, root != NULL, view);
}

ECode LayoutInflater::Inflate(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IViewGroup* root,
    /* [out] */ IView** view)
{
    return Inflate(parser, root, root != NULL, view);
}

ECode LayoutInflater::Inflate(
    /* [in] */ Int32 resource,
    /* [in] */ IViewGroup* root,
    /* [in] */ Boolean attachToRoot,
    /* [out] */ IView** view)
{
    assert(view != NULL);

    if (DEBUG) {
        Slogger::D(TAG, "Inflate from resource: %08x", resource);
    }

    AutoPtr<IResources> res;
    ASSERT_SUCCEEDED(mContext->GetResources((IResources**)&res));
    AutoPtr<IXmlResourceParser> parser;
    ASSERT_SUCCEEDED(res->GetLayout(resource, (IXmlResourceParser**)&parser));
    ECode ec = Inflate(parser, root, attachToRoot, view);
    parser->Close();
    return ec;
}

ECode LayoutInflater::Inflate(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IViewGroup* root,
    /* [in] */ Boolean attachToRoot,
    /* [out] */ IView** view)
{
    assert(view != NULL);
    AutoLock lock(mConstructorArgsLock);

    AutoPtr<IAttributeSet> attrs = Xml::AsAttributeSet(parser);
    AutoPtr<IContext> lastContext = (IContext*)((*mConstructorArgs)[0]);
    mConstructorArgs->Set(0, (IInterface*)mContext);
    AutoPtr<IView> result = root != NULL? (IView*)root->Probe(EIID_IView) : NULL;

//    try {
    // Look for the root node.
    Int32 type;
    LAYOUT_INFLATOR_CATCH_EXCEPTION2(parser->Next(&type));
    while ((parser->Next(&type), type != IXmlPullParser::START_TAG) &&
            type != IXmlPullParser::END_DOCUMENT) {
        // Empty
    }

    if (type != IXmlPullParser::START_TAG) {
        Slogger::E(TAG, "InflateException No start tag found!");
        mConstructorArgs->Set(0, lastContext);
        mConstructorArgs->Set(1, NULL);
        return E_INFLATE_EXCEPTION;
    }

    String name;
    parser->GetName(&name);
    Int32 tmpIndex = name.LastIndexOf('.');
    if (tmpIndex >= 0) {
        //Get the last word.
        Int32 tmpLen = name.GetLength() - tmpIndex + 1;

        assert(tmpLen > 0);
        name = name.Substring(tmpIndex + 1, name.GetLength());
    }

    if (DEBUG) {
        Slogger::D(TAG, "**************************");
        Slogger::D(TAG, "Creating root view: %s", name.string());
        Slogger::D(TAG, "**************************");
    }

    if (name.Equals(TAG_MERGE)) {
        if (root == NULL || !attachToRoot) {
            Slogger::E(TAG, "<merge /> can be used only with a valid ViewGroup root and attachToRoot=true");
            mConstructorArgs->Set(0, lastContext);
            mConstructorArgs->Set(1, NULL);
            return E_INFLATE_EXCEPTION;
        }

        LAYOUT_INFLATOR_CATCH_EXCEPTION2(RInflate(parser, root, attrs.Get(), FALSE));
    }
    else {
        // Temp is the root view that was found in the xml
        AutoPtr<IView> temp;

        if (name.Equals(TAG_1995)) {
            AutoPtr<IFrameLayout> layout;
            CBlinkLayout::New(mContext, attrs, (IFrameLayout**)&layout);
            temp = IView::Probe(layout);
        }
        else {
            LAYOUT_INFLATOR_CATCH_EXCEPTION2(CreateViewFromTag(root, name, attrs, (IView**)&temp));
        }

        SetViewXmlPath(temp, parser);

        AutoPtr<IViewGroupLayoutParams> params;

        if (root != NULL) {
            if (DEBUG) {
                Slogger::D(TAG, "Creating params from root: %p", root);
            }
            // Create layout params that match root, if supplied
            root->GenerateLayoutParams(attrs, (IViewGroupLayoutParams**)&params);
            assert(params.Get());
            if (!attachToRoot) {
                // Set the layout params for temp if we are not
                // attaching. (If we are, we use addView, below)
                temp->SetLayoutParams(params.Get());
            }
        }

        if (DEBUG) {
            Slogger::D(TAG, "-----> start inflating children");
        }
        // Inflate all children under temp
        LAYOUT_INFLATOR_CATCH_EXCEPTION2(RInflate(parser, temp.Get(), attrs.Get(), TRUE));
        if (DEBUG) {
            Slogger::D(TAG, "-----> done inflating children");
        }

        // We are supposed to attach all the views we found (int temp)
        // to root. Do that now.
        if (root != NULL && attachToRoot) {
            root->AddView(temp.Get(), params.Get());
        }

        // Decide whether to return the root that was passed in or the
        // top view found in xml.
        if (root == NULL || !attachToRoot) {
            result = temp;
        }
    }

//    } catch (XmlPullParserException e) {
//        InflateException ex = new InflateException(e.getMessage());
//        ex.initCause(e);
//        throw ex;
//    } catch (IOException e) {
//        InflateException ex = new InflateException(
//                parser.getPositionDescription()
//                + ": " + e.getMessage());
//        ex.initCause(e);
//        throw ex;
//    } finally {
//        // Don't retain static reference on context.
//        mConstructorArgs[0] = lastContext;
//        mConstructorArgs[1] = null;
//    }

    *view = result.Get();
    REFCOUNT_ADD(*view);

    mConstructorArgs->Set(0, lastContext);
    mConstructorArgs->Set(1, NULL);
    return NOERROR;
}

ECode LayoutInflater::CreateView(
    /* [in] */ const String& name,
    /* [in] */ const String& prefix_,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IView** view)
{
    String prefix = prefix_;
    if (name.Equals("TabletStatusBarView") || name.Equals("NotificationPanel")) {
        prefix = "Elastos.Droid.SystemUI.StatusBar.Tablet.";
    }
    else if (name.Equals("ActionMenuView")) {
        prefix = "Elastos.Droid.View.Menu.";
    }

    AutoPtr<IConstructorInfo> constructor;
    AutoPtr<IClassInfo> clazz;

//    try {
    HashMap<String, AutoPtr<IConstructorInfo> >::Iterator it = \
        sConstructorMap->Find(name);

    if (it == sConstructorMap->End()) {
        // Class not found in the cache, see if it's real, and try to add it
        AutoPtr<IClassLoader> cl;
        LAYOUT_INFLATOR_CATCH_EXCEPTION1(mContext->GetClassLoader((IClassLoader**)&cl));
        if (!prefix.IsNull()) {
            LAYOUT_INFLATOR_CATCH_EXCEPTION1(cl->LoadClass(prefix + name, (IClassInfo**)&clazz));
        }
        else {
            LAYOUT_INFLATOR_CATCH_EXCEPTION1(cl->LoadClass(name, (IClassInfo**)&clazz));
        }

        if (mFilter != NULL && clazz != NULL) {
            Boolean allowed;
            mFilter->OnLoadClass((Handle32)clazz.Get(), &allowed);
            if (!allowed) {
                LAYOUT_INFLATOR_CATCH_EXCEPTION1(FailNotAllowed(name, prefix, attrs));
            }
        }

        LAYOUT_INFLATOR_CATCH_EXCEPTION1(clazz->GetConstructorInfoByParamNames(
                mConstructorSignature, (IConstructorInfo**)&constructor));
        (*sConstructorMap)[name] = constructor;
    }
    else {
        constructor = it->mSecond;
        // If we have a filter, apply it to cached constructor
        if (mFilter != NULL) {
            // Have we seen this name before?
            Boolean allowedState;
            HashMap<String, Boolean>::Iterator iit = mFilterMap->Find(name);
            if (iit == mFilterMap->End()) {
                // New class -- remember whether it is allowed
                AutoPtr<IClassLoader> cl;
                LAYOUT_INFLATOR_CATCH_EXCEPTION1(mContext->GetClassLoader((IClassLoader**)&cl));
                if (!prefix.IsNull()) {
                    LAYOUT_INFLATOR_CATCH_EXCEPTION1(cl->LoadClass(prefix + name, (IClassInfo**)&clazz));
                }
                else {
                    LAYOUT_INFLATOR_CATCH_EXCEPTION1(cl->LoadClass(name, (IClassInfo**)&clazz));
                }

                Boolean allowed;
                mFilter->OnLoadClass((Handle32)clazz.Get(), &allowed);
                allowed = allowed && (clazz != NULL);
                (*mFilterMap)[name] = allowed;
                if (!allowed) {
                    LAYOUT_INFLATOR_CATCH_EXCEPTION1(FailNotAllowed(name, prefix, attrs));
                }
            }
            else {
                allowedState = iit->mSecond;
                if (!allowedState) {
                    LAYOUT_INFLATOR_CATCH_EXCEPTION1(FailNotAllowed(name, prefix, attrs));
                }
            }
        }
    }

    mConstructorArgs->Set(1, attrs);

    AutoPtr<IArgumentList> args;
    LAYOUT_INFLATOR_CATCH_EXCEPTION1(constructor->CreateArgumentList((IArgumentList**)&args));
    LAYOUT_INFLATOR_CATCH_EXCEPTION1(args->SetInputArgumentOfObjectPtr(0, (*mConstructorArgs)[0]));
    LAYOUT_INFLATOR_CATCH_EXCEPTION1(args->SetInputArgumentOfObjectPtr(1, (*mConstructorArgs)[1]));

    AutoPtr<IInterface> obj;
    LAYOUT_INFLATOR_CATCH_EXCEPTION1(constructor->CreateObject(args, (IInterface**)&obj));
    *view = (IView*)obj->Probe(EIID_IView);
    assert(*view != NULL);
    REFCOUNT_ADD(*view);

    if (IViewStub::Probe(*view) != NULL) {
        // always use ourselves when inflating ViewStub later
        IViewStub::Probe(*view)->SetLayoutInflater((ILayoutInflater*)this->Probe(EIID_ILayoutInflater));
    }

#if defined(_DEBUG) || defined(_ELASTOS_DEBUG)
    // Int32 viewId;
    // (*view)->GetId(&viewId);
    // Slogger::D(TAG, "Created view: %p, id: 0x%08x, type: %s%s ",
    //     *view, viewId, prefix.IsNull() ? "" : prefix.string(), name.string());
#endif
    return NOERROR;

//    } catch (NoSuchMethodException e) {
//        InflateException ie = new InflateException(attrs.getPositionDescription()
//                + ": Error inflating class "
//                + (prefix != null ? (prefix + name) : name));
//        ie.initCause(e);
//        throw ie;
// } catch (ClassCastException e) {
//     // If loaded class is not a View subclass
//     InflateException ie = new InflateException(attrs.getPositionDescription()
//             + ": Class is not a View "
//             + (prefix != null ? (prefix + name) : name));
//     ie.initCause(e);
//     throw ie;
//
//    } catch (ClassNotFoundException e) {
//        // If loadClass fails, we should propagate the exception.
//        throw e;
//    } catch (Exception e) {
//        InflateException ie = new InflateException(attrs.getPositionDescription()
//                + ": Error inflating class "
//                + (clazz == null ? "<unknown>" : clazz.getName()));
//        ie.initCause(e);
//        throw ie;
//    }
}

ECode LayoutInflater::FailNotAllowed(
    /* [in] */ const String& name,
    /* [in] */ const String& prefix,
    /* [in] */ IAttributeSet* attrs)
{
    Slogger::E(TAG, "Class not allowed to be inflated: %s%s", prefix.string(), name.string());
    return E_INFLATE_EXCEPTION;
}

ECode LayoutInflater::OnCreateView(
    /* [in] */ const String& name,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IView** view)
{
    return CreateView(name, String("Elastos.Droid.Widget."), attrs, view);
}

ECode LayoutInflater::OnCreateView(
    /* [in] */ IView* parent,
    /* [in] */ const String& name,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IView** view)
{
    return OnCreateView(name, attrs, view);
}

static String Replace(const String& name, const char* regix, const char* replacement)
{
    String result;
    StringUtils::ReplaceAll(name, String(regix), String(replacement), &result);
    return result;
}

String LayoutInflater::ConvertViewName(
    /* [in] */ const String& nameIn)
{
    String name(nameIn.string());
    if (name.IndexOf("$") >= 0) {
        // inner class case: systemui.statusbar.tablet.NotificationIconArea$IconLayout
        // NotificationIconArea$IconLayout to NotificationIconAreaIconLayout
        name = Replace(name, "\\$", "");
    }

    if (name.StartWith("com.android.internal.widget.")) {
        return Replace(name, "com.android.internal.widget.",
            "Elastos.Droid.Widget.Internal.");
    }
    // else if (name.StartWith("com.android.systemui.statusbar.tablet.NotificationIconArea$IconLayout")) {
    //     return Replace(name, "com.android.systemui.statusbar.tablet.NotificationIconArea$IconLayout",
    //         "Elastos.Droid.SystemUI.StatusBar.Tablet.NotificationIconAreaIconLayout");
    // }
    else if (name.StartWith("com.android.systemui.statusbar.tablet.")) {
        return Replace(name, "com.android.systemui.statusbar.tablet.",
            "Elastos.Droid.SystemUI.StatusBar.Tablet.");
    }
    else if (name.StartWith("com.android.internal.view.menu.")) {
        return Replace(name, "com.android.internal.view.menu.",
            "Elastos.Droid.View.Menu.");
    }
    else if (name.StartWith("com.android.systemui.statusbar.phone.")) {
        return Replace(name, "com.android.systemui.statusbar.phone.",
            "Elastos.Droid.SystemUI.StatusBar.Phone.");
    }
    else if (name.StartWith("com.android.systemui.statusbar.policy.")) {
        return Replace(name, "com.android.systemui.statusbar.policy.",
            "Elastos.Droid.SystemUI.StatusBar.Policy.");
    }
    else if (name.StartWith("com.android.systemui.statusbar.")) {
        return Replace(name, "com.android.systemui.statusbar.",
            "Elastos.Droid.SystemUI.StatusBar.");
    }
    else if (name.StartWith("com.android.systemui.recent.")) {
        return Replace(name, "com.android.systemui.recent.",
            "Elastos.Droid.SystemUI.Recent.");
    }
    else if (name.StartWith("com.android.systemui.")) {
        assert(0 && "TODO");
    }
    else if(name.Equals("com.android.internal.app.AlertControllerRecycleListView")) {
        return String("Elastos.Droid.Widget.RecycleListView");
    }
    else if (name.StartWith("android.preference.")) {
        return Replace(name, "android.preference.",
            "Elastos.Droid.Preference.");
    }
    else if (name.StartWith("android.opengl.")) {
        return Replace(name, "android.opengl.",
            "Elastos.Droid.Opengl.");
    }
    return name;
}

ECode LayoutInflater::CreateViewFromTag(
    /* [in] */ IView* parent,
    /* [in] */ const String& _name,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IView** view)
{
    String name;
    if (!_name.Compare("view")) {
        attrs->GetAttributeValue(String(NULL), String("class"), &name);
    }
    else {
        name = _name;
    }

    name = ConvertViewName(name);

    if (DEBUG) {
        Slogger::D(TAG, "******** Creating view: %s, mFactory2: %p, mFactory: %p, mPrivateFactory: %p",
            name.string(),  mFactory2.Get(), mFactory.Get(), mPrivateFactory);
    }

//    try {
    if (mFactory2 != NULL) {
        mFactory2->OnCreateView(parent, name, mContext, attrs, view);
    }
    else if (mFactory != NULL) {
        mFactory->OnCreateView(name, mContext, attrs, view);
    }
    else *view = NULL;

    if (*view == NULL && mPrivateFactory != NULL) {
        mPrivateFactory->OnCreateView(parent, name, mContext, attrs, view);
    }

    if (*view == NULL) {
        if (-1 == name.IndexOf('.')) {
            LAYOUT_INFLATOR_CATCH_EXCEPTION3(OnCreateView(parent, name, attrs, view));
        } else {
            LAYOUT_INFLATOR_CATCH_EXCEPTION3(CreateView(name, String(NULL), attrs, view));
        }
    }

    if (DEBUG) {
        Slogger::D(TAG, "Created view is: %p", *view);
    }

    return NOERROR;

//    } catch (InflateException e) {
//        throw e;
//
//    } catch (ClassNotFoundException e) {
//        InflateException ie = new InflateException(attrs.getPositionDescription()
//                + ": Error inflating class " + name);
//        ie.initCause(e);
//        throw ie;
//
//    } catch (Exception e) {
//        InflateException ie = new InflateException(attrs.getPositionDescription()
//                + ": Error inflating class " + name);
//        ie.initCause(e);
//        throw ie;
//    }
}

ECode LayoutInflater::RInflate(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IView* parent,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Boolean finishInflate)
{
    Int32 orgDepth, depth;
    Int32 type;
    String name;

    FAIL_RETURN(parser->GetDepth(&orgDepth));
    FAIL_RETURN(parser->Next(&type));
    FAIL_RETURN(parser->GetDepth(&depth));
    while ((type != IXmlPullParser::END_TAG ||
            depth > orgDepth) && type != IXmlPullParser::END_DOCUMENT) {

        if (type != IXmlPullParser::START_TAG) {
            FAIL_RETURN(parser->Next(&type));
            FAIL_RETURN(parser->GetDepth(&depth));
            continue;
        }

        FAIL_RETURN(parser->GetName(&name));
        if (DEBUG)
        {
           // System.out.println("LayoutInflater : rInflate : name  === " + name );
        }

        if (name.Equals(TAG_REQUEST_FOCUS)) {
            FAIL_RETURN(ParseRequestFocus(parser, parent));
        }
        else if (String(TAG_INCLUDE).Equals(name)) {
            Int32 d;
            FAIL_RETURN(parser->GetDepth(&d));
            if (d == 0) {
//                throw new InflateException("<include /> cannot be the root element");
                return E_INFLATE_EXCEPTION;
            }
            FAIL_RETURN(ParseInclude(parser, parent, attrs));
        }
        else if (name.Equals(TAG_MERGE)) {
//            throw new InflateException("<merge /> must be the root element");
            return E_INFLATE_EXCEPTION;
        }
        else if (name.Equals(TAG_1995)) {
            AutoPtr<IFrameLayout> layout;
            CBlinkLayout::New(mContext, attrs, (IFrameLayout**)&layout);
            AutoPtr<IView> view = (IView*)layout->Probe(EIID_IView);
            AutoPtr<IViewGroup> viewGroup = (IViewGroup*)parent->Probe(EIID_IViewGroup);
            AutoPtr<IViewGroupLayoutParams> params;
            FAIL_RETURN(viewGroup->GenerateLayoutParams(attrs, (IViewGroupLayoutParams**)&params));
            FAIL_RETURN(RInflate(parser, view, attrs, TRUE));
            FAIL_RETURN(viewGroup->AddView(view, params));
            SetViewXmlPath( view, parser);
        }
        else {
            AutoPtr<IView> view;
            FAIL_RETURN(CreateViewFromTag(parent, name, attrs, (IView**)&view));
            AutoPtr<IViewGroup> viewGroup = (IViewGroup*)parent->Probe(EIID_IViewGroup);
            AutoPtr<IViewGroupLayoutParams> params;
            FAIL_RETURN(viewGroup->GenerateLayoutParams(attrs, (IViewGroupLayoutParams**)&params));
            FAIL_RETURN(RInflate(parser, view, attrs, TRUE));
            viewGroup->AddView(view, params);
            SetViewXmlPath(view, parser);
        }

        FAIL_RETURN(parser->Next(&type));
        FAIL_RETURN(parser->GetDepth(&depth));
    }

    if (finishInflate) {
        parent->OnFinishInflate();
    }

    return NOERROR;
}

void LayoutInflater::SetViewXmlPath(
    /* [in] */ IView* view,
    /* [in] */ IXmlPullParser* parser)
{
    AutoPtr<IXmlResourceParser> xmlP = IXmlResourceParser::Probe(parser);

    if(xmlP)
    {
        String xmlPath;
        xmlP->GetXmlPath(&xmlPath);
        view->SetXmlPath(xmlPath);
        view->SetInflaterContext(mContext);
    }


    if ( DEBUG )
    {
        // Context context;
        // ApplicationInfo appinfo;

        // context = view.getContext();
        // appinfo = context.getApplicationInfo();

    }
}

ECode LayoutInflater::ParseRequestFocus(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IView* parent)
{
    Int32 type;
    Boolean hasFocus;
    Int32 currentDepth, depth;

    parent->RequestFocus(&hasFocus);
    FAIL_RETURN(parser->GetDepth(&currentDepth));
    FAIL_RETURN(parser->Next(&type));
    FAIL_RETURN(parser->GetDepth(&depth));
    while ((type != IXmlPullParser::END_TAG ||
            depth > currentDepth) && type != IXmlPullParser::END_DOCUMENT) {
        // Empty
        FAIL_RETURN(parser->Next(&type));
        FAIL_RETURN(parser->GetDepth(&depth));
    }
    return NOERROR;
}

ECode LayoutInflater::ParseInclude(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IView* parent,
    /* [in] */ IAttributeSet* attrs)
{
    Int32 type;

    AutoPtr<IViewGroup> group = (IViewGroup*)parent->Probe(EIID_IViewGroup);
    if (group != NULL) {
        Int32 layout;
        attrs->GetAttributeResourceValue(String(NULL), String("layout"), 0, &layout);
        if (layout == 0) {
            String value;
            attrs->GetAttributeValue(String(NULL), String("layout"), &value);
            if (value.IsNull()) {
//                throw new InflateException("You must specifiy a layout in the"
//                        + " include tag: <include layout=\"@layout/layoutID\" />");
                return E_INFLATE_EXCEPTION;
            } else {
//                throw new InflateException("You must specifiy a valid layout "
//                        + "reference. The layout ID " + value + " is not valid.");
                return E_INFLATE_EXCEPTION;
            }
        } else {
            AutoPtr<IResources> res;
            ASSERT_SUCCEEDED(mContext->GetResources((IResources**)&res));

            AutoPtr<IXmlResourceParser> childParser;
            ASSERT_SUCCEEDED(res->GetLayout(layout, (IXmlResourceParser**)&childParser));

//            try {
            AutoPtr<IAttributeSet> childAttrs = Xml::AsAttributeSet(childParser);

            FAIL_RETURN_WITH_CLOSE(childParser->Next(&type));
            while (type != IXmlPullParser::START_TAG && type != IXmlPullParser::END_DOCUMENT) {
                // Empty.
                FAIL_RETURN_WITH_CLOSE(childParser->Next(&type));
            }

            if (type != IXmlPullParser::START_TAG) {
//                throw new InflateException(childParser.getPositionDescription() +
//                        ": No start tag found!");
                childParser->Close();
                return E_INFLATE_EXCEPTION;
            }

            String childName;
            FAIL_RETURN_WITH_CLOSE(childParser->GetName(&childName));

            if (childName.Equals(TAG_MERGE)) {
                // Inflate all children.
                FAIL_RETURN_WITH_CLOSE(RInflate(childParser, parent, childAttrs, FALSE));
            } else {
                AutoPtr<IView> view;
                FAIL_RETURN_WITH_CLOSE(CreateViewFromTag(parent, childName, childAttrs, (IView**)&view));

                // We try to load the layout params set in the <include /> tag. If
                // they don't exist, we will rely on the layout params set in the
                // included XML file.
                // During a layoutparams generation, a runtime exception is thrown
                // if either layout_width or layout_height is missing. We catch
                // this exception and set localParams accordingly: true means we
                // successfully loaded layout params from the <include /> tag,
                // false means we need to rely on the included layout params.
                AutoPtr<IViewGroupLayoutParams> params;
//                try {
                ECode ec = group->GenerateLayoutParams(attrs, (IViewGroupLayoutParams**)&params);
                if (ec == (Int32)E_RUNTIME_EXCEPTION) {
                    params = NULL;
                    FAIL_RETURN_WITH_CLOSE(group->GenerateLayoutParams(childAttrs, (IViewGroupLayoutParams**)&params));
                }
//                } catch (RuntimeException e) {
//                    params = group.generateLayoutParams(childAttrs);
//                } finally {
                if (params != NULL) {
                    PFL_EX("LayoutInflater::ParseInclude %p", params.Get());
                    view->SetLayoutParams(params.Get());
                }
//                }
                SetViewXmlPath(view, childParser);

                // Inflate all children.
                FAIL_RETURN_WITH_CLOSE(RInflate(childParser, view, childAttrs, TRUE));

                // Attempt to override the included layout's android:id with the
                // one set on the <include /> tag itself.
                AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
                    const_cast<Int32 *>(R::styleable::View),
                    ARRAY_SIZE(R::styleable::View));
                AutoPtr<ITypedArray> a;
                mContext->ObtainStyledAttributes(attrs, attrIds, 0, 0, (ITypedArray**)&a);
                Int32 id;
                a->GetResourceId(R::styleable::View_id, IView::NO_ID, &id);
                // While we're at it, let's try to override android:visibility.
                Int32 visibility;
                a->GetInt32(R::styleable::View_visibility, -1, &visibility);
                a->Recycle();

                if (id != IView::NO_ID) {
                    view->SetId(id);
                }

                switch (visibility) {
                    case 0:
                        view->SetVisibility(IView::VISIBLE);
                        break;
                    case 1:
                        view->SetVisibility(IView::INVISIBLE);
                        break;
                    case 2:
                        view->SetVisibility(IView::GONE);
                        break;
                }

                group->AddView(view);
            }

            childParser->Close();
        }
    } else {
//        throw new InflateException("<include /> can only be used inside of a ViewGroup");
        return E_INFLATE_EXCEPTION;
    }

    Int32 currentDepth, depth;
    FAIL_RETURN(parser->GetDepth(&currentDepth));
    FAIL_RETURN(parser->Next(&type));
    FAIL_RETURN(parser->GetDepth(&depth));
    while ((type != IXmlPullParser::END_TAG ||
            depth > currentDepth) &&
            type != IXmlPullParser::END_DOCUMENT) {
        // Empty
        FAIL_RETURN(parser->Next(&type));
        FAIL_RETURN(parser->GetDepth(&depth));
    }
    return NOERROR;
}

ECode LayoutInflater::Init(
    /* [in] */ IContext* context)
{
    mContext = context;
    return NOERROR;
}

ECode LayoutInflater::Init(
    /* [in] */ LayoutInflater* original,
    /* [in] */ IContext* newContext)
{
    mContext = newContext;
    mFactory = original->mFactory;
    mFactory2 = original->mFactory2;
    mPrivateFactory = original->mPrivateFactory;
    mFilter = original->mFilter;
    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
