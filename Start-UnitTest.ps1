& .\Start-BuildAll.ps1 -CloseShell

if($?) {
    $GTestRunner = ".\Engine\Programs\GTestRunner\bin\gtest-runner.exe"
    Start-Process -FilePath $GTestRunner -ArgumentList ".\Build\bin\Debug\ZHUENGINE_UnitTest.exe"
}

if(-Not $?) {
    Write-Host "!!! 'Run Unit Test' failed"
    Pause
}
