& .\Generate.ps1
& cmake --open .\Build

if($LASTEXITCODE -NE 0) {
    Write-Host "!!! 'Open Visual Studio' failed"
    Pause
}
