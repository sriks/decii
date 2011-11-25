/* This file is part of the Nokia Qt SDK

Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
All rights reserved.

Contact:  Nokia Corporation (qt-info@nokia.com)
 
GNU Lesser General Public License Usage
This file may be used under the terms of the GNU Lesser General Public
License version 2.1 as published by the Free Software Foundation and
appearing in the file LICENSE.LGPL included in the packaging of this file.
Please review the following information to ensure the GNU Lesser General
Public License version 2.1 requirements will be met:
http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.

If you have questions regarding the use of this file, please contact Nokia
at qt-info@nokia.com.
*/

function Component()
{
    if( component.fromOnlineRepository )
    {
	   // Contributions are from parent components
    }
}

Component.prototype.createOperations = function()
{
    // Call the base createOperations
    
    component.createOperations();
 
    print("---> Installing Symbian Belle base for Qt 4.7.4");
    
    var MCLQt474TargetDir = "@TargetDir@/Symbian/SDKs/SymbianSR1Qt474";
    
    component.addOperation( "Extract", "@TargetDir@/Symbian/cache/symbian_sr1.7z", MCLQt474TargetDir);
    component.addOperation( "Extract", "@TargetDir@/Symbian/cache/qt474_symbian.7z", MCLQt474TargetDir);	 
    component.addOperation( "Extract", "@TargetDir@/Symbian/cache/smartinstaller.7z", MCLQt474TargetDir);
    
    if (installer.value("os") == "win") {
        component.addOperation( "Extract", "@TargetDir@/Symbian/cache/qmake_path_patcher_sbsv2.7z", MCLQt474TargetDir);
        component.addOperation( "Extract", "@TargetDir@/Symbian/cache/symbian_sr1_win32_tools.7z", MCLQt474TargetDir);
    }

    if (installer.value("os") == "x11") {
        component.addOperation( "Extract", "@TargetDir@/Symbian/cache/qt474_symbian_linux.7z", MCLQt474TargetDir);
        component.addOperation( "Extract", "@TargetDir@/Symbian/cache/epoc32_linux.7z", MCLQt474TargetDir);
        component.addOperation( "Extract", "@TargetDir@/Symbian/cache/qmake_path_patcher_linux_sbsv2.7z", MCLQt474TargetDir);
    }	

    component.addOperation( "Extract", "@TargetDir@/Symbian/cache/symbian3_nativeapis.7z", MCLQt474TargetDir);
	
    if (installer.value("os") == "win") {
        component.addOperation( "Execute", "@TargetDir@/Symbian/SDKs/SymbianSR1Qt474/patch.qmake.paths.bat");
    }
    
    if (installer.value("os") == "x11") {
        component.addOperation( "Execute", "@TargetDir@/Symbian/SDKs/SymbianSR1Qt474/qmake_patch.sh");
    }
        
    component.addOperation( "RegisterQtInCreatorV23",
        "Qt 4.7.4 for Symbian Belle (@QtVersionLabel@)",
        MCLQt474TargetDir,
        "Symbian",
        MCLQt474TargetDir,
        MCLQt474TargetDir,
        "@TargetDir@/Symbian/tools/sbs/bin");

		if (installer.value("os") == "win") {		
		    component.addOperation( "CreateShortcut",
		        "@TargetDir@/Symbian/SDKs/SymbianSR1Qt474/bin/qtenvSBSv2.bat",
                        "@StartMenuDir@/Symbian Belle Qt 4.7.4/Qt 4.7.4 for Symbian Belle Command Prompt.lnk" );
		}

    component.registerPathForUninstallation( MCLQt474TargetDir, true );

	  var sis = (installer.value("os") == "win") && installer.componentByName("com.nokia.ndk.tools.symbian.sis").selected;
	  
    if (sis == true)
    {
    
        component.addOperation( "CreateShortcut",
                           "@TargetDir@/Symbian/sis/Symbian_Anna/Qt/4.7.4/QMLViewer-4.7.4-for-Anna-Belle.sis",
                           "@StartMenuDir@/Symbian Belle Qt 4.7.4/Install QML Viewer for Symbian Belle on device.lnk" ); //, "-rxvt" );
    
        component.addOperation( "CreateShortcut",
		                           "@TargetDir@/Symbian/sis/common/tools/NokiaEnergyProfiler-MultiLang-1.26.0-Anna-Belle.sis",
                               "@StartMenuDir@/Symbian Belle Qt 4.7.4/Install Nokia Energy Profiler for Symbian Belle on device.lnk" );
    
		    component.addOperation( "CreateShortcut",
		                           "@TargetDir@/Symbian/sis/common/tools/PerformanceMonitor-1.4.2-Anna-Belle.sis",
                               "@StartMenuDir@/Symbian Belle Qt 4.7.4/Install Nokia Performance Monitor for Symbian Belle on device.lnk" );
		    
        component.addOperation( "CreateShortcut",
		                           "@TargetDir@/Symbian/sis/common/CODA/Public-CODA-1.0.5-b2-for-S60v5-Anna-Belle.sis",
                               "@StartMenuDir@/Symbian Belle Qt 4.7.4/Install CODA (Debug Agent) on Symbian Belle device.lnk" ); //, "-rxvt" );
		}   

    print("<--- Installing Symbian Belle base for Qt 4.7.4");
}
