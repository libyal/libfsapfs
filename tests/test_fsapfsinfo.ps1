# Info tool testing script
#
# Version: 20251224

$ExitSuccess = 0
$ExitFailure = 1
$ExitIgnore = 77

$Profiles = @("fsapfsinfo", "fsapfsinfo_fs")
$OptionsPerProfile = @("", "-H")
$OptionSets = "offset password";

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

	If (Test-Path -Path ${IgnoreFile} -PathType Leaf)
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

	If (-Not (Test-Path -Path ${TestProfileDirectory} -PathType Container))
	{
		New-Item -ItemType "directory" -Path ${TestProfileDirectory} | Out-Null
	}
	$IgnoreList = ReadIgnoreList ${TestProfileDirectory}

	ForEach ($TestSetInputDirectory in Get-ChildItem -Path "input" -Exclude ".*")
	{
		If (-Not (Test-Path -Path ${TestSetInputDirectory} -PathType Container))
		{
			Continue
		}
		If (${TestSetInputDirectory} -Contains ${IgnoreList})
		{
			Continue
		}
		$TestSetName = ${TestSetInputDirectory}.Name

		If (Test-Path -Path "${TestProfileDirectory}\${TestSetName}\files" -PathType Leaf)
		{
			$InputFiles = Get-Content -Path "${TestProfileDirectory}\${TestSetName}\files" | Where {$_ -ne ""}
			$InputFiles = $InputFiles -replace "^","${TestSetInputDirectory}\"
		}
		Else
		{
			$InputFiles = Get-ChildItem -Path ${TestSetInputDirectory} -Include ${InputGlob}
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

					If (-Not (Test-Path -Path "${TestDataOptionFile}" -PathType Leaf))
					{
						Continue
					}
					$OptionsHeader = Get-content -Path "${TestDataOptionFile}" -First 1

					If (-Not (${OptionsHeader} -match "^# libyal test data options"))
					{
						Continue
					}
					$InputOptions = Get-content -Path "${TestDataOptionFile}" | Select-Object -Skip 1

					$InputOptions = $InputOptions -replace "^offset=","-o"
					$InputOptions = $InputOptions -replace "^password=","-p"
					$InputOptions = $InputOptions -replace "^recovery_password=","-r"
					$InputOptions = $InputOptions -replace "^startup_key=","-s"
					$InputOptions = $InputOptions -replace "^virtual_address=","-v"

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
					# Strip header with version.
					(Get-Content ${TestLog} | Select-Object -Skip 2) | Set-Content ${TestLog}

					$StoredTestLog = "..\${TestProfileDirectory}\${TestSetName}\${TestLog}"

					If (Test-Path -Path ${StoredTestLog} -PathType Leaf)
					{
						$Difference = Compare-Object -ReferenceObject (Get-Content -Path ${StoredTestLog}) -DifferenceObject (Get-Content -Path ${TestLog})

						If (${Difference})
						{
							$Result = ${ExitFailure}
						}
					}
					Else
					{
						Move-Item -Path ${TestLog} -Destination ${StoredTestLog}
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

