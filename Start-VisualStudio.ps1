& .\Start-GenerateProject.ps1

$VSSetupInstance = Get-VSSetupInstance | Select-VSSetupInstance -Latest -Require Microsoft.VisualStudio.Component.VC.Tools.x86.x64
$DevEnv = Join-Path -Path $VSSetupInstance.InstallationPath -ChildPath Common7\IDE\devenv.exe

$SolutionFile = "Build/ZHUENGINE.sln"
Start-Process -FilePath $DevEnv -ArgumentList $SolutionFile

if(-Not $?) {
    Write-Host "!!! Command failed"
    Pause
}