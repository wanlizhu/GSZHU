& .\BuildAll.ps1 -CloseShell

if($?) {
    $GTestRunner = ".\Tools\GTestRunner\bin\gtest-runner.exe"
    Start-Process -FilePath $GTestRunner -ArgumentList ".\Build\bin\Debug\Engine_UnitTest.exe"
}

if(-Not $?) {
    Write-Host "!!! 'Run Unit Test' failed"
    Pause
}
