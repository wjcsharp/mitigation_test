AFLAGS = /nologo /safeseh
CPPFLAGS = /nologo /GR- /MD /O2y- /Zi
LFLAGS = /nologo /DEBUG /INCREMENTAL:NO /MANIFEST:NO /PDBALTPATH:%_PDB% /RELEASE

all: dep_test aslr_test sehop_test

dep_test: dep_test_nxcompat.exe dep_test_nocompat.exe dep_test_incompat.exe

dep_test_incompat.exe: dep_test.obj
	link $** $(LFLAGS) /OUT:$(@F) /NXCOMPAT:NO /ENTRY:NXViolationStartup

dep_test_nocompat.exe: dep_test.obj
	link $** $(LFLAGS) /OUT:$(@F) /NXCOMPAT:NO

dep_test_nxcompat.exe: dep_test.obj
	link $** $(LFLAGS) /OUT:$(@F) /NXCOMPAT


sehop_test: sehop_test_nt60.exe sehop_test_nt62.exe

sehop_test_nt60.exe: sehop_test.obj safeseh.obj
	link $** $(LFLAGS) /OUT:$(@F) /SUBSYSTEM:CONSOLE,6.0

sehop_test_nt62.exe: sehop_test.obj safeseh.obj
	link $** $(LFLAGS) /OUT:$(@F) /SUBSYSTEM:CONSOLE,6.2


aslr_test: aslr_dll aslr_runtime aslr_loadtime
aslr_dll: DLL_aslraware.dll DLL_relocatable.dll DLL_fixed.dll
aslr_runtime: aslr_runtime_test_fixed.exe aslr_runtime_test_reloc.exe aslr_runtime_test_dynamic.exe
aslr_loadtime: aslr_loadtime_test_fixed.exe aslr_loadtime_test_reloc.exe aslr_loadtime_test_dynamic.exe

aslr_runtime_test_fixed.exe: aslr_runtime_test.obj aslr_impl.obj
	link $** $(LFLAGS) /OUT:$(@F) /FIXED

aslr_runtime_test_reloc.exe: aslr_runtime_test.obj aslr_impl.obj
	link $** $(LFLAGS) /OUT:$(@F) /DYNAMICBASE:NO /FIXED:NO

aslr_runtime_test_dynamic.exe: aslr_runtime_test.obj aslr_impl.obj
	link $** $(LFLAGS) /OUT:$(@F) /DYNAMICBASE

aslr_loadtime_test_fixed.exe: aslr_loadtime_test.obj aslr_impl.obj
	link $** *.lib $(LFLAGS) /OUT:$(@F) /FIXED

aslr_loadtime_test_reloc.exe: aslr_loadtime_test.obj aslr_impl.obj
	link $** *.lib $(LFLAGS) /OUT:$(@F) /DYNAMICBASE:NO /FIXED:NO

aslr_loadtime_test_dynamic.exe: aslr_loadtime_test.obj aslr_impl.obj
	link $** *.lib $(LFLAGS) /OUT:$(@F) /DYNAMICBASE

DLL_aslraware.dll: DLL_aslraware.obj dllmain.obj
	link /nologo $** /DLL /OUT:$(@F) /DYNAMICBASE /BASE:0x20000000

DLL_relocatable.dll: DLL_relocatable.obj dllmain.obj
	link /nologo $** /DLL /OUT:$(@F) /DYNAMICBASE:NO /BASE:0x30000000

DLL_fixed.dll: DLL_fixed.obj dllmain.obj
	link /nologo $** /DLL /OUT:$(@F) /FIXED /BASE:0x40000000


clean:
	-del *.obj *.pdb *.cod *.ilk *.suo *.nativecodeanalysis.xml
	-del dep_test*.exe sehop_test*.exe aslr_*test*.exe
	-del DLL_*.dll DLL_*.lib DLL_*.exp