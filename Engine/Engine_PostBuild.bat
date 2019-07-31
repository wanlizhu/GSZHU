@echo off

if "%VC_OUTDIR%"=="" (
    echo PostBuildEventError: Variable 'VC_OUTDIR' is NOT defined.
    exit
)

::xcopy /e/y "external\vld\bin\dbghelp.dll" "%VC_OUTDIR%"
