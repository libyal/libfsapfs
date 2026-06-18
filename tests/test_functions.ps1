# Tests functions.
#
# Version: 20260617

$ExitSuccess = 0
$ExitFailure = 1
$ExitIgnore = 77
$ExitCommandNotFound = 127

$VSDirectories = @(
	"msvscpp",
	"vs2008",
	"vs2010",
	"vs2012",
	"vs2013",
	"vs2015",
	"vs2017",
	"vs2019",
	"vs2022",
	"vs2026"
)

$VSConfigurations = @(
	"Release",
	"VSDebug"
)

$VSPlatforms = @(
	"Win32",
	"x64"
)

Function GenerateTestInputs
{
	param( [string]$TestProfile, [string[]]$OptionSets )

	$TestInputs = [System.Collections.Generic.List[string[]]]::new()

	If (Test-Path -Path "input")
	{
		$TestProfileDirectory = "input\.${TestProfile}"

		$IgnoreList = ReadIgnoreList "input\.${TestProfile}"

		ForEach ($DirectoryEntry in Get-ChildItem -Path "input" -Exclude ".*")
		{
			$TestSet = ${DirectoryEntry}.Name

			If (-Not (Test-Path -Path ${DirectoryEntry} -PathType Container))
			{
				Write-Host "Skipping '${TestSet}' not a directory"

				Continue
			}
			If (${TestSet} -Contains ${IgnoreList})
			{
				Write-Host "Skipping '${TestSet}' defined in ignore list"

				Continue
			}
			If (Test-Path -Path "input\.${TestProfile}\${TestSet}\files" -PathType Leaf)
			{
				$GlobFiles=$False
			}
			Else
			{
				$GlobFiles=$True
			}
			If (-Not ($GlobFiles))
			{
				$TestFiles = ReadTestFiles "input\.${TestProfile}" ${TestSet} "input\${TestSet}"
			}
			Else
			{
				$GlobFile = "input\.${TestProfile}\glob"

				If (Test-Path -Path "${GlobFile}" -PathType Leaf)
				{
					$TestFilesGlob = Get-content -Path "${GlobFile}" -First 1
				}
				Else
				{
					$TestFilesGlob = ${InputGlob}
				}
				# Note FullName is necessary otherwise TestFiles will contain objects.
				$TestFiles = Get-ChildItem -Path "input\${TestSet}" -Include ${TestFilesGlob} | ForEach-Object -MemberName FullName
			}
			If (-Not (${TestFiles}))
			{
				Write-Host "Skipping '${TestSet}' no test files"

				Continue
			}
			ForEach ($TestFile in ${TestFiles})
			{
				$TestFileName = Split-Path -Path ${TestFile} -Leaf

				If (-Not ($GlobFiles) -And -Not (Test-Path -Path "${GlobFile}" -PathType Leaf))
				{
					Write-Host "Skipping missing file '${TestFile}' defined in '${TestSet}\files'"

					Continue
				}
				$OptionsPerProfile = ""
				$OptionsFile = "input\.${TestProfile}\options"

				If (Test-Path -Path "${OptionsFile}" -PathType Leaf)
				{
					$OptionsPerProfile = Get-content -Path "${OptionsFile}" -First 1
				}
				$TestedWithOptions = $False

				ForEach ($OptionSet in ${OptionSets})
				{
					# Split will return an array of a single empty string when OptionSets is empty.
					If (-Not (${OptionSets}))
					{
						Continue
					}
					$Options = ReadOptions "input" ${TestProfile} ${TestSet} ${TestFileName} ${OptionSet}

					If ($Options)
					{
						if (${OptionsPerProfile})
						{
							$Options = "${OptionsPerProfile} ${Options}"
						}
						$TestInput = @(${OptionSet}, $Options, ${TestFile})
						$TestInputs.Add(${TestInput})

						$TestedWithOptions = $True
					}
				}
				If (-Not (${TestedWithOptions}))
				{
					$TestInput = @("", ${OptionsPerProfile}, ${TestFile})
					$TestInputs.Add(${TestInput})
				}
			}
			If (${Result} -ne ${ExitSuccess})
			{
				Break
			}
		}
	}
	# Note the leading comma here is deliberate.
	Return ,${TestInputs}
}

Function GetTestExecutablesDirectory
{
	$TestExecutablesDirectory = ""

	ForEach (${VSDirectory} in $VSDirectories)
	{
		ForEach (${VSConfiguration} in $VSConfigurations)
		{
			ForEach (${VSPlatform} in $VSPlatforms)
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

	$IgnoreList = ""

	$IgnoreFile = "${TestProfileDirectory}\ignore"
	If (Test-Path -Path ${IgnoreFile} -PathType Leaf)
	{
		$IgnoreList = Get-Content -Path ${IgnoreFile} | Where {$_ -notmatch '^#.*'}
	}
	Return $IgnoreList
}

Function ReadOptions
{
	param( [string]$InputDirectory, [string]$TestProfile, [string]$TestSet, [string]$TestFileName, [string]$OptionSet )

	$Options = ""

	$File = "${InputDirectory}\.${TestProfile}\${TestSet}\${TestFileName}.${OptionSet}"

	If (Test-Path -Path "${File}" -PathType Leaf)
	{
		# The original format of the options file contains all options on the first line.
		$Options = Get-content -Path "${File}" -First 1

		# The newer format of the options file has a distinct header.
		If (${Options} -match "^# libyal test data options")
		{
			$Options = Get-content -Path "${File}" | Select-Object -Skip 1

			$Options = $Options -replace "^offset=","-o"
			$Options = $Options -replace "^password=","-p"
			$Options = $Options -replace "^recovery_password=","-r"
			$Options = $Options -replace "^startup_key=","-s"
			$Options = $Options -replace "^virtual_address=","-v"
		}
	}
	Return $Options
}

Function ReadTestFiles
{
	param( [string]$TestProfileDirectory, [string]$TestSet, [string]$TestSetInputDirectory )

	$TestFiles = Get-Content -Path "${TestProfileDirectory}\${TestSet}\files" | Where {$_ -ne ""}
	$TestFiles = $TestFiles -replace "^","${TestSetInputDirectory}\"

	Return $TestFiles
}

Function RunTestBinary
{
	param( [string]$TestExecutablesDirectory, [string]$TestName )

	$TestExecutable = "${TestExecutablesDirectory}\${TestName}.exe"

	If (-Not (Test-Path -Path ${TestExecutable} -PathType Leaf))
	{
		$TestDescription = "Missing binary: ${TestName}"
		WriteTestResult ${TestDescription} ${ExitCommandNotFound}

		Return ${ExitCommandNotFound}
	}
	$Output = Invoke-Expression ${TestExecutable}
	$Result = $global:LastExitCode

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host ${Output} -foreground Red
	}
	$TestDescription = "${TestName}"
	WriteTestResult ${TestDescription} ${Result}

	Return ${Result}
}

Function RunTestBinaryWithInput
{
	param( [string]$TestExecutablesDirectory, [string]$TestName, [string[]]$TestInput )

	$OptionSet = $TestInput[0]
	$Options = $TestInput[1]
	$TestFile = $TestInput[2]

	$TestFileName = (${TestFile} -split '\\')[-2..-1] -join '\'

	$TestExecutable = "${TestExecutablesDirectory}\${TestName}.exe"

	If (-Not (Test-Path -Path ${TestExecutable} -PathType Leaf))
	{
		$TestDescription = "Missing binary: ${TestName}"
		WriteTestResult ${TestDescription} ${ExitCommandNotFound}

		Return ${ExitCommandNotFound}
	}
	$Output = Invoke-Expression "${TestExecutable} ${Options} ${TestFile}"
	$Result = $global:LastExitCode

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host ${Output} -foreground Red
	}
	$TestDescription = "${TestName} with input: '${TestFileName}"
	WriteTestResult ${TestDescription} ${Result}

	Return ${Result}
}

Function WriteTestResult
{
        param( [string]$Description, [int]$Result )

        Write-Host "${Description} (" -nonewline

        If (${Result} -eq ${ExitSuccess})
        {
                Write-Host "PASS" -foreground Green -nonewline
        }
        ElseIf (${Result} -eq ${ExitIgnore})
        {
		Write-Host "SKIP" -foreground Cyan -nonewline
        }
        Else
        {
                Write-Host "FAIL" -foreground Red -nonewline
        }
        Write-Host ")"
}
