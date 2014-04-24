CPPFLAGS = /nologo /GR- /MD /O2y- /Zi
LFLAGS = /nologo /DEBUG /INCREMENTAL:NO /MANIFEST:NO /RELEASE

all: dep_test aslr_test sehop_test

dep_test: dep_test_nxcompat.exe dep_test_nocompat.exe dep_test_incompat.exe
dep_test_incompat.exe: dep_test.obj
	link $** $(LFLAGS) /OUT:$(@F) /NXCOMPAT:NO /ENTRY:NXViolationStartup
dep_test_nocompat.exe: dep_test.obj
	link $** $(LFLAGS) /OUT:$(@F) /NXCOMPAT:NO
dep_test_nxcompat.exe: dep_test.obj
	link $** $(LFLAGS) /OUT:$(@F) /NXCOMPAT

sehop_test: sehop_test_nt60.exe sehop_test_nt62.exe
sehop_test_nt60.exe: sehop_test.obj
	link $** $(LFLAGS) /OUT:$(@F) /SUBSYSTEM:CONSOLE,6.0
sehop_test_nt62.exe: sehop_test.obj
	link $** $(LFLAGS) /OUT:$(@F) /SUBSYSTEM:CONSOLE,6.2

aslr_test: A.dll B.dll C.dll D.dll E.dll F.dll aslr_test_fixed.exe aslr_test_reloc.exe aslr_test_dynamic.exe
aslr_test_fixed.exe: aslr_test.obj
	link $** $(LFLAGS) /OUT:$(@F) /FIXED
aslr_test_reloc.exe: aslr_test.obj
	link $** $(LFLAGS) /OUT:$(@F) /DYNAMICBASE:NO /FIXED:NO
aslr_test_dynamic.exe: aslr_test.obj
	link $** $(LFLAGS) /OUT:$(@F) /DYNAMICBASE
A.dll: dllmain.obj
	link /nologo $** /DLL /OUT:$(@F) /DYNAMICBASE /BASE:0x20000000
B.dll: dllmain.obj
	link /nologo $** /DLL /OUT:$(@F) /DYNAMICBASE /BASE:0x22000000
C.dll: dllmain.obj
	link /nologo $** /DLL /OUT:$(@F) /DYNAMICBASE:NO /BASE:0x24000000
D.dll: dllmain.obj
	link /nologo $** /DLL /OUT:$(@F) /DYNAMICBASE:NO /BASE:0x26000000
E.dll: dllmain.obj
	link /nologo $** /DLL /OUT:$(@F) /FIXED /BASE:0x28000000
F.dll: dllmain.obj
	link /nologo $** /DLL /OUT:$(@F) /FIXED /BASE:0x2A000000

clean:
	-del *.obj *.pdb *.cod *.ilk *.suo *.nativecodeanalysis.xml
	-del A.dll B.dll C.dll D.dll E.dll F.dll
	-del dep_test*.exe sehop_test*.exe aslr_test*.exe