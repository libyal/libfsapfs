# Script to generate the necessary files for a msvscpp build
#
# Version: 20260608

$WinFlex = "..\win_flex_bison\win_flex.exe"
$WinBison = "..\win_flex_bison\win_bison.exe"

$PackageName = Get-Content -Path configure.ac |
	select -skip 3 -first 1 |
	 % { $_ -Replace "  \[","" } |
	 % { $_ -Replace "\],","" }
$PackageVersion = Get-Content -Path configure.ac |
	 select -skip 4 -first 1 |
	 % { $_ -Replace "  \[","" } |
	 % { $_ -Replace "\],","" }

Get-Content -Path "common\types.h.in" |
	% { $_ -Replace "@PACKAGE@","${PackageName}" } |
	Out-File -Encoding ascii "common\types.h"

If (Test-Path "include\${PackageName}.h.in")
{
	Get-Content -Path "include\${PackageName}.h.in" |
		Out-File -Encoding ascii "include\${PackageName}.h"
	Get-Content -Path "include\${PackageName}\definitions.h.in" |
		% { $_ -Replace "@VERSION@","${PackageVersion}" } |
		Out-File -Encoding ascii "include\${PackageName}\definitions.h"
	Get-Content -Path "include\${PackageName}\features.h.in" |
		% { $_ -Replace "@[A-Z0-9_]*@","0" } |
		Out-File -Encoding ascii "include\${PackageName}\features.h"
	Get-Content -Path "include\${PackageName}\types.h.in" |
		% { $_ -Replace "@[A-Z0-9_]*@","0" } |
		Out-File -Encoding ascii "include\${PackageName}\types.h"
}
If (Test-Path "${PackageName}\${PackageName}.c")
{
	Get-Content -Path "${PackageName}\${PackageName}_definitions.h.in" |
		% { $_ -Replace "@VERSION@","${PackageVersion}" } |
		Out-File -Encoding ascii "${PackageName}\${PackageName}_definitions.h"
	Get-Content -Path "${PackageName}\${PackageName}.rc.in" |
		% { $_ -Replace "@VERSION@","${PackageVersion}" } |
		Out-File -Encoding ascii "${PackageName}\${PackageName}.rc"
}
If (Test-Path "pyproject.toml.in")
{
	Get-Content -Path "pyproject.toml.in" |
		% { $_ -Replace "@VERSION@","${PackageVersion}" } |
		Out-File -Encoding ascii "pyproject.toml"
}
$PackagePrefix = ${PackageName}.Substring(3)

If (Test-Path "${PackagePrefix}.net")
{
	Get-Content -Path "${PackagePrefix}.net\${PackagePrefix}.net.rc.in" |
		% { $_ -Replace "@VERSION@","${PackageVersion}" } |
		Out-File -Encoding ascii "${PackagePrefix}.net\${PackagePrefix}.net.rc"
}
ForEach (${PackageName} in Get-ChildItem -Directory -Path "lib*")
{
	$NamePrefix = ""

	ForEach (${DirectoryElement} in Get-ChildItem -Path "${PackageName}\*.l")
	{
		$OutputFile = ${DirectoryElement} -Replace ".l$",".c"

		$NamePrefix = Split-Path -path ${DirectoryElement} -leaf
		$NamePrefix = ${NamePrefix} -Replace ".l$","_"

		$WinFlexArguments = @(
			"-Cf",
			"${DirectoryElement}"
		)
		Write-Host "Running: ${WinFlex} $($WinFlexArguments -join ' ')"

		# PowerShell will raise NativeCommandError if win_flex writes to stdout or stderr
		# therefore 2>&1 is added and the output is stored in a variable.
		$Output = Invoke-Expression -Command "& '${WinFlex}' $($WinFlexArguments -join ' ') 2>&1" | %{ "$_" }
		Write-Host ${Output}

		# Moving manually since `win_flex -o filename' does not provide the expected behavior.
		Move-Item "lex.yy.c" ${OutputFile} -force
	}
	ForEach (${DirectoryElement} in Get-ChildItem -Path "${PackageName}\*.y")
	{
		$OutputFile = ${DirectoryElement} -Replace ".y$",".c"

		$WinBisonArguments = @(
			"-d"
			"-v"
			"-l"
			"-p", "${NamePrefix}"
			"-o", "${OutputFile}"
			"${DirectoryElement}"
		)
		Write-Host "Running: ${WinBison} $($WinBisonArguments -join ' ')"

		# PowerShell will raise NativeCommandError if win_bison writes to stdout or stderr
		# therefore 2>&1 is added and the output is stored in a variable.
		$Output = Invoke-Expression -Command "& '${WinBison}' $($WinBisonArguments -join ' ') 2>&1" | %{ "$_" }
		Write-Host ${Output}
	}
}
