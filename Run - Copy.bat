TASKKILL /IM java*
TASKKILL /IM matlab.exe /f
TASKKILL /IM BodyBasics-D2D.exe /f
TASKKILL /IM python*
TASKKILL /IM i2p_reactive_layer.exe
TASKKILL /IM i2p_Nadine_Control.exe

C:

cd C:\Users\IMI-Demo\Desktop\i2p_nadine_control_V.1\build\Release

REM START i2p_Nadine_Control.exe -voicepath "C:\IMI_LIBRARIES\smartbody\data\cereproc\voices\cerevoice_heather_3.0.7_22k" -voicepathGerman "C:\IMI_LIBRARIES\smartbody\data\cereproc\voices\cerevoice_gudrun_3.0.6_22k" -animationXMLPath "C:\IMI-PROJECTS\development_Org\i2p_nadine_control\XMLAnimations" -feedbackServerIP "155.69.54.66"
START i2p_Nadine_Control.exe -voicepath "C:\IMI_LIBRARIES\smartbody\data\cereproc\voices\cerevoice_heather_3.0.7_22k" -animationXMLPath "C:\IMI-PROJECTS\development_Org\i2p_nadine_control\XMLAnimations" -feedbackServerIP "155.69.54.66"

REM cd C:\IMI-PROJECTS\development\i2p_reactive_layer_v1\build\Release
REM START i2p_reactive_layer.exe