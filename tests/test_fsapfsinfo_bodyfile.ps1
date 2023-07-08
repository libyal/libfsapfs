# Info tool testing script
#
# Version: 20230410

$ExitSuccess = 0
$ExitFailure = 1
$ExitIgnore = 77

$Profiles = @("fsapfsinfo_bodyfile_fs")
$OptionsPerProfile = @("-Bbodyfile -H")
$OptionSets = "offset";

$InputGlob = "*"

Function GetTestExecutablesDirectory
{
	$TestExecutablesDirectory = ""

	ForEach (${VSDirectory} in ("msvscpp", "vs2008", "vs2010", "vs2012", "vs2013", "vs2015", "vs2017", "vs2019", "vs2022"))
	{
		ForEach (${VSConfiguration} in ("Release", "VSDebug"))
		{
			ForEach (${VSPlatform} in ("Win32", "x64"))
			{
				$TestExecutablesDirectory = "..\${VSDirectory}\${VSConfiguration}\${VSPlatform}"

				If (Test-Path ${TestExecutablesDirectory})
				{
					Return ${TestExecutablesDirectory}
				}
			}
			$TestExecutablesDirectory = "..\${VSDirectory}\${VSConfiguration}"

			If (Test-Path ${TestExecutablesDirectory})
			{
				Return ${TestExecutablesDirectory}
			}
		}
	}
	Return ${TestExecutablesDirectory}
}

Function ReadIgnoreList
{
	param( [string]$TestProfileDirectory )

	$IgnoreFile = "${TestProfileDirectory}\ignore"
	$IgnoreList = ""

	If (Test-Path -Path ${IgnoreFile} -PathType "Leaf")
	{
		$IgnoreList = Get-Content -Path ${IgnoreFile} | Where {$_ -notmatch '^#.*'}
	}
	Return $IgnoreList
}

$TestExecutablesDirectory = GetTestExecutablesDirectory

If (-Not (Test-Path ${TestExecutablesDirectory}))
{
	Write-Host "Missing test executables directory." -foreground Red

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
	$Options = $OptionsPerProfile[$ProfileIndex]

	$TestProfileDirectory = "input\.${TestProfile}"

	If (-Not (Test-Path -Path ${TestProfileDirectory} -PathType "Container"))
	{
		New-Item -ItemType "directory" -Path ${TestProfileDirectory} | Out-Null
	}
	$IgnoreList = ReadIgnoreList ${TestProfileDirectory}

	# Note that the trailing backtick is needed.
	Get-ChildItem -Path "input" -Exclude ".*" | ForEach-Object `
	{
		$TestSetDirectory = $_

		If (-Not (Test-Path -Path ${TestSetDirectory} -PathType Container))
		{
			Continue
		}
		$TestSetName = ${TestSetDirectory}.Name

		If (${IgnoreList}.Contains(${TestSetName}))
		{
			Continue
		}
		If (-Not (Test-Path -Path "${TestProfileDirectory}\${TestSetName}" -PathType Container))
		{
			New-Item -Name "${TestProfileDirectory}\${TestSetName}" -ItemType "directory" | Out-Null
		}
		If (Test-Path -Path "${TestProfileDirectory}\${TestSetName}\files" -PathType Container)
		{
			$InputFiles = Get-content -Path "${TestProfileDirectory}\${TestSetName}\files"
		}
		Else
		{
			$InputFiles = Get-ChildItem -Path "${TestSetDirectory}\${InputGlob}"
		}
		ForEach ($InputFile in ${InputFiles})
		{
			$InputFileName = ${InputFile}.Name

			$TestedWithOptions = $False

			$TmpDir = "tmp${PID}"

			New-Item -Name ${TmpDir} -ItemType "directory" | Out-Null

			Push-Location ${TmpDir}

			Try
			{
				ForEach ($OptionSet in ${OptionSets} -split " ")
				{
					$TestDataOptionFile = "..\${TestProfileDirectory}\${TestSetName}\${InputFileName}.${OptionSet}"

					If (-Not (Test-Path -Path "${TestDataOptionFile}" -PathType "Leaf"))
					{
						Continue
					}
					$InputOptions = Get-content -Path "${TestDataOptionFile}" -First 1

					$TestLog = "${InputFileName}-${OptionSet}.log"

					Invoke-Expression "..\${TestExecutable} ${Options} ${InputOptions} ${InputFile} > ${TestLog}"
					$Result = $LastExitCode

					If (${Result} -ne ${ExitSuccess})
					{
						Break
					}
					$TestedWithOptions = $True
				}
				If ((${Result} -eq ${ExitSuccess}) -And (-Not (${TestedWithOptions})))
				{
					$TestLog = "${InputFileName}.log"

					Invoke-Expression "..\${TestExecutable} ${Options} ${InputFile} > ${TestLog}"
					$Result = $LastExitCode
				}
				If (${Result} -eq ${ExitSuccess})
				{
					$TestResults = "bodyfile"
					$StoredTestResults = "..\${TestProfileDirectory}\${TestSetName}\${InputFileName}-bodyfile"

					If (Test-Path -Path ${StoredTestResults} -PathType "Leaf")
					{
						$Difference = Compare-Object -ReferenceObject (Get-Content -Path ${StoredTestResults}) -DifferenceObject (Get-Content -Path ${TestResults})

						If (${Difference})
						{
							$Result = ${ExitFailure}
						}
					}
					Else
					{
						Move-Item -Path ${TestResults} -Destination ${StoredTestResults}
					}
				}
			}
			Finally
			{
				Pop-Location

				Remove-Item ${TmpDir} -Force -Recurse
			}
		}
		If (${Result} -ne ${ExitSuccess})
		{
			Break
		}
	}
}

Exit ${Result}

