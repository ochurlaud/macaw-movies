############################################################################################
#      NSIS Installation Script created by NSIS Quick Setup Script Generator v1.09.18
#               Entirely Edited with NullSoft Scriptable Installation System                
#              by Vlasis K. Barkas aka Red Wine red_wine@freemail.gr Sep 2006               
############################################################################################

!define APP_NAME "Macaw-Movies"
!define COMP_NAME "Macaw-Movies"
!define WEB_SITE "http://github.com/macaw-movies"
!define VERSION "0.1.0.0"
!define COPYRIGHT "Olivier CHURLAUD & Sébastien TOUZÉ © 2015"
!define DESCRIPTION "Application"
!define LICENSE_TXT "..\COPYING"
!define INSTALLER_NAME "..\..\Macaw_setup.exe"
!define MAIN_APP_EXE "Macaw-Movies.exe"
!define INSTALL_TYPE "SetShellVarContext all"
!define REG_ROOT "HKLM"
!define REG_APP_PATH "Software\Microsoft\Windows\CurrentVersion\App Paths\${MAIN_APP_EXE}"
!define UNINSTALL_PATH "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"

!define REG_START_MENU "Start Menu Folder"

var SM_Folder

######################################################################

VIProductVersion  "${VERSION}"
VIAddVersionKey "ProductName"  "${APP_NAME}"
VIAddVersionKey "CompanyName"  "${COMP_NAME}"
VIAddVersionKey "LegalCopyright"  "${COPYRIGHT}"
VIAddVersionKey "FileDescription"  "${DESCRIPTION}"
VIAddVersionKey "FileVersion"  "${VERSION}"

######################################################################

SetCompressor ZLIB
Name "${APP_NAME}"
Caption "${APP_NAME}"
OutFile "${INSTALLER_NAME}"
BrandingText "${APP_NAME}"
XPStyle off
InstallDirRegKey "${REG_ROOT}" "${REG_APP_PATH}" ""
InstallDir "$PROGRAMFILES\Macaw-Movies"

######################################################################

!include "MUI.nsh"

!define MUI_ABORTWARNING
!define MUI_UNABORTWARNING

!define MUI_LANGDLL_REGISTRY_ROOT "${REG_ROOT}"
!define MUI_LANGDLL_REGISTRY_KEY "${UNINSTALL_PATH}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

!insertmacro MUI_PAGE_WELCOME

!ifdef LICENSE_TXT
!insertmacro MUI_PAGE_LICENSE "${LICENSE_TXT}"
!endif

!ifdef REG_START_MENU
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "Macaw-Movies"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${REG_ROOT}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${UNINSTALL_PATH}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${REG_START_MENU}"
!insertmacro MUI_PAGE_STARTMENU Application $SM_Folder
!endif

!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_RUN "$INSTDIR\${MAIN_APP_EXE}"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM

!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"

!insertmacro MUI_RESERVEFILE_LANGDLL

######################################################################

Function .onInit
!insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

######################################################################

Section -MainProgram
${INSTALL_TYPE}
SetOverwrite ifnewer
SetOutPath "$INSTDIR"
File "..\build-win\icudt53.dll"
File "..\build-win\icuin53.dll"
File "..\build-win\icuuc53.dll"
File "..\build-win\Macaw-Movies.exe"
File "..\build-win\Qt5Core.dll"
File "..\build-win\Qt5Gui.dll"
File "..\build-win\Qt5Network.dll"
File "..\build-win\Qt5Sql.dll"
File "..\build-win\Qt5Svg.dll"
File "..\build-win\Qt5Widgets.dll"
File "..\build-win\qt_ca.qm"
File "..\build-win\qt_cs.qm"
File "..\build-win\qt_de.qm"
File "..\build-win\qt_fi.qm"
File "..\build-win\qt_hu.qm"
File "..\build-win\qt_it.qm"
File "..\build-win\qt_ja.qm"
File "..\build-win\qt_ru.qm"
File "..\build-win\qt_sk.qm"
File "..\build-win\qt_uk.qm"
SetOutPath "$INSTDIR\sqldrivers"
File "..\build-win\sqldrivers\qsqlite.dll"
File "..\build-win\sqldrivers\qsqlmysql.dll"
File "..\build-win\sqldrivers\qsqlodbc.dll"
File "..\build-win\sqldrivers\qsqlpsql.dll"
SetOutPath "$INSTDIR\platforms"
File "..\build-win\platforms\qwindows.dll"
SetOutPath "$INSTDIR\imageformats"
File "..\build-win\imageformats\qdds.dll"
File "..\build-win\imageformats\qgif.dll"
File "..\build-win\imageformats\qicns.dll"
File "..\build-win\imageformats\qico.dll"
File "..\build-win\imageformats\qjp2.dll"
File "..\build-win\imageformats\qjpeg.dll"
File "..\build-win\imageformats\qmng.dll"
File "..\build-win\imageformats\qsvg.dll"
File "..\build-win\imageformats\qtga.dll"
File "..\build-win\imageformats\qtiff.dll"
File "..\build-win\imageformats\qwbmp.dll"
File "..\build-win\imageformats\qwebp.dll"
SetOutPath "$INSTDIR\iconengines"
File "..\build-win\iconengines\qsvgicon.dll"
SetOutPath "$INSTDIR\bearer"
File "..\build-win\bearer\qgenericbearer.dll"
File "..\build-win\bearer\qnativewifibearer.dll"
SectionEnd

######################################################################

Section -Icons_Reg
SetOutPath "$INSTDIR"
WriteUninstaller "$INSTDIR\uninstall.exe"

!ifdef REG_START_MENU
!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
CreateDirectory "$SMPROGRAMS\$SM_Folder"
CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$SMPROGRAMS\$SM_Folder\Uninstall ${APP_NAME}.lnk" "$INSTDIR\uninstall.exe"

!ifdef WEB_SITE
WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
!endif
!insertmacro MUI_STARTMENU_WRITE_END
!endif

!ifndef REG_START_MENU
CreateDirectory "$SMPROGRAMS\Macaw-Movies"
CreateShortCut "$SMPROGRAMS\Macaw-Movies\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$SMPROGRAMS\Macaw-Movies\Uninstall ${APP_NAME}.lnk" "$INSTDIR\uninstall.exe"

!ifdef WEB_SITE
WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
CreateShortCut "$SMPROGRAMS\Macaw-Movies\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
!endif
!endif

WriteRegStr ${REG_ROOT} "${REG_APP_PATH}" "" "$INSTDIR\${MAIN_APP_EXE}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayName" "${APP_NAME}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "UninstallString" "$INSTDIR\uninstall.exe"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayIcon" "$INSTDIR\${MAIN_APP_EXE}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayVersion" "${VERSION}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "Publisher" "${COMP_NAME}"

!ifdef WEB_SITE
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "URLInfoAbout" "${WEB_SITE}"
!endif
SectionEnd

######################################################################

Section Uninstall
${INSTALL_TYPE}
Delete "$INSTDIR\icudt53.dll"
Delete "$INSTDIR\icuin53.dll"
Delete "$INSTDIR\icuuc53.dll"
Delete "$INSTDIR\Macaw-Movies.exe"
Delete "$INSTDIR\Qt5Core.dll"
Delete "$INSTDIR\Qt5Gui.dll"
Delete "$INSTDIR\Qt5Network.dll"
Delete "$INSTDIR\Qt5Sql.dll"
Delete "$INSTDIR\Qt5Svg.dll"
Delete "$INSTDIR\Qt5Widgets.dll"
Delete "$INSTDIR\qt_ca.qm"
Delete "$INSTDIR\qt_cs.qm"
Delete "$INSTDIR\qt_de.qm"
Delete "$INSTDIR\qt_fi.qm"
Delete "$INSTDIR\qt_hu.qm"
Delete "$INSTDIR\qt_it.qm"
Delete "$INSTDIR\qt_ja.qm"
Delete "$INSTDIR\qt_ru.qm"
Delete "$INSTDIR\qt_sk.qm"
Delete "$INSTDIR\qt_uk.qm"
Delete "$INSTDIR\sqldrivers\qsqlite.dll"
Delete "$INSTDIR\sqldrivers\qsqlmysql.dll"
Delete "$INSTDIR\sqldrivers\qsqlodbc.dll"
Delete "$INSTDIR\sqldrivers\qsqlpsql.dll"
Delete "$INSTDIR\platforms\qwindows.dll"
Delete "$INSTDIR\imageformats\qdds.dll"
Delete "$INSTDIR\imageformats\qgif.dll"
Delete "$INSTDIR\imageformats\qicns.dll"
Delete "$INSTDIR\imageformats\qico.dll"
Delete "$INSTDIR\imageformats\qjp2.dll"
Delete "$INSTDIR\imageformats\qjpeg.dll"
Delete "$INSTDIR\imageformats\qmng.dll"
Delete "$INSTDIR\imageformats\qsvg.dll"
Delete "$INSTDIR\imageformats\qtga.dll"
Delete "$INSTDIR\imageformats\qtiff.dll"
Delete "$INSTDIR\imageformats\qwbmp.dll"
Delete "$INSTDIR\imageformats\qwebp.dll"
Delete "$INSTDIR\iconengines\qsvgicon.dll"
Delete "$INSTDIR\bearer\qgenericbearer.dll"
Delete "$INSTDIR\bearer\qnativewifibearer.dll"
 
RmDir "$INSTDIR\bearer"
RmDir "$INSTDIR\iconengines"
RmDir "$INSTDIR\imageformats"
RmDir "$INSTDIR\platforms"
RmDir "$INSTDIR\sqldrivers"
 
Delete "$INSTDIR\uninstall.exe"
!ifdef WEB_SITE
Delete "$INSTDIR\${APP_NAME} website.url"
!endif

RmDir "$INSTDIR"

!ifdef REG_START_MENU
!insertmacro MUI_STARTMENU_GETFOLDER "Application" $SM_Folder
Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk"
Delete "$SMPROGRAMS\$SM_Folder\Uninstall ${APP_NAME}.lnk"
!ifdef WEB_SITE
Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk"
!endif
Delete "$DESKTOP\${APP_NAME}.lnk"

RmDir "$SMPROGRAMS\$SM_Folder"
!endif

!ifndef REG_START_MENU
Delete "$SMPROGRAMS\Macaw-Movies\${APP_NAME}.lnk"
Delete "$SMPROGRAMS\Macaw-Movies\Uninstall ${APP_NAME}.lnk"
!ifdef WEB_SITE
Delete "$SMPROGRAMS\Macaw-Movies\${APP_NAME} Website.lnk"
!endif
Delete "$DESKTOP\${APP_NAME}.lnk"

RmDir "$SMPROGRAMS\Macaw-Movies"
!endif

DeleteRegKey ${REG_ROOT} "${REG_APP_PATH}"
DeleteRegKey ${REG_ROOT} "${UNINSTALL_PATH}"
SectionEnd

######################################################################

Function un.onInit
!insertmacro MUI_UNGETLANGUAGE
FunctionEnd

######################################################################

