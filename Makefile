AFLAGS = /nologo /safeseh
CPPFLAGS = /nologo /GR- /MD /O2y- /Zi
LFLAGS = /nologo /DEBUG /INCREMENTAL:NO /MANIFEST:NO /PDBALTPATH:%_PDB% /RELEASE /SUBSYSTEM:CONSOLE,5.1

all: dep_test aslr_test sehop_test

!INCLUDE <Makefile_DEP>
!INCLUDE <Makefile_ASLR>
!INCLUDE <Makefile_SEHOP>

clean:
	-del *.obj *.pdb *.cod *.ilk *.suo *.nativecodeanalysis.xml
	-del dep_test*.exe sehop_test*.exe aslr_*test*.exe
	-del DLL_*.dll DLL_*.lib DLL_*.exp