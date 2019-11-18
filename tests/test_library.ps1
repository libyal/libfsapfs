# Tests C library functions and types.
#
# Version: 20181221

$ExitSuccess = 0
$ExitFailure = 1
$ExitIgnore = 77

$LibraryTests = "btree_entry btree_footer btree_node btree_node_header buffer_data_handle checkpoint_map checkpoint_map_entry checksum chunk_information_block container_data_handle container_key_bag container_reaper container_superblock compressed_data_handle compression data_block data_stream deflate directory_record encryption_context error file_extent file_system_btree inode io_handle key_bag_entry key_bag_header key_encrypted_key name name_hash notify object object_map object_map_btree object_map_descriptor space_manager volume volume_key_bag"
$LibraryTestsWithInput = "container support"

$InputGlob = "*"

Function GetTestProfileDirectory
{
	param( [string]$TestInputDirectory, [string]$TestProfile )

	$TestProfileDirectory = "${TestInputDirectory}\.${TestProfile}"

	If (-Not (Test-Path -Path ${TestProfileDirectory} -PathType "Container"))
	{
		New-Item -ItemType "directory" -Path ${TestProfileDirectory}
	}
	Return ${TestProfileDirectory}
}

Function GetTestSetDirectory
{
	param( [string]$TestProfileDirectory, [string]$TestSetInputDirectory )

	$TestSetDirectory = "${TestProfileDirectory}\${TestSetInputDirectory.Basename}"

	If (-Not (Test-Path -Path ${TestSetDirectory} -PathType "Container"))
	{
		New-Item -ItemType "directory" -Path ${TestSetDirectory}
	}
	Return ${TestSetDirectory}
}

Function GetTestToolDirectory
{
	$TestToolDirectory = ""

	ForEach (${VSDirectory} in "msvscpp vs2008 vs2010 vs2012 vs2013 vs2015 vs2017" -split " ")
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

Function RunTest
{
	param( [string]$TestType )

	$TestDescription = "Testing: ${TestName}"
	$TestExecutable = "${TestToolDirectory}\fsapfs_test_${TestName}.exe"

	$Output = Invoke-Expression ${TestExecutable}
	$Result = ${LastExitCode}

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host ${Output} -foreground Red
	}
	Write-Host "${TestDescription} " -nonewline

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host " (FAIL)"
	}
	Else
	{
		Write-Host " (PASS)"
	}
	Return ${Result}
}

Function RunTestWithInput
{
	param( [string]$TestType )

	$TestDescription = "Testing: ${TestName}"
	$TestExecutable = "${TestToolDirectory}\fsapfs_test_${TestName}.exe"

	$TestProfileDirectory = GetTestProfileDirectory "input" "libfsapfs"

	$IgnoreList = ReadIgnoreList ${TestProfileDirectory}

	$Result = ${ExitSuccess}

	ForEach ($TestSetInputDirectory in Get-ChildItem -Path "input" -Exclude ".*")
	{
		If (-Not (Test-Path -Path ${TestSetInputDirectory} -PathType "Container"))
		{
			Continue
		}
		If (${TestSetInputDirectory} -Contains ${IgnoreList})
		{
			Continue
		}
		$TestSetDirectory = GetTestSetDirectory ${TestProfileDirectory} ${TestSetInputDirectory}

		If (Test-Path -Path "${TestSetDirectory}\files" -PathType "Leaf")
		{
			$InputFiles = Get-Content -Path "${TestSetDirectory}\files" | Where {$_ -ne ""}
		}
		Else
		{
			$InputFiles = Get-ChildItem -Path ${TestSetInputDirectory} -Include ${InputGlob}
		}
		ForEach ($InputFile in ${InputFiles})
		{
			# TODO: add test option support
			$Output = Invoke-Expression ${TestExecutable}
			$Result = ${LastExitCode}

			If (${Result} -ne ${ExitSuccess})
			{
				Break
			}
		}
		If (${Result} -ne ${ExitSuccess})
		{
			Break
		}
	}
	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host ${Output} -foreground Red
	}
	Write-Host "${TestDescription} " -nonewline

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host " (FAIL)"
	}
	Else
	{
		Write-Host " (PASS)"
	}
	Return ${Result}
}

$TestToolDirectory = GetTestToolDirectory

If (-Not (Test-Path ${TestToolDirectory}))
{
	Write-Host "Missing test tool directory." -foreground Red

	Exit ${ExitFailure}
}

$Result = ${ExitIgnore}

Foreach (${TestName} in ${LibraryTests} -split " ")
{
	# Split will return an array of a single empty string when LibraryTests is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	$Result = RunTest ${TestName}

	If (${Result} -ne ${ExitSuccess})
	{
		Break
	}
}

Foreach (${TestName} in ${LibraryTestsWithInput} -split " ")
{
	# Split will return an array of a single empty string when LibraryTestsWithInput is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	If (Test-Path -Path "input" -PathType "Container")
	{
		$Result = RunTestWithInput ${TestName}
	}
	Else
	{
		$Result = RunTest ${TestName}
	}
	If (${Result} -ne ${ExitSuccess})
	{
		Break
	}
}

Exit ${Result}

