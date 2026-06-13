# Script that builds libfsapfs
#
# Version: 20260608

Param (
	[string]$Configuration = ${Env:Configuration},
	[string]$Platform = ${Env:Platform},
	[string]$PlatformToolset = "",
	[string]$PythonPath = "C:\Python314",
	[string]$VisualStudioVersion = "",
	[string]$VSToolsOptions = "--extend-with-x64",
	[string]$VSToolsPath = "..\vstools"
)

$ExitSuccess = 0
$ExitFailure = 1

$Python = "${PythonPath}\python.exe"

$Git = "git"
$GitUrl = "https://github.com/libyal/vstools.git"

$MSVSCppConvert = "${VSToolsPath}\vstools\scripts\msvscpp_convert.py"

If (-Not (Test-Path $Python))
{
    Write-Error "Missing Python: ${Python}"

    Exit ${ExitFailure}
}
If (-Not (Test-Path ${VSToolsPath}))
{
	# PowerShell will raise NativeCommandError if git writes to stdout or stderr
	# therefore 2>&1 is added and the output is stored in a variable.
	$Output = Invoke-Expression -Command "${Git} clone ${GitUrl} ${VSToolsPath} 2>&1" | %{ "$_" }
}
Else
{
	Push-Location "${VSToolsPath}"

	Try
	{
		# Make sure vstools are up to date.
		$Output = Invoke-Expression -Command "${Git} pull 2>&1" | %{ "$_" }
	}
	Finally
	{
		Pop-Location
	}
}
If (-Not (Test-Path ${MSVSCppConvert}))
{
	Write-Error "Missing msvscpp_convert.py: ${MSVSCppConvert}"

	Exit ${ExitFailure}
}
If (-Not ${VisualStudioVersion})
{
	$VisualStudioVersion = "2022"

	Write-Warning "Visual Studio version not set defauting to: ${VisualStudioVersion}"
}
if ($VisualStudioVersion -NotIn ("2008", "2010", "2012", "2013", "2015", "2017", "2019", "2022", "2026"))
{
	Write-Error "Unsupported Visual Studio version: ${VisualStudioVersion}"

	Exit ${ExitFailure}
}
$MSBuild = If (Get-Command "MSBuild.exe" -ErrorAction SilentlyContinue) { (Get-Command "MSBuild.exe").Source } else { "" }

If (-Not ${MSBuild})
{
	Switch ($VisualStudioVersion) {
		"2008" {
			$MSBuild = "C:\Windows\Microsoft.NET\Framework\v3.5\MSBuild.exe"
		}
		{ $_ -in "2010", "2012" } {
			$MSBuild = "C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
		}
		"2013" {
			$MSBuild = "C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe"
		}
		"2015" {
			$MSBuild = "C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe"
		}
		"2017" {
			$Results = Get-ChildItem -Path "C:\Program Files\Microsoft Visual Studio\${VisualStudioVersion}\*\MSBuild\15.0\Bin\MSBuild.exe" -Recurse -ErrorAction SilentlyContinue -Force

			If ($Results.Count -eq 0)
			{
				$Results = Get-ChildItem -Path "C:\Program Files (x86)\Microsoft Visual Studio\${VisualStudioVersion}\*\MSBuild\15.0\Bin\MSBuild.exe" -Recurse -ErrorAction SilentlyContinue -Force
			}
			If ($Results.Count -gt 0)
			{
				$MSBuild = $Results[0].FullName
			}
		}
		{ $_ -in "2019", "2022" } {
			$Results = Get-ChildItem -Path "C:\Program Files\Microsoft Visual Studio\${VisualStudioVersion}\*\MSBuild\Current\Bin\MSBuild.exe" -Recurse -ErrorAction SilentlyContinue -Force

			If ($Results.Count -eq 0)
			{
				$Results = Get-ChildItem -Path "C:\Program Files (x86)\Microsoft Visual Studio\${VisualStudioVersion}\*\MSBuild\Current\Bin\MSBuild.exe" -Recurse -ErrorAction SilentlyContinue -Force
			}
			If ($Results.Count -gt 0)
			{
				$MSBuild = $Results[0].FullName
			}
		}
		"2026" {
			$Results = Get-ChildItem -Path "C:\Program Files\Microsoft Visual Studio\18\*\MSBuild\Current\Bin\MSBuild.exe" -Recurse -ErrorAction SilentlyContinue -Force

			If ($Results.Count -eq 0)
			{
				$Results = Get-ChildItem -Path "C:\Program Files (x86)\Microsoft Visual Studio\18\*\MSBuild\Current\Bin\MSBuild.exe" -Recurse -ErrorAction SilentlyContinue -Force
			}
			If ($Results.Count -gt 0)
			{
				$MSBuild = $Results[0].FullName
			}
		}
	}
}
If (-Not ${MSBuild})
{
	Write-Error "Unable to determine path to msbuild.exe"

	Exit ${ExitFailure}
}
ElseIf (-Not (Test-Path ${MSBuild}))
{
	Write-Error "Missing msbuild.exe: ${MSBuild}"

	Exit ${ExitFailure}
}

If (-Not (Test-Path "common\types.h"))
{
	.\autogen.ps1
}
If (${VisualStudioVersion} -eq "2008")
{
	$VSSolutionPath = "msvscpp"
}
Else
{
	$VSSolutionPath = "vs${VisualStudioVersion}"

	If ("${VSToolsOptions}" -notmatch "python[-_]path")
	{
		Write-Warning "No Python path set in VSToolsOptions defaulting to: ${PythonPath}"

		${VSToolsOptions} = "${VSToolsOptions} --python-path ${PythonPath}"
	}
	If (-Not (Test-Path "${VSSolutionPath}"))
	{
		${Env:PYTHONPATH} = ${VSToolsPath}

		Invoke-Expression -Command "& '${Python}' ${MSVSCppConvert} --output-format ${VisualStudioVersion} ${VSToolsOptions} msvscpp\libfsapfs.sln 2>&1" | %{ "$_" }
	}
}
If (${VisualStudioVersion} -eq "2026")
{
	$VSSolutionFile = "${VSSolutionPath}\libfsapfs.slnx"
}
Else
{
	$VSSolutionFile = "${VSSolutionPath}\libfsapfs.sln"
}
If (-Not (Test-Path "${VSSolutionFile}"))
{
	Write-Error "Missing Visual Studio ${VisualStudioVersion} solution file: ${VSSolutionFile}"

	Exit ${ExitFailure}
}
If (-Not ${Configuration})
{
	$Configuration = "Release"

	Write-Warning "Configuration not set defauting to: ${Configuration}"
}
If (-Not ${Platform})
{
	$Platform = "Win32"

	Write-Warning "Platform not set defauting to: ${Platform}"
}
$PlatformToolset = ""

If (-Not ${PlatformToolset})
{
	Switch ($VisualStudioVersion) {
		"2010" {
			$PlatformToolset = "v100"
		}
		"2012" {
			$PlatformToolset = "v110"
		}
		"2013" {
			$PlatformToolset = "v120"
		}
		"2015" {
			$PlatformToolset = "v140"
		}
		"2017" {
			$PlatformToolset = "v141"
		}
		"2019" {
			$PlatformToolset = "v142"
		}
		"2022" {
			$PlatformToolset = "v143"
		}
		"2026" {
			$PlatformToolset = "v145"
		}
	}
	Write-Warning "PlatformToolset not set defauting to: ${PlatformToolset}"
}
$MSBuildOptions = "/verbosity:quiet /target:Build /property:Configuration=${Configuration},Platform=${Platform}"

If (${PlatformToolset})
{
	$MSBuildOptions = "${MSBuildOptions} /property:PlatformToolset=${PlatformToolset}"
}
If (${Env:APPVEYOR} -eq "True")
{
	Invoke-Expression -Command "& '${MSBuild}' ${MSBuildOptions} ${VSSolutionFile} /logger:'C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll' 2>&1" | %{ "$_" }
}
Else
{
	Invoke-Expression -Command "& '${MSBuild}' ${MSBuildOptions} ${VSSolutionFile} 2>&1" | %{ "$_" }
}

Exit ${ExitSuccess}
