# Script that builds dokan
#
# Version: 20180322

Param (
	[string]$Configuration = ${Env:Configuration},
	[string]$Platform = ${Env:Platform},
	[switch]$UseLegacyVersion = $false
)

If (-not ${Configuration})
{
	$Configuration = "Release"
}
If (-not ${Platform})
{
	$Platform = "Win32"
}

If (${Env:AppVeyor} -eq "True")
{
	$MSBuild = "MSBuild.exe"
}
ElseIf (${Env:VisualStudioVersion} -eq "15.0")
{
	$MSBuild = "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\amd64\MSBuild.exe"
}
ElseIf (${Env:VisualStudioVersion} -eq "9.0")
{
	$MSBuild = "C:\\Windows\Microsoft.NET\Framework\v3.5\MSBuild.exe"
}
Else
{
	$MSBuild = "C:\\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
}
$MSBuildOptions = "/verbosity:quiet /target:Build /property:Configuration=${Configuration},Platform=${Platform}"

If ($UseLegacyVersion)
{
	$DokanPath = "../dokan"
	$ProjectFile = "msvscpp\dokan.sln"
}
Else
{
	$DokanPath = "../dokany"
	$ProjectFile = "dokan\dokan.vcxproj"
}

Push-Location ${DokanPath}

Try
{
	Write-Host "${MSBuild} ${MSBuildOptions} ${ProjectFile}"

	Invoke-Expression -Command "& '${MSBuild}' ${MSBuildOptions} ${ProjectFile}"
}
Finally
{
	Pop-Location
}

