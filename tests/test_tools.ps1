# Tests tools functions and types.

$ToolsTests = "bodyfile digest_hash info_handle mount_path_string output path_string signal"
$OptionSets = "offset password" -split " "

. .\test_functions.ps1

Function CompareWithReference
{
	param( [string]$TestProfileDirectory, [string]$TestSet, [string]$TestFileName, [string]$TestResults )

	$ExpectedTestResults = "${TestProfileDirectory}\${TestSet}\${TestFileName}"

	If (Test-Path -Path ${ExpectedTestResults} -PathType Leaf)
	{
		$Difference = Compare-Object -ReferenceObject (Get-Content -Path ${ExpectedTestResults}) -DifferenceObject (Get-Content -Path ${TestResults})

		If (${Difference})
		{
			Return ${ExitFailure}
		}
	}
	Else
	{
		New-Item -Force -ItemType Directory -Path "${TestProfileDirectory}\${TestSet}" | Out-Null
		Move-Item -Path ${TestResults} -Destination ${ExpectedTestResults}
	}
	Return ${ExitSuccess}
}

Function RunToolsBinaryAndCompareStdout
{
	param( [string]$TestExecutablesDirectory, [string]$ToolName, [string]$TestProfile, [string]$TestOptions, [string[]]$TestInput )

	$TestExecutable = "${TestExecutablesDirectory}\${ToolName}.exe"

	If (-Not (Test-Path -Path ${TestExecutable} -PathType Leaf))
	{
		$TestDescription = "Missing binary: ${ToolName}"
		WriteTestResult ${TestDescription} ${ExitCommandNotFound}

		Return ${ExitCommandNotFound}
	}
	$OptionSet = $TestInput[0]
	$Options = $TestInput[1]
	$TestFile = $TestInput[2]

	$TestProfileDirectory = "input\.${TestProfile}"
	$TestSet = Split-Path (Split-Path -Path ${TestFile} -Parent) -Leaf
	$TestFileName = Split-Path -Path ${TestFile} -Leaf

	If ($OptionSet)
	{
		$OutputFile = "${TestFileName}-${OptionSet}.log"
	}
	Else
	{
		$OutputFile = "${TestFileName}.log"
	}
	$TmpDir = "tmp${PID}"

	New-Item -Name ${TmpDir} -ItemType "directory" | Out-Null

	Push-Location ${TmpDir}

	Try
	{
		Invoke-Expression "..\${TestExecutable} ${Options} ${TestFile} > ${OutputFile}"
		$Result = $global:LastExitCode

		If (${Result} -eq ${ExitSuccess})
		{
			# Strip header with version.
			(Get-Content ${OutputFile} | Select-Object -Skip 2) | Set-Content ${OutputFile}

			$Result = CompareWithReference "..\${TestProfileDirectory}" ${TestSet} ${TestFileName} ${OutputFile}
		}
	}
	Finally
	{
		Pop-Location

		Remove-Item ${TmpDir} -Force -Recurse
	}
	$TestDescription = "${ToolName} with input: '${TestSet}\${TestFileName}"
	WriteTestResult ${TestDescription} ${Result}

	Return ${Result}
}

$TestExecutablesDirectory = GetTestExecutablesDirectory

If (-Not (Test-Path ${TestExecutablesDirectory}))
{
	Write-Error "Missing test executables directory"

	Exit ${ExitFailure}
}

$Result = ${ExitIgnore}

Foreach (${TestName} in ${ToolsTests} -split " ")
{
	# Split will return an array of a single empty string when ToolsTests is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	$Result = RunTestBinary ${TestExecutablesDirectory} "fsapfs_test_tools_${TestName}"

	If ((${Result} -ne ${ExitSuccess}) -And (${Result} -ne ${ExitIgnore}))
	{
		Break
	}
}

$Profiles = @("fsapfsinfo", "fsapfsinfo_fs", "fsapfsinfo_bodyfile_fs")

For ($ProfileIndex = 0; $ProfileIndex -le ($Profiles.length - 1); $ProfileIndex += 1)
{
	$TestProfile = $Profiles[$ProfileIndex]

	$TestInputs = GenerateTestInputs ${TestProfile} ${OptionSets}

	ForEach ($TestInput in ${TestInputs})
	{
		$Result = RunToolsBinaryAndCompareStdout ${TestExecutablesDirectory} "fsapfsinfo" ${TestProfile} "" ${TestInput}

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
