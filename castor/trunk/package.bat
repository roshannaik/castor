SET ROOT=%USERPROFILE%\Documents\Projects
SET SRC=%USERPROFILE%\Documents\Projects\castor-trunk
SET RELEASENAME=castor1.1
SET DST=%USERPROFILE%\Documents\Projects\%RELEASENAME%
SET OUTFILE=castor-1.1
SET OUTDIR=%USERPROFILE%\Documents\Projects\deploy
SET TESTDIR=%USERPROFILE%\Documents\Projects\test

PATH=%PATH%;

echo ----------- 1) STAGING into %DST% --------------


cd %SRC%

rmdir /s /q %DST%
mkdir %DST%

xcopy README.txt %DST%\
xcopy LICENSE.txt %DST%\
xcopy castor.h %DST%\
xcopy whats?new.txt %DST%\

mkdir %DST%\includes
xcopy includes\*.h  %DST%\includes\

mkdir %DST%\docs
xcopy docs\*.pdf %DST%\docs\

mkdir %DST%\test
xcopy test\*.h  %DST%\test\
xcopy test\*.cpp %DST%\test\

attrib +R %DST%


echo ----------- 2) PACKAGING into %OUTDIR% -------------


rmdir /s /q %OUTDIR%
mkdir %OUTDIR%


cd %DST%\..

tar -cvf %OUTDIR%\%OUTFILE%.tar %RELEASENAME%

winrar a %OUTDIR%\%OUTFILE%.zip %RELEASENAME%

cd %DST%\test



echo ----------- 3) TESTING in %TESTDIR & %TESTDIR%2 ---------------


rmdir /s /q  %TESTDIR%
mkdir %TESTDIR%
cd %TESTDIR%
winrar x %OUTDIR%\%OUTFILE%.zip

rmdir /s /q  %TESTDIR%2
mkdir %TESTDIR%2
cd %TESTDIR%2
winrar x %OUTDIR%\%OUTFILE%.tar


cd %RELEASENAME%\test
echo cl /I..\ /EHsc  *.cpp /Fetest_castor
:: test_castor.exe

:: cd %SRC%