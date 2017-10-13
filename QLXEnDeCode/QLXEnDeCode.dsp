# Microsoft Developer Studio Project File - Name="QLXEnDeCode" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SHL2DATAFEED - WIN32 RELEASE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "QLXEnDeCode.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "QLXEnDeCode.mak" CFG="SHL2DATAFEED - WIN32 RELEASE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "QLXEnDeCode - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "QLXEnDeCode - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "QLXEnDeCode - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DATASRCDRVSHL2FAST_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DATASRCDRVSHL2FAST_EXPORTS" /D "WINDOWSCODE" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"E:\runningEnv\SZL2Fast\VSSSource\tran_src_shl2\QLXEnDeCode.dll"

!ELSEIF  "$(CFG)" == "QLXEnDeCode - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DATASRCDRVSHL2FAST_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DATASRCDRVSHL2FAST_EXPORTS" /D "WINDOWSCODE" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"E:\runningEnv\SZL2Fast\VSSSource\tran_src_shl2\QLXEnDeCode.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "QLXEnDeCode - Win32 Release"
# Name "QLXEnDeCode - Win32 Debug"
# Begin Group "SrvUnit"

# PROP Default_Filter ""
# Begin Group "GlobalIO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SrvUnit\GlobalIO\MGlobalIO.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\GlobalIO\MGlobalIO.h
# End Source File
# End Group
# Begin Group "OptionIO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SrvUnit\OptionIO\MOptionIO.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\OptionIO\MOptionIO.h
# End Source File
# End Group
# Begin Group "ProcessIO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SrvUnit\ProcessIO\Process.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\ProcessIO\Process.h
# End Source File
# End Group
# Begin Group "DateCenter"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SrvUnit\DataCenter\DataCenter.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\DataCenter\DataCenter.h
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\DataCenter\HKBaseMemStructure.h
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\DataCenter\MemData.h
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\DataCenter\MemStructure.h
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\DataCenter\MQuickNameTableSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\DataCenter\MQuickNameTableSearch.h
# End Source File
# End Group
# Begin Group "PushIO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SrvUnit\PushIO\MSrvPushIO.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\PushIO\MSrvPushIO.h
# End Source File
# End Group
# Begin Group "DataIO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SrvUnit\DataIO\DataIO.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\DataIO\DataIO.h
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\DataIO\Encode.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\DataIO\Encode.h
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\DataIO\MDbfWrite.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\DataIO\MDbfWrite.h
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\DataIO\zipdll.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\DataIO\zipdll.h
# End Source File
# End Group
# Begin Group "templateparser"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SrvUnit\templateParser\FastBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\templateParser\FastBase.h
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\templateParser\fastdef.h
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\templateParser\MarkupSTL.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\templateParser\MarkupSTL.h
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\templateParser\templateReader.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\templateParser\templateReader.h
# End Source File
# End Group
# Begin Group "CommIO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SrvUnit\CommIO\ClientComm.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\CommIO\ClientComm.h
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\CommIO\CommRecorder.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\CommIO\CommRecorder.h
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\CommIO\ConvertStr.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\CommIO\ConvertStr.h
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\CommIO\FastDecoder.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\CommIO\FastDecoder.h
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\CommIO\QzGzFile.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\CommIO\QzGzFile.h
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\CommIO\StepClient.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\CommIO\StepClient.h
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\CommIO\UADecoder.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\CommIO\UADecoder.h
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\CommIO\VDEIO.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\CommIO\VDEIO.h
# End Source File
# End Group
# Begin Group " ControlIO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SrvUnit\ControlIO\MControlIO.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvUnit\ControlIO\MControlIO.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\QLXEnDeCode.cpp
# End Source File
# Begin Source File

SOURCE=.\QLXEnDeCode.def
# End Source File
# End Target
# End Project
