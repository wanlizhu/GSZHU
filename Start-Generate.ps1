Param (
    [String]$Arguments = ""
)

Set-ExecutionPolicy Unrestricted -Scope CurrentUser

if (-Not (Test-Path Build)) {
    New-Item -ItemType Directory -Path Build -Force
}
cd Build
& cmake .. -G "Visual Studio 15 2017 Win64" $Arguments
cd ..

if((-Not (Test-Path .\Build\*.sln)) -Or (-Not $?)) {
    Write-Host "!!! 'Generate Project' Failed"
    Pause
}