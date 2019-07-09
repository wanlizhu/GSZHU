& .\Start-GenerateProject.ps1

$SolutionFile = (Get-ChildItem -Path Build\* -Include *.sln).FullName
Start-Process -FilePath $DevEnv -ArgumentList $SolutionFile
