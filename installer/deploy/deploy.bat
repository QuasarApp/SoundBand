echo off

set deploy=%1
set qmldir=%2

SET CURRENTDIR=%3

echo on
RMDIR /S /Q "%CURRENTDIR%\packages\SoundBand\data"
RMDIR /S /Q "%CURRENTDIR%\packages\SoundBand.tests\data"

MKDIR "%CURRENTDIR%\packages\SoundBand\data"
MKDIR "%CURRENTDIR%\packages\SoundBand.tests\data"

xcopy \d \y \r \f "%CURRENTDIR%packages\SoundBand\extra" "%CURRENTDIR%\packages\SoundBand\data"


copy /Y "%CURRENTDIR%\..\SoundBand\build\release\SoundBand.exe" "%CURRENTDIR%\packages\SoundBand\data\SoundBand.exe"
copy /Y "%CURRENTDIR%\..\QuasarAppLib\build\release\QuasarApp.dll" "%CURRENTDIR%\packages\SoundBand\data\QuasarApp.dll"
copy /Y "%CURRENTDIR%\..\Sync\build\release\Sync.dll" "%CURRENTDIR%\packages\SoundBand\data\Sync.dll"
copy /Y "%CURRENTDIR%\..\tests\build\release\test.exe" "%CURRENTDIR%\packages\SoundBand.Tests\data\test.exe"

%deploy% --qmldir %qmldir% "%CURRENTDIR%\packages\SoundBand\data\SoundBand.exe"
%deploy% --qmldir %qmldir% "%CURRENTDIR%\packages\SoundBand\data\QuasarApp.dll"
%deploy% --qmldir %qmldir% "%CURRENTDIR%\packages\SoundBand\data\Sync.dll"

