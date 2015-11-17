##########################################################################
#
# File:    $XDK/DevKit/misc/makerules_gnu.mk
# Purpose: Building rules for GCC compiler
#

##########################################################################
#
# Intermediate targets
#
.SUFFIXES: .h .def .exp .lib .S .asm .rc .car .cfg .lub

%$(OBJ_SUFFIX): $(MAKEDIR)/%.S
	@echo Compiling $< ...
	@echo $(ASM) $(C_FLAGS) $(DEPEND_FLAGS) -o $@ $< $(BLACKHOLE)
	$(ASM) $(C_FLAGS) $(DEPEND_FLAGS) -o $@ $<

%$(OBJ_SUFFIX): %.S
	@echo Compiling $< ...
	@echo $(ASM) $(C_FLAGS) $(DEPEND_FLAGS) -o $@ $< $(BLACKHOLE)
	$(ASM) $(C_FLAGS) $(DEPEND_FLAGS) -o $@ $<

%$(OBJ_SUFFIX): $(MAKEDIR)/%.c
	@echo Compiling $< ...
	@echo $(CC) $(C_FLAGS) $(DEPEND_FLAGS) -o $@ $< $(BLACKHOLE)
	$(CC) $(C_FLAGS) $(DEPEND_FLAGS) -o $@ $<

%$(OBJ_SUFFIX): %.c
	@echo Compiling mirror directory $< ...
	$(CC) $(C_FLAGS) $(DEPEND_FLAGS) -o $@ $< $(BLACKHOLE)
	$(CC) $(C_FLAGS) $(DEPEND_FLAGS) -o $@ $<

%$(OBJ_SUFFIX): $(MAKEDIR)/%.cc
	@echo Compiling $< ...
	$(CC) $(C_FLAGS) $(DEPEND_FLAGS) -o $@ $< $(BLACKHOLE)
	$(CC) $(C_FLAGS) $(DEPEND_FLAGS) -o $@ $<

ifdef APPPACK
ifeq "$(shell if [ -d $(MAKEDIR)/res ]; then echo exist; fi)" "exist"
DEPEND_LIST := $(shell find $(MAKEDIR)/res -name "*")
$(MAKEDIR)/R.cpp: $(MAKEDIR)/AndroidManifest.xml $(MAKEDIR)/res $(DEPEND_LIST) $(MAKEDIR)/$(TARGET_NAME).car
	if [ -d $(MAKEDIR)/gen ]; then rm -rf $(MAKEDIR)/gen;fi
	if [ ! -d $(MAKEDIR)/gen ]; then mkdir $(MAKEDIR)/gen;fi
ifndef RES_NAMESPACE
	@cd $(MAKEDIR); \
	sed -n 's/    namespace \(.*\) [\{]/\1/p' $(MAKEDIR)/$(TARGET_NAME).car | tr -s "\n" "\." | sed -n 's/\(.*\)\./\1/p' >temp; \
	aapt package -f -m -J gen -S res --cpp `cat $(MAKEDIR)/temp` -I $(EMULATOR_PATH)/platforms/android-21/android.jar -M $(MAKEDIR)/AndroidManifest.xml
else
	@cd $(MAKEDIR); \
	aapt package -f -m -J gen -S res --cpp $(RES_NAMESPACE) -I $(EMULATOR_PATH)/platforms/android-21/android.jar -M $(MAKEDIR)/AndroidManifest.xml
endif
	mv -f $(MAKEDIR)/gen/$(TARGET_NAME)/R.cpp $(MAKEDIR)/gen/$(TARGET_NAME)/R.h $(MAKEDIR)
	rm -rf $(MAKEDIR)/temp
	rm -rf $(MAKEDIR)/gen
endif
endif

%$(OBJ_SUFFIX): $(MAKEDIR)/%.cpp
	@echo Compiling $< ...
	@echo $(CXX) $(CPP_FLAGS) -fvisibility=hidden $(C_FLAGS) $(DEPEND_FLAGS) -o $@ $< $(BLACKHOLE)
	$(CXX) $(CPP_FLAGS) $(C_FLAGS) $(DEPEND_FLAGS) -fvisibility=hidden -o $@ $<

%$(OBJ_SUFFIX): %.cpp
	@echo Compiling mirror directory $< ...
	@echo $(CXX) $(CPP_FLAGS) -fvisibility=hidden $(C_FLAGS) $(DEPEND_FLAGS) -o $@ $< $(BLACKHOLE)
	$(CXX) $(CPP_FLAGS) $(C_FLAGS) $(DEPEND_FLAGS) -fvisibility=hidden -o $@ $<

%.lbo: $(MAKEDIR)/%.lub
	@echo Compiling $< ...
	@echo $(LUBC) -I$(MAKEDIR) $(LUBC_FLAGS) -o $@ $< $(BLACKHOLE)
	$(LUBC) -I$(MAKEDIR) $(LUBC_FLAGS) -o $@ $<

%.res: $(MAKEDIR)/%.rc
	@echo Resourcing $< ...
ifneq "$(XDK_TARGET_FORMAT)" "elf"
	$(RES) $(RES_FLAGS) -o $@ -i $(subst \,/,$<)
else
	$(CC) $(C_DEFINES) -E -P -x c -Wall -o __$*.rc $<
	fromdos __$*.rc
	perl $(XDK_TOOLS)/version.pl "__Elastos_RC" "`pwd`" "$(XDK_TARGETS)" "__$*.rc"
	perl $(XDK_TOOLS)/cls_trans.pl __$*.rc $(dir $<)
	perl $(XDK_TOOLS)/res_trans.pl __$*.rc "rc" "$(SOURCES)"
	if [ -e "__section.cpp" ]; then \
		$(CC) $(C_DEFINES) -c -fno-builtin -o __section.o __section.cpp; \
	fi

	-mv __section.cpp __section0.cpp;
	$(CC) $(C_FLAGS) -o $@ __$*.cpp
endif

%.exp: $(MAKEDIR)/%.def
	@echo Exporting $< ...
ifneq "$(XDK_TARGET_FORMAT)" "elf"
	$(CC) $(C_DEFINES) -E -P -x c -Wall -o $*.def $<
	sed -i "s/LIBRARY.*/LIBRARY/" $*.def
	sed -i "s/DESCRIPTION.*//" $*.def
	$(DLLTOOL) $(DLLTOOL_FLAGS) -d $*.def -e $*.exp
	-$(CP) $(TARGET_NAME).exp $*.exp $(BLACKHOLE) 2>/dev/null 1>/dev/null
else
	$(CC) $(C_DEFINES) -E -P -x c -Wall -o __$*.def $<
ifeq "$(XDK_TARGET_FORMAT)" "elf"
ifneq "$(filter %.res,$(OBJECTS))" ""
	echo "g_pDllResource     DATA">> __$*.def
endif
endif
	perl $(XDK_TOOLS)/def_trans.pl __$*.def
ifeq "$(TARGET_TYPE)" "eco"
	perl $(XDK_TOOLS)/res_trans.pl __$*.def "def" "$(SOURCES)"
	if [ -e "__section.cpp" ]; then \
		$(CXX) $(CPP_FLAGS) $(C_FLAGS) -o __section.o __section.cpp; \
		$(MV) __section.cpp __section0.cpp; \
	fi
endif
	if [ -e "__dllmain.cpp" ]; then \
		$(CC) $(C_DEFINES) -c -fno-builtin -o __dllmain.o __dllmain.cpp; \
	fi
	$(CC) $(C_DEFINES) -c -fno-builtin -o $*.def __$*_exp.c
#	$(CC) $(C_DEFINES) -fPIC -c -fno-builtin -o __$*_dllmain.o __$*_dllmain.c
#	$(CC) $(C_DEFINES) -fPIC -c -fno-builtin -o $*.def __$*_exp.c
	touch $*.exp
endif
	touch $(TARGET_NAME).lib

%.exp: %.def
	@echo Exporting $< ...
ifneq "$(XDK_TARGET_FORMAT)" "elf"
	$(CC) $(C_DEFINES) -E -P -x c -Wall -o __$*.def $<
	-$(CP) __$*.def $*.def $(BLACKHOLE) 2>/dev/null 1>/dev/null
	sed -i "s/LIBRARY.*/LIBRARY/" $*.def
	sed -i "s/DESCRIPTION.*//" $*.def
	$(DLLTOOL) $(DLLTOOL_FLAGS) -d $*.def -e $*.exp
	-$(CP) $(TARGET_NAME).exp $*.exp $(BLACKHOLE) 2>/dev/null 1>/dev/null
else
	$(CC) $(C_DEFINES) -E -P -x c -Wall -o __$*.def $<
ifeq "$(XDK_TARGET_FORMAT)" "elf"
ifneq "$(filter %.res,$(OBJECTS))" ""
	echo "g_pDllResource     DATA">> __$*.def
endif
endif
ifeq "$(TARGET_TYPE)" "eco"
	perl $(XDK_TOOLS)/res_trans.pl __$*.def "def" "$(SOURCES)"
	if [ -e "__section.cpp" ]; then \
		$(CXX) $(CPP_FLAGS) $(C_FLAGS) -o __section.o __section.cpp; \
		$(MV) __section.cpp __section0.cpp; \
	fi
endif
	$(CC) $(C_DEFINES) -c -fno-builtin -o __dllmain.o __dllmain.cpp
	$(CC) $(C_DEFINES) -c -fno-builtin -o $*.def __$*_exp.c
	touch $*.exp
endif
	touch $(TARGET_NAME).lib

%.rsc: $(MAKEDIR)/%.car
	@echo Compiling $< ...
	$(Q)if [ -f $*.xml ]; then \
		$(RM) $*.xml; \
	fi

ifeq "$(XDK_TARGET_FORMAT)" "elf"
	$(CP) $*.cls $(XDK_TARGETS) $(BLACKHOLE)
endif

ifeq "$(TARGET_TYPE)" "eco"
	@echo Compiling $*.rc ...
	echo 1 ClassInfo $*.cls > $*.rc
	if [ -d $(MAKEDIR)/$*.rc ]; then \
         cat $(MAKEDIR)/$*.rc >> $*.rc; \
    fi

	$(CC) $(C_DEFINES) -E -P -x c -Wall -o __$*.rc $*.rc
	fromdos __$*.rc
	perl $(XDK_TOOLS)/version.pl "__Elastos_CAR" "`pwd`" "$(XDK_TARGETS)" "$<" "$(XDK_RUNTIME_PLATFORM)"
	perl $(XDK_TOOLS)/cls_trans.pl __$*.rc 'NA'
	perl $(XDK_TOOLS)/res_trans.pl __$*.rc "rc" "$(SOURCES)"
	if [ -e "__section.cpp" ]; then \
		$(CXX) $(CPP_FLAGS) $(C_FLAGS) -o __section.o __section.cpp; \
		$(MV) __section.cpp __section0.cpp; \
	fi
	if [ -e "__dllmain.cpp" ]; then \
		$(CXX) $(CPP_FLAGS) $(C_FLAGS) -o __dllmain.o __dllmain.cpp; \
		$(MV) __dllmain.cpp __dllmain0.cpp; \
	fi
	$(CC) $(C_FLAGS) -o $@ __$*.cpp
endif
	touch $@

%.mk: $(MAKEDIR)/%.car
		$(CAR) $(CAR_FLAGS) -a -c $*.cls -E $*Ex.cls $<
		@echo Generating H, CPP files from $*.cls ...
		$(LUBE) $(LUBE_FLAGS) -C$*Ex.cls -f -T header -T serverh -T cls2abrg \
				-T background $(LUBE_TS)
		$(CAR) $(CAR_FLAGS) $(DEPEND_CAR_FLAGS) $< >$*.d
		@echo >>$*.d
		@touch $@

$(EXPORT_HEADERS) : $(patsubst %,$(MAKEDIR)/%,$(EXPORT_HEADERS))
	@echo Copying $@ ...
	$(MKDIR) $(dir $(XDK_USER_INC)/$@)
	$(CP) $(MAKEDIR)/$@ $(XDK_USER_INC)/$@ $(BLACKHOLE)
	$(MKDIR) $(dir $@)
	$(CP) $(MAKEDIR)/$@ $@

%.idx: $(MAKEDIR)/%.idx
	@echo Copying $< ...
	$(CP) $< $(TARGET_OBJ_PATH) $(BLACKHOLE)
	perl $(XDK_TOOLS)/private/htm2chm.pl -o $< $(TARGET_NAME)
	touch $(@F)

%.htm: $(MAKEDIR)/%.htm
	@echo Copying $< ...
	$(CP) $< $(TARGET_OBJ_PATH) $(BLACKHOLE)
	touch $(@F)

%.cfg: $(MAKEDIR)/%.cfg
	@echo Copying $< ...
	$(CP) $< $(XDK_TARGETS) $(BLACKHOLE)
	$(CP) $< . $(BLACKHOLE)

##########################################################################
#
# Final targets
#
ifneq "$(TARGET_TYPE)" ""
$(TARGET_NAME).ecx: $(OBJECTS) $(filter-out -l%,$(LIBRARIES)) $(ELASTOS_LIBS) $(MAKEDIR)/sources
	@echo Linking $(TARGET_NAME) ...
# if ELF(for linux)
ifneq "$(USE_STDLIB)" ""
	$(CC) $(SEARCH_LIB) $(LINK_FLAGS) -o  $(XDK_TARGETS)/$(TARGET_NAME) \
		--start-group $(OBJECTS) $(LIBRARIES) $(ELASTOS_LIBS) --end-group $(ELF_FLAGS)
ifeq "$(DEBUG_INFO)" "1"
	$(CP) $(XDK_TARGETS)/$(TARGET_NAME) $(TARGET_DBG_INFO_PATH)
endif
else
ifeq "$(XDK_TARGET_PLATFORM)" "win32"
	$(LD) $(ECX_FLAGS) $(LINK_FLAGS) $(PASS2LD)-Map $(PASS2LD)$(TARGET_NAME).map\
		$(SEARCH_LIB) -o $(XDK_TARGETS)/$(TARGET_NAME) $(PASS2LD)--start-group $(OBJECTS) $(LIBRARIES) $(ELASTOS_LIBS) $(PASS2LD)--end-group
else
	perl $(XDK_TOOLS)/res_trans.pl $(TARGET_NAME) "ecx" "$(SOURCES)"
	if [ -e "__section.cpp" ]; then \
		$(CC) $(C_DEFINES) -c -fno-builtin -o __section.o __section.cpp; \
	fi
	-mv __section.cpp __section0.cpp;
	@echo $(LD) $(ECX_CRT_BEGIN) $(ECX_FLAGS) $(LINK_FLAGS) \
		$(SEARCH_LIB) -o $(XDK_TARGETS)/$(TARGET_NAME) $(PASS2LD)--strip-all \
		$(PASS2LD)--start-group $(OBJECTS) $(RESSECTION) $(LIBRARIES) $(ELASTOS_LIBS) $(PASS2LD)--end-group $(ECX_CRT_END) $(BLACKHOLE)
	$(LD) $(ECX_CRT_BEGIN) $(ECX_FLAGS) $(LINK_FLAGS) \
		$(SEARCH_LIB) -o $(XDK_TARGETS)/$(TARGET_NAME) $(PASS2LD)--strip-all \
		$(PASS2LD)--start-group $(OBJECTS) $(RESSECTION) $(LIBRARIES) $(ELASTOS_LIBS) $(PASS2LD)--end-group $(ECX_CRT_END)

ifeq "$(DEBUG_INFO)" "1"
	@echo $(LD) $(ECX_CRT_BEGIN) $(ECX_FLAGS) $(LINK_FLAGS) \
		$(SEARCH_LIB) -o $(TARGET_DBG_INFO_PATH)/$(TARGET_NAME) \
		$(PASS2LD)--start-group $(OBJECTS) $(LIBRARIES) $(RESSECTION) $(ELASTOS_LIBS) $(PASS2LD)--end-group $(ECX_CRT_END) $(BLACKHOLE)
	$(LD) $(ECX_CRT_BEGIN) $(ECX_FLAGS) $(LINK_FLAGS) \
		$(SEARCH_LIB) -o $(TARGET_DBG_INFO_PATH)/$(TARGET_NAME) \
		$(PASS2LD)--start-group $(OBJECTS) $(LIBRARIES) $(RESSECTION) $(ELASTOS_LIBS) $(PASS2LD)--end-group $(ECX_CRT_END)

endif
endif
endif
# endif (ELF PE)
	touch $@

$(TARGET_NAME).exe: $(OBJECTS) $(filter-out -l%,$(LIBRARIES)) $(ELASTOS_LIBS) $(MAKEDIR)/sources
	@echo Linking $(TARGET_NAME)...
# if ELF(for linux)
ifneq "$(USE_STDLIB)" ""
	$(CC) $(SEARCH_LIB) $(LINK_FLAGS) -o  $(XDK_TARGETS)/$(TARGET_NAME) \
		--start-group $(OBJECTS) $(LIBRARIES) $(ELASTOS_LIBS) --end-group $(ELF_FLAGS)
ifeq "$(DEBUG_INFO)" "1"
	$(CP) $(XDK_TARGETS)/$(TARGET_NAME) $(TARGET_DBG_INFO_PATH)
endif
else
ifeq "$(XDK_TARGET_PLATFORM)" "win32"
	$(LD) $(EXE_FLAGS) $(LINK_FLAGS) $(PASS2LD)-Map $(PASS2LD)$(TARGET_NAME).map\
		$(SEARCH_LIB) -o $(XDK_TARGETS)/$(TARGET_NAME) $(PASS2LD)--start-group $(OBJECTS) $(LIBRARIES) $(ELASTOS_LIBS) $(PASS2LD)--end-group
else
	perl $(XDK_TOOLS)/res_trans.pl $(TARGET_NAME) "exe" "$(SOURCES)"
	if [ -e "__section.cpp" ]; then \
		$(CC) $(C_DEFINES) -c -fno-builtin -o __section.o __section.cpp; \
	fi
	-mv __section.cpp __section0.cpp;
	@echo $(LD) $(EXE_CRT_BEGIN) $(EXE_FLAGS) $(LINK_FLAGS) \
		$(SEARCH_LIB) -o $(XDK_TARGETS)/$(TARGET_NAME) $(PASS2LD)--strip-all \
		$(PASS2LD)--start-group $(OBJECTS) $(RESSECTION) $(LIBRARIES) $(ELASTOS_LIBS) $(PASS2LD)--end-group $(EXE_CRT_END) $(BLACKHOLE)
	$(LD) $(EXE_CRT_BEGIN) $(EXE_FLAGS) $(LINK_FLAGS) \
		$(SEARCH_LIB) -o $(XDK_TARGETS)/$(TARGET_NAME) $(PASS2LD)--strip-all \
		$(PASS2LD)--start-group $(OBJECTS) $(RESSECTION) $(LIBRARIES) $(ELASTOS_LIBS) $(PASS2LD)--end-group $(EXE_CRT_END)

ifeq "$(DEBUG_INFO)" "1"
	@echo $(LD) $(EXE_CRT_BEGIN) $(EXE_FLAGS) $(LINK_FLAGS) \
		$(SEARCH_LIB) -o $(TARGET_DBG_INFO_PATH)/$(TARGET_NAME) \
		$(PASS2LD)--start-group $(OBJECTS) $(LIBRARIES) $(RESSECTION) $(ELASTOS_LIBS) $(PASS2LD)--end-group $(EXE_CRT_END) $(BLACKHOLE)
	$(LD) $(EXE_CRT_BEGIN) $(EXE_FLAGS) $(LINK_FLAGS) \
		$(SEARCH_LIB) -o $(TARGET_DBG_INFO_PATH)/$(TARGET_NAME) \
		$(PASS2LD)--start-group $(OBJECTS) $(LIBRARIES) $(RESSECTION) $(ELASTOS_LIBS) $(PASS2LD)--end-group $(EXE_CRT_END)

endif
endif
endif
# endif (ELF PE)
	touch $@

$(TARGET_NAME).so: $(OBJECTS) $(filter-out -l%,$(LIBRARIES))
	@echo Linking $@ ...
	$(LD) $(LINK_FLAGS) -m32 -shared -fpic -o  $(XDK_TARGETS)/$@ \
		$(PASS2LD)--start-group $(OBJECTS) $(LIBRARIES) $(PASS2LD)--end-group
ifeq "$(DEBUG_INFO)" "1"
	$(CP) $(XDK_TARGETS)/$@ $(TARGET_DBG_INFO_PATH)
endif
	touch $@
ifneq "$(EXPORT_LIBRARY)" ""
	$(CP) $(XDK_TARGETS)/$@ $(XDK_USER_LIB) $(BLACKHOLE)
	touch $(XDK_USER_LIB)/$@
endif
endif

$(TARGET_NAME).eco: $(RESOURCES_OBJECTS) $(OBJECTS) $(filter-out -l%,$(LIBRARIES)) $(ELASTOS_LIBS) $(MAKEDIR)/sources
	@echo Linking $@ ...
# else PE(for win32 .zener. linux)
ifneq "$(filter %.car,$(SOURCES))" ""
ifneq "$(TARGET_NAME).car" "$(filter %.car,$(SOURCES))"
	$(error The target Eco's Name must be the same as Car Filename)
endif
endif

ifeq "$(DEBUG_INFO)" "1"
ifneq "$(EXPORT_ALL_SYMBOLS)" ""
	@echo $(LD) $(DLL_FLAGS) $(DLLTOOL_FLAGS) $(DLL_CRT_BEGIN) $(LINK_FLAGS) $(PASS2LD)-Map $(PASS2LD)$(TARGET_NAME).map $(SEARCH_LIB) \
		-o $(TARGET_DBG_INFO_PATH)/$@ \
		$(PASS2LD)--start-group $(OBJECTS:exp=def) $(PASS2LD)--whole-archive $(ELASTOS_LIBS) $(PASS2LD)--no-whole-archive $(LIBRARIES) \
		$(RESSECTION) __dllmain.o $(DLL_ENTRY_OBJECT_FILE) $(PASS2LD)--end-group $(DLL_CRT_END) $(BLACKHOLE)
	$(LD) $(DLL_FLAGS) $(DLLTOOL_FLAGS) $(DLL_CRT_BEGIN) $(LINK_FLAGS) $(PASS2LD)-Map $(PASS2LD)$(TARGET_NAME).map $(SEARCH_LIB) \
		-o $(TARGET_DBG_INFO_PATH)/$@ \
		$(PASS2LD)--start-group $(OBJECTS:exp=def) $(PASS2LD)--whole-archive $(ELASTOS_LIBS) $(PASS2LD)--no-whole-archive $(LIBRARIES) \
		$(RESSECTION) __dllmain.o $(DLL_ENTRY_OBJECT_FILE) $(PASS2LD)--end-group $(DLL_CRT_END)
	$(STRIP) --strip-all -o $(XDK_TARGETS)/$@ $(TARGET_DBG_INFO_PATH)/$@
	rm -f $(TARGET_DBG_INFO_PATH)/$@
	$(LD) $(DLL_DBGINFO_FLAGS) $(DLLTOOL_FLAGS) $(DLL_CRT_BEGIN) $(LINK_FLAGS) $(PASS2LD)-Map $(PASS2LD)$(TARGET_NAME).map $(SEARCH_LIB) \
		-o $(TARGET_DBG_INFO_PATH)/$@ \
		$(PASS2LD)--start-group $(OBJECTS:exp=def) $(PASS2LD)--whole-archive $(ELASTOS_LIBS) $(PASS2LD)--no-whole-archive $(LIBRARIES) \
		$(RESSECTION) __dllmain.o $(DLL_ENTRY_OBJECT_FILE) $(PASS2LD)--end-group $(DLL_CRT_END)
else
	$(LD) $(DLL_FLAGS) $(DLLTOOL_FLAGS) $(DLL_CRT_BEGIN) $(LINK_FLAGS) $(PASS2LD)-Map $(PASS2LD)$(TARGET_NAME).map $(SEARCH_LIB) \
		-o $(TARGET_DBG_INFO_PATH)/$@ \
		$(PASS2LD)--start-group $(OBJECTS:exp=def) $(PASS2LD)--whole-archive $(ELASTOS_LIBS) $(PASS2LD)--no-whole-archive $(LIBRARIES) \
		$(RESSECTION) __dllmain.o $(DLL_ENTRY_OBJECT_FILE) $(PASS2LD)--end-group $(DLL_CRT_END)
	$(STRIP) --strip-all -o $(XDK_TARGETS)/$@ $(TARGET_DBG_INFO_PATH)/$@
endif
else
	@echo $(LD) $(DLL_FLAGS) $(DLLTOOL_FLAGS) $(DLL_CRT_BEGIN) $(LINK_FLAGS) $(PASS2LD)-Map $(PASS2LD)$(TARGET_NAME).map $(SEARCH_LIB) -o  $(XDK_TARGETS)/$@ \
		$(PASS2LD)--strip-all $(PASS2LD)--start-group $(OBJECTS:exp=def) $(PASS2LD)--whole-archive $(ELASTOS_LIBS) $(PASS2LD)--no-whole-archive $(LIBRARIES) \
		$(RESSECTION) __dllmain.o $(DLL_ENTRY_OBJECT_FILE) $(PASS2LD)--end-group $(DLL_CRT_END) $(BLACKHOLE)
	$(LD) $(DLL_FLAGS) $(DLLTOOL_FLAGS) $(DLL_CRT_BEGIN) $(LINK_FLAGS) $(PASS2LD)-Map $(PASS2LD)$(TARGET_NAME).map $(SEARCH_LIB) -o  $(XDK_TARGETS)/$@ \
		$(PASS2LD)--strip-all $(PASS2LD)--start-group $(OBJECTS:exp=def) $(PASS2LD)--whole-archive $(ELASTOS_LIBS) $(PASS2LD)--no-whole-archive $(LIBRARIES) \
		$(RESSECTION) __dllmain.o $(DLL_ENTRY_OBJECT_FILE) $(PASS2LD)--end-group $(DLL_CRT_END)
endif

ifneq "$(APPPACK)" ""
	if [ ! -d  $(XDK_TARGETS)/$(TARGET_NAME) ]; then $(MKDIR) $(XDK_TARGETS)/$(TARGET_NAME); fi
ifneq "$(FILERESOURCES)" ""
	$(foreach resources_file, $(FILERESOURCES), $(CP) $(resources_file) $(XDK_TARGETS)/$(TARGET_NAME); )
endif
ifneq "$(DIRRESOURCES)" ""
	$(foreach resources_dir, $(DIRRESOURCES), $(CPDIR) $(resources_dir) $(XDK_TARGETS)/$(TARGET_NAME);$(RMDIR) $(XDK_TARGETS)/$(TARGET_NAME)/$(subst $(dir $(resources_dir)),,$(resources_dir))/.svn;)
endif

ifneq "$(LIBRESOURCES)" ""
ifeq "$(TARGET_CPU_ABI)" ""
	$(error please check if set value for variable TARGET_CPU_ABI)
endif
	if [ ! -d  $(XDK_TARGETS)/$(TARGET_NAME)/lib ]; then $(MKDIR) $(XDK_TARGETS)/$(TARGET_NAME)/lib; fi
	if [ ! -d  $(XDK_TARGETS)/$(TARGET_NAME)/lib/$(TARGET_CPU_ABI) ]; then $(MKDIR) $(XDK_TARGETS)/$(TARGET_NAME)/lib/$(TARGET_CPU_ABI); fi
	$(foreach resources_lib, $(LIBRESOURCES), $(CP) $(resources_lib) $(XDK_TARGETS)/$(TARGET_NAME)/lib/$(TARGET_CPU_ABI); )
endif
	$(CP) $(XDK_TARGETS)/$@ $(XDK_TARGETS)/$(TARGET_NAME)
	$(RMDIR) `find $(XDK_TARGETS)/$(TARGET_NAME) -name *.svn`
	if [ -d $(XDK_TARGETS)/$(TARGET_NAME)/assets ]; then \
	    if [ -d $(XDK_TARGETS)/$(TARGET_NAME)/res ]; then \
			aapt package -f $(AAPT_FLAGS) -S $(XDK_TARGETS)/$(TARGET_NAME)/res -A $(XDK_TARGETS)/$(TARGET_NAME)/assets -I $(EMULATOR_PATH)/platforms/android-21/android.jar -M $(MAKEDIR)/AndroidManifest.xml --min-sdk-version 21 --target-sdk-version 21 -F $(XDK_TARGETS)/$(TARGET_NAME).epk; \
		else \
			aapt package -f $(AAPT_FLAGS) -A $(XDK_TARGETS)/$(TARGET_NAME)/assets -I $(EMULATOR_PATH)/platforms/android-21/android.jar -M $(MAKEDIR)/AndroidManifest.xml --min-sdk-version 21 --target-sdk-version 21 -F $(XDK_TARGETS)/$(TARGET_NAME).epk; \
		fi \
	else \
	    if [ -d $(XDK_TARGETS)/$(TARGET_NAME)/res ]; then \
			aapt package -f $(AAPT_FLAGS) -S $(XDK_TARGETS)/$(TARGET_NAME)/res -I $(EMULATOR_PATH)/platforms/android-21/android.jar -M $(MAKEDIR)/AndroidManifest.xml --min-sdk-version 21 --target-sdk-version 21 -F $(XDK_TARGETS)/$(TARGET_NAME).epk; \
		else \
			aapt package -f $(AAPT_FLAGS) -I $(EMULATOR_PATH)/platforms/android-21/android.jar -M $(MAKEDIR)/AndroidManifest.xml --min-sdk-version 21 --target-sdk-version 21 -F $(XDK_TARGETS)/$(TARGET_NAME).epk; \
		fi \
	fi
	cd $(XDK_TARGETS)/$(TARGET_NAME);zip -r9D $(XDK_TARGETS)/$(TARGET_NAME).epk $@ $(BLACKHOLE)
	if [ -d $(XDK_TARGETS)/$(TARGET_NAME)/lib ]; then cd $(XDK_TARGETS)/$(TARGET_NAME);zip -r9D $(XDK_TARGETS)/$(TARGET_NAME).epk lib $(BLACKHOLE); fi
	if [ -d $(XDK_TARGETS)/$(TARGET_NAME)/res ]; then rm -f $(MAKEDIR)/R.cpp; fi
endif

	if [ -f $(XDK_USER_LIB)/$(TARGET_NAME).lib ]; then  \
	    touch $(XDK_USER_LIB)/$(TARGET_NAME).lib; \
	fi
	touch $@

$(TARGET_NAME).lib: $(OBJECTS) $(ELASTOS_LIBS) $(LIBRARIES) $(MAKEDIR)/sources
	@echo Packing $@ ...
	-rm $@ $(BLACKHOLE) 2>/dev/null 1>/dev/null
#  It's for split the lib to .o file
ifneq "$(LIBRARIES)" ""
	$(foreach aaa, $(filter %.o, $(LIBRARIES)), $(CP) $(aaa) . ;)
endif
ifneq "$(LIBRARIES)" ""
	$(foreach aaa, $(LIBRARIES), $(AR) x $(aaa) ;)
endif
ifneq "$(ELASTOS_LIBS)" ""
	$(foreach aaa, $(filter %.o, $(ELASTOS_LIBS)), $(CP) $(aaa) . ;)
endif
ifneq "$(ELASTOS_LIBS)" ""
	$(foreach aaa, $(ELASTOS_LIBS), $(AR) x $(aaa) ;)
endif

	@echo $(AR) $(AR_FLAGS) $@ *.o $(BLACKHOLE)
	$(AR) $(AR_FLAGS) $@ *.o
ifneq "$(EXPORT_LIBRARY)" ""
	$(CP) $@ $(XDK_USER_LIB) $(BLACKHOLE)
	touch $(XDK_USER_LIB)/$@
endif

##########################################################################
#
# For depend
#
depend.mk:
	if [ -f depend.mk ]; then \
		$(RM) depend.mk; \
	fi
	touch $@
ifeq "$(NODEPEND)" ""
ifneq "$(DEPEND_SOURCES)" ""
	$(foreach aaa, $(DEPEND_SOURCES), echo "-include $(aaa)" >>depend.mk; )
endif
endif
	touch $@
