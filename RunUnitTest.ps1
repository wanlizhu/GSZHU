& .\GenerateProject.ps1

if(!(Get-Module "VSSetup")){
    Install-Module VSSetup -Scope CurrentUser
}

$VSSetupInstance = Get-VSSetupInstance | Select-VSSetupInstance -Latest -Require Microsoft.VisualStudio.Component.VC.Tools.x86.x64
$VsDevCmd = Join-Path -Path $VSSetupInstance.InstallationPath -ChildPath Common7\Tools\VsDevCmd.bat
$DevEnv = Join-Path -Path $VSSetupInstance.InstallationPath -ChildPath Common7\IDE\devenv.exe

if((Test-Path .\Build\*.sln)) {
    $SolutionFile = (Get-ChildItem -Path Build\* -Include *.sln).FullName
    Start-Process -Wait -FilePath $DevEnv -ArgumentList "$SolutionFile /Build ""Debug"" /Project ""ZHUENGINE_UnitTest"" "
    
    Write-Host "Starting gtest-runner.exe..."
    $GTestRunner = ".\Engine\Programs\GTestRunner\bin\gtest-runner.exe"
    Start-Process -Wait -FilePath $GTestRunner -ArgumentList ".\Build\bin\Debug\ZHUENGINE_UnitTest.exe"
}
