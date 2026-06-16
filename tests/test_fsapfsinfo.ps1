# Info tool testing script
#
# Version: 20260615

$Profiles = @("fsapfsinfo", "fsapfsinfo_fs", "fsapfsinfo_bodyfile_fs")
$OptionSets = "" -split " "

. .\test_functions.ps1

$TestExecutablesDirectory = GetTestExecutablesDirectory

If (-Not (Test-Path ${TestExecutablesDirectory}))
{
	Write-Error "Missing test executables directory"

	Exit ${ExitFailure}
}

$TestExecutable = "${TestExecutablesDirectory}\fsapfsinfo.exe"

If (-Not (Test-Path -Path "input"))
{
	Exit ${ExitIgnore}
}
$Result = ${ExitSuccess}

For ($ProfileIndex = 0; $ProfileIndex -le ($Profiles.length - 1); $ProfileIndex += 1)
{
	$TestProfile = $Profiles[$ProfileIndex]

	$TestProfileDirectory = "input\.${TestProfile}"

	$TestInputs = GenerateTestInputs ${TestProfile} ${OptionSets}

	ForEach ($TestInput in ${TestInputs})
	{
		$TmpDir = "tmp${PID}"

		New-Item -Name ${TmpDir} -ItemType "directory" | Out-Null

		Push-Location ${TmpDir}

		Try
		{
			$OptionSet = $TestInput[0]
			$Options = $TestInput[1]
			$TestFile = $TestInput[2]

			$TestFileName = Split-Path -Path ${TestFile} -Leaf
			$TestSet = Split-Path (Split-Path -Path ${TestFile} -Parent) -Leaf

			If ($OptionSet)
			{
				$OutputFile = "${TestFileName}-${OptionSet}.log"
			}
			Else
			{
				$OutputFile = "${TestFileName}.log"
			}
			Invoke-Expression "..\${TestExecutable} ${OptionsPerProfile} ${Options} ${TestFile} > ${OutputFile}"
			$Result = $LastExitCode

			If (${Result} -eq ${ExitSuccess})
			{
				# Strip header with version.
				(Get-Content ${OutputFile} | Select-Object -Skip 2) | Set-Content ${OutputFile}

				$Result = CompareWithReference "..\${TestProfileDirectory}" ${TestSet} ${TestFileName} ${OutputFile}
			}
			$TestDescription = "fsapfsinfo with input: '${TestSet}\${TestFileName}"

			WriteTestResult ${TestDescription} ${Result}

			If (${Result} -ne ${ExitSuccess})
			{
				Break
			}
		}
		Finally
		{
			Pop-Location

			Remove-Item ${TmpDir} -Force -Recurse
		}

		If ((${Result} -ne ${ExitSuccess}) -And (${Result} -ne ${ExitIgnore}))
		{
			Break
		}
	}
	If ((${Result} -ne ${ExitSuccess}) -And (${Result} -ne ${ExitIgnore}))
	{
		Break
	}
}

Exit ${Result}

