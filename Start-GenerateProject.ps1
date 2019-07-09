Write-Host "This might take a while."
Set-ExecutionPolicy Unrestricted -Scope CurrentUser

# CMake configuration and generation
if(!(Test-Path Build) -Or !(Test-Path Build\*.sln)){
    Write-Host "CMake Configuration..."
    New-Item -ItemType Directory -Path Build -Force
    cd Build
    cmake .. -G "Visual Studio 15 2017 Win64"
    cd ..

    if(-Not (Test-Path .\Build\*.sln)) {
        Write-Host "!!! Failed"
        Pause
    }
}