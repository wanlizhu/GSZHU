& .\Start-GenerateProject.ps1

if(!(Get-Module "VSSetup")){
    Install-Module VSSetup -Scope CurrentUser
}

$VSSetupInstance = Get-VSSetupInstance | Select-VSSetupInstance -Latest -Require Microsoft.VisualStudio.Component.VC.Tools.x86.x64
$VsDevCmd = Join-Path -Path $VSSetupInstance.InstallationPath -ChildPath Common7\Tools\VsDevCmd.bat
$DevEnv = Join-Path -Path $VSSetupInstance.InstallationPath -ChildPath Common7\IDE\devenv.exe

function Start-Process2 {
    Param([String]$FilePath, [Array]$ArgumentList)
    
    $StartInfo = New-Object System.Diagnostics.ProcessStartInfo
    $StartInfo.FileName = $FilePath
    $StartInfo.RedirectStandardError = $true
    $StartInfo.RedirectStandardOutput = $true
    $StartInfo.UseShellExecute = $false
    $StartInfo.Arguments = [System.String]::Join(" ", $ArgumentList)

    $Proc = New-Object System.Diagnostics.Process
    $Proc.StartInfo = $StartInfo
    $Proc.Start() | Out-Null
    $Proc.WaitForExit()

    Write-Host $Proc.StandardOutput.ReadToEnd()
    Write-Host $Proc.StandardError.ReadToEnd()
}

if((Test-Path .\Build\*.sln)) {
    Write-Host "Building Projects..."
    $SolutionFile = (Get-ChildItem -Path Build\* -Include *.sln).FullName
    Start-Process2 -FilePath $DevEnv -ArgumentList "$SolutionFile /Build ""Debug"" /Project ""ZHUENGINE_UnitTest"" "
    
    $GTestRunner = ".\Engine\Programs\GTestRunner\bin\gtest-runner.exe"
    Start-Process -Wait -FilePath $GTestRunner -ArgumentList ".\Build\bin\Debug\ZHUENGINE_UnitTest.exe"
}
