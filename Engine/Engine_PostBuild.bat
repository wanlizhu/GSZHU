@echo off

if "%VC_OUTDIR%"=="" (
    echo PostBuildEventError: Variable 'VC_OUTDIR' is NOT defined.
    exit
)

xcopy /e/y "resource\*" "%VC_OUTDIR%resource\"
