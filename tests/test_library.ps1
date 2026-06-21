# Tests library functions and types.

$LibraryTests = "bit_stream btree_entry btree_footer btree_node btree_node_header buffer_data_handle checkpoint_map checkpoint_map_entry checksum chunk_information_block container_data_handle container_key_bag container_reaper container_superblock compressed_data_handle compression data_block data_block_data_handle data_stream deflate directory_record encryption_context error extended_attribute extent_reference_tree file_entry file_extent file_system file_system_btree file_system_data_handle fusion_middle_tree huffman_tree inode io_handle key_bag_entry key_bag_header key_encrypted_key name name_hash notify object object_map object_map_btree object_map_descriptor profiler snapshot snapshot_metadata snapshot_metadata_tree space_manager volume volume_key_bag"
$LibraryTestsWithInput = "container support"
$OptionSets = "offset password" -split " "

. .\test_functions.ps1

$TestExecutablesDirectory = GetTestExecutablesDirectory

If (-Not (Test-Path ${TestExecutablesDirectory}))
{
	Write-Error "Missing test executables directory"

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
	$Result = RunTestBinary ${TestExecutablesDirectory} "fsapfs_test_${TestName}"

	If ((${Result} -ne ${ExitSuccess}) -And (${Result} -ne ${ExitIgnore}))
	{
		Break
	}
}

$TestInputs = GenerateTestInputs "libfsapfs" ${OptionSets}

Foreach (${TestName} in ${LibraryTestsWithInput} -split " ")
{
	# Split will return an array of a single empty string when LibraryTestsWithInput is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	ForEach ($TestInput in ${TestInputs})
	{
		$Result = RunTestBinaryWithInput ${TestExecutablesDirectory} "fsapfs_test_${TestName}" ${TestInput}

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
