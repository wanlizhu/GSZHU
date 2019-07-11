Param (
    [Switch]$CloseShell
)

& .\Generate.ps1 -Arguments "-DBUILD_SAMPLES=OFF -DBUILD_TESTS=OFF"
& cmake --build .\Build --parallel --target Engine --config Debug

if($LASTEXITCODE -NE 0) {
    Write-Host "!!! 'Build Engine' failed"
    Pause
}
elseif(-Not $CloseShell) {
    Pause
}