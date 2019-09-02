# Script that synchronizes dokan
#
# Version: 20190810

Param (
	[switch]$UseHead = $false,
	[switch]$UseLegacyVersion = $false
)

$Git = "git"

If (${UseLegacyVersion})
{
	# Patched version of dokan 0.6.0
	$GitUrl = "https://github.com/joachimmetz/dokan.git"
	$Destination = "..\dokan"
}
Else
{
	$GitUrl = "https://github.com/dokan-dev/dokany.git"
	$Destination = "..\dokany"
}

# PowerShell will raise NativeCommandError if git writes to stdout or stderr
# therefore 2>&1 is added and the output is stored in a variable.
$Output = Invoke-Expression -Command "${Git} clone ${GitUrl} ${Destination} 2>&1"

Push-Location ${Destination}

Try
{
	$Output = Invoke-Expression -Command "${Git} fetch --quiet --all --tags --prune 2>&1"

	$LatestTag = Invoke-Expression -Command "${Git} describe --tags --abbrev=0 2>&1"

	If (${LatestTag} -and -not ${UseHead})
	{
		Write-Host "Synchronizing: dokan from ${GitUrl} tag ${LatestTag}"

		$Output = Invoke-Expression -Command "${Git} checkout --quiet tags/${LatestTag} 2>&1"
	}
	Else
	{
		Write-Host "Synchronizing: dokan from ${GitUrl} HEAD"
	}
	If (-Not ${UseLegacyVersion})
	{
		# AppVeyor does not come with platform toolset version 142
		((Get-Content -Path "..\dokany\dokan\dokan.vcxproj" -Raw) -Replace '<PlatformToolset>v142</PlatformToolset>','<PlatformToolset>v141</PlatformToolset>') | Set-Content -Path "..\dokany\dokan\dokan.vcxproj"
	}
}
Finally
{
	Pop-Location
}

