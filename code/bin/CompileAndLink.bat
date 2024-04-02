SET APP_NAME=masterarbeit
SET LOCAL_DEV=C:\Users\Lis\Uni\sem12\Hiwi\masterarbeit
SET VS=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.36.32532\bin\Hostx64\x64
SET PATH=%PATH%;%VS%
SET EXPORT_BIN=%LOCAL_DEV%
SET VCPATH="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\amd64"
DEL %APP_NAME%.exe /F /S /Q
%VCPATH%\MSBuild.exe /m:2 "%LOCAL_DEV%\%APP_NAME%.sln" /p:Platform=x64;Configuration=Release;WarningLevel=0
xcopy %LOCAL_DEV%\x64\Release\%APP_NAME%.exe %EXPORT_BIN% /E /I /Y

