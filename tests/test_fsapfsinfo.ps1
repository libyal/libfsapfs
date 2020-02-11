# Info tool testing script
#
# Version: 20200126

$ExitSuccess = 0
$ExitFailure = 1
$ExitIgnore = 77

$InputGlob = "*"

Function GetTestToolDirectory
{
	$TestToolDirectory = ""

	ForEach (${VSDirectory} in "msvscpp vs2008 vs2010 vs2012 vs2013 vs2015 vs2017 vs2019" -split " ")
	{
		ForEach (${VSConfiguration} in "Release VSDebug" -split " ")
		{
			ForEach (${VSPlatform} in "Win32 x64" -split " ")
			{
				$TestToolDirectory = "..\${VSDirectory}\${VSConfiguration}\${VSPlatform}"

				If (Test-Path ${TestToolDirectory})
				{
					Return ${TestToolDirectory}
				}
			}
			$TestToolDirectory = "..\${VSDirectory}\${VSConfiguration}"

			If (Test-Path ${TestToolDirectory})
			{
				Return ${TestToolDirectory}
			}
		}
	}
	Return ${TestToolDirectory}
}

$TestToolDirectory = GetTestToolDirectory

If (-Not (Test-Path ${TestToolDirectory}))
{
	Write-Host "Missing test tool directory." -foreground Red

	Exit ${ExitFailure}
}

$TestExecutable = "${TestToolDirectory}\fsapfsinfo.exe"

If (-Not (Test-Path -Path "input"))
{
	Exit ${ExitSuccess}
}

Get-ChildItem -Path "input\${InputGlob}" | Foreach-Object
{
	Invoke-Expression ${TestExecutable} $_

	If (${LastExitCode} -ne ${ExitSuccess})
	{
		Break
	}
}

Exit ${LastExitCode}

