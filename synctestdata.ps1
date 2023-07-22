# Script that synchronizes the local test data
#
# Version: 20230709

$TestSet = "public"
$TestInputDirectory = "tests/input"
$TestFiles = "apfs.raw"

If (-Not (Test-Path ${TestInputDirectory}))
{
	New-Item -Name ${TestInputDirectory} -ItemType "directory" | Out-Null
}
If (-Not (Test-Path "${TestInputDirectory}\${TestSet}"))
{
	New-Item -Name "${TestInputDirectory}\${TestSet}" -ItemType "directory" | Out-Null
}
ForEach ($TestFile in ${TestFiles} -split " ")
{
	$Url = "https://github.com/log2timeline/dfvfs/blob/main/test_data/${TestFile}?raw=true"

	Invoke-WebRequest -Uri ${Url} -OutFile "${TestInputDirectory}\${TestSet}\${TestFile}"
}

