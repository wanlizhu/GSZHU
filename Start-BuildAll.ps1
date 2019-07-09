Param (
    [Switch]$CloseShell
)

& .\Start-Generate.ps1 -Arguments "-DBUILD_SAMPLES=ON -DBUILD_TESTS=ON"

& cmake --build .\Build --parallel --target ALL_BUILD --config Debug

if($LASTEXITCODE -NE 0) {
    Write-Host "!!! 'Build All' failed"
    Pause
}
elseif(-Not $CloseShell) {
    Pause
}