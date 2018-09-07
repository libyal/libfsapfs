# Script that builds libfsapfs
#
# Version: 20180728

Param (
	[string]$Configuration = ${Env:Configuration},
	[string]$MSVSCppConvertOptions = "--extend-with-x64",
	[string]$Platform = ${Env:Platform},
	[string]$PythonPath = "C:\Python27",
	[string]$VSToolsPath = "..\vstools"
)

$ExitSuccess = 0
$ExitFailure = 1

$Python = "${PythonPath}\python.exe"

$Git = "git"
$GitUrl = "https://github.com/libyal/vstools.git"

$MSVSCppConvert = "${VSToolsPath}\scripts\msvscpp-convert.py"

If (-Not (Test-Path $Python))
{
    Write-Host "Missing Python: ${Python}" -foreground Red

    Exit ${ExitFailure}
}
If (-Not (Test-Path ${VSToolsPath}))
{
	# PowerShell will raise NativeCommandError if git writes to stdout or stderr
	# therefore 2>&1 is added and the output is stored in a variable.
	$Output = Invoke-Expression -Command "${Git} clone ${GitUrl} ${VSToolsPath} 2>&1"
}
Else
{
	Push-Location "${VSToolsPath}"

	Try
	{
		# Make sure vstools are up to date.
		$Output = Invoke-Expression -Command "${Git} pull 2>&1"
	}
	Finally
	{
		Pop-Location
	}
}
If (-Not (Test-Path ${MSVSCppConvert}))
{
	Write-Host "Missing msvscpp-convert.py: ${MSVSCppConvert}" -foreground Red

	Exit ${ExitFailure}
}
ElseIf (-Not ${Env:VisualStudioVersion})
{
	Write-Host "Unknown Visual Studio version make sure to set %VisualStudioVersion%" -foreground Red

	Exit ${ExitFailure}
}
$OutputFormat = ""

If (${Env:VisualStudioVersion} -eq "15.0")
{
	$OutputFormat = "2017"
}
ElseIf (${Env:VisualStudioVersion} -eq "14.0")
{
	$OutputFormat = "2015"
}
ElseIf (${Env:VisualStudioVersion} -eq "12.0")
{
	$OutputFormat = "2013"
}
ElseIf (${Env:VisualStudioVersion} -eq "11.0")
{
	$OutputFormat = "2012"
}
ElseIf (${Env:VisualStudioVersion} -eq "10.0")
{
	$OutputFormat = "2010"
}
ElseIf (${Env:VisualStudioVersion} -eq "9.0")
{
	$OutputFormat = "2008"
}
Else
{
	Write-Host "Unsupported Visual Studio version: %VisualStudioVersion% = ${Env:VisualStudioVersion}" -foreground Red

	Exit ${ExitFailure}
}
If (${OutputFormat} -eq "2017")
{
	$MSBuild = "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\amd64\MSBuild.exe"
}
ElseIf (${OutputFormat} -eq "2008")
{
	$MSBuild = "C:\\Windows\Microsoft.NET\Framework\v3.5\MSBuild.exe"
}
Else
{
	$MSBuild = "C:\\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
}
If (-Not (Test-Path ${MSBuild}))
{
	Write-Host "Missing msbuild.exe: ${MSBuild}" -foreground Red

	Exit ${ExitFailure}
}
If (${OutputFormat} -eq "2008")
{
	$VSSolutionPath = "msvscpp"
}
Else
{
	$VSSolutionPath = "vs${OutputFormat}"

	If (-Not (Test-Path "${VSSolutionPath}"))
	{
		${Env:PYTHONPATH} = ${VSToolsPath}

		Invoke-Expression -Command "& '${Python}' ${MSVSCppConvert} --output-format ${OutputFormat} ${MSVSCppConvertOptions} msvscpp\libfsapfs.sln"
	}
}
$VSSolutionFile = "${VSSolutionPath}\libfsapfs.sln"

If (-Not (Test-Path "${VSSolutionFile}"))
{
	Write-Host "Missing Visual Studio ${OutputFormat} solution file: ${VSSolutionFile}" -foreground Red

	Exit ${ExitFailure}
}
If (-Not ${Configuration})
{
	$Configuration = "Release"
}
If (-Not ${Platform})
{
	$Platform = "Win32"
}
$MSBuildOptions = "/verbosity:quiet /target:Build /property:Configuration=${Configuration},Platform=${Platform}"

If (${Env:VisualStudioVersion} -eq "15.0")
{
	$MSBuildOptions = "${MSBuildOptions} /property:PlatformToolset=v141"
}
Invoke-Expression -Command "& '${MSBuild}' ${MSBuildOptions} ${VSSolutionFile}"

Exit ${ExitSuccess}
