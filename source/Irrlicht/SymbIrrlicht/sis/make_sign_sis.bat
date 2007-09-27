REM !!!! replace the certificate and key file with your own ones!!!!

@echo off
set filename=SymbIrrlicht
if "%1"=="gcce" set filename=SymbIrrlicht-GCCE
for  /F "usebackq tokens=3 delims= " %%i in (`devices -default`) DO (
  for  /F "usebackq tokens=1,3 delims= " %%j in (`devices -info @%%i`) DO if "%%j"=="Root" makesis -d%%k %filename%.pkg
  )
)
signsis %filename%.sis %filename%.sisx ming-rd.crt ming-rd.key
