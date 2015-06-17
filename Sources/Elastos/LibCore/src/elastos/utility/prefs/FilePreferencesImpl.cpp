
#include "FilePreferencesImpl.h"

namespace Elastos {
namespace Utility {
namespace Prefs {

extern "C" const InterfaceID EIID_FilePreferencesImpl =
        { 0x41993109, 0xfb1f, 0x40e9, { 0xb3, 0x58, 0xcf, 0xac, 0xe7, 0xb4, 0x44, 0x16 } };

CAR_INTERFACE_IMPL_2(FilePreferencesImpl, Object, IAbstractPreferences, IPreferences);

const String FilePreferencesImpl::PREFS_FILE_NAME = String("prefs.xml");
FilePreferencesImpl::FilePreferencesImpl(
    /* [in] */ const String& path,
    /* [in] */ Boolean isUserNode)
    : AbstractPreferences(NULL, "")
    , mPath(path)
    , mUserNode(isUserNode)
{
    InitPrefs();
}

FilePreferencesImpl::FilePreferencesImpl(
    /* [in] */ IAbstractPreferences* parent,
    /* [in] */ const String& name)
    : AbstractPreferences(parent, name)
{
    mPath = ((FilePreferencesImpl*) parent)->mPath + File::sSeparator + name;
    InitPrefs();
}

void FilePreferencesImpl::InitPrefs()
{
    CFile::New(mPath, (IFile**)&mDir);
    dir->Exists(&mNewNode);
    mNewNode = !mNewNode;
    CFile::New(path + File::sSeparator + PREFS_FILE_NAME, (IFile**)&mPrefsFile);
    XMLParser::ReadXmlPreferences(prefsFile, (IProperties**)&mPrefs);
}

AutoPtr<ArrayOf<String> > FilePreferencesImpl::ChildrenNamesSpi(
    /* [out, callee] */ AutoPtr<ArrayOf<String> >** list)
{
    String[] names = dir.list(new FilenameFilter() {
        public boolean accept(File parent, String name) {
            return new File(path + File.separator + name).isDirectory();
        }
    });
    if (names == null) {// file is not a directory, exception case
        throw new BackingStoreException("Cannot get child names for " + toString()
                + " (path is " + path + ")");
    }
    return names;
}

AutoPtr<IAbstractPreferences> FilePreferencesImpl::ChildSpi(
    /* [in] */ const String& name)
{
    AutoPtr<FilePreferencesImpl> child = new FilePreferencesImpl(this, name);
    return child;
}

ECode FilePreferencesImpl::FlushSpi() /*throws BackingStoreException*/
{
    try {
        //if removed, return
        if(isRemoved()){
            return;
        }
        // reload
        Properties currentPrefs = XMLParser.readXmlPreferences(prefsFile);
        // merge
        Iterator<String> it = removed.iterator();
        while (it.hasNext()) {
            currentPrefs.remove(it.next());
        }
        removed.clear();
        it = updated.iterator();
        while (it.hasNext()) {
            Object key = it.next();
            currentPrefs.put(key, prefs.get(key));
        }
        updated.clear();
        // flush
        prefs = currentPrefs;
        XMLParser.writeXmlPreferences(prefsFile, prefs);
    } catch (Exception e) {
        // throw new BackingStoreException(e);
        return E_BACKING_STORE_EXCEPTION;
    }
}

String FilePreferencesImpl::GetSpi(
    /* [in] */ const String& key)
{
    if (mPrefs == NULL) {
        mPrefs = XMLParser::ReadXmlPreferences(prefsFile);
    }

    String spi;
    ECode ec = mPrefs->GetProperty(key, &spi);
    if (FAILED(ec)) {
        return String(NULL);
    }

    return spi;
}

AutoPtr<ArrayOf<IInterface*> > FilePreferencesImpl::KeysSpi()
{
    AutoPtr<ISet> ks;
    IMap::Probe(mPrefs)->GetKeySet((ISet**)&ks);
    Int32 size = 0;
    ks->GetSize(&size);
    AutoPtr<ArrayOf<IInterface*> > datas = ArrayOf<<IInterface*> >::Alloc(size);
    ks->ToArray((ArrayOf<IInterface*>**)&datas);
    return datas;
}

ECode FilePreferencesImpl::PutSpi(
    /* [in] */ const String& name,
    /* [in] */ const String& value)
{
    mPrefs->SetProperty(name, value);
    return mUpdated->Add(name);
}

ECode FilePreferencesImpl::RemoveNodeSpi()
{
    mPrefsFile->Delete();
    Boolean removeSucceed = FALSE;
    mDir->Delete(&removeSucceed);
    if (!removeSucceed) {
        // throw new BackingStoreException("Cannot remove " + toString());
        return E_BACKING_STORE_EXCEPTION;
    }
    return NOERROR;
}

ECode FilePreferencesImpl::RemoveSpi(
    /* [in] */ const String& key)
{
    mPrefs->Remove(key);
    mUpdated->Remove(key);
    return mRemoved->Add(key);
}

ECode FilePreferencesImpl::SyncSpi()
{
    return FlushSpi();
}

ECode FilePreferencesImpl::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = String("Elastos.Utility.Prefs.EIID_FilePreferencesImpl");
    return NOERROR;
}

} // namespace Prefs
} // namespace Utility
} // namespace Elastos
