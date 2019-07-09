& .\Start-BuildEngine.ps1 -CloseShell

& cmake --build .\Build --parallel --target INSTALL --config Debug

if($LASTEXITCODE -NE 0) {
    Write-Host "!!! 'Make Install' failed"
}

Pause