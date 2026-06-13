# Script that runs the tests
#
# Version: 20260609

$ExitSuccess = 0
$ExitFailure = 1
$ExitIgnore = 77

Set-Location -Path "tests"

$Result = ${ExitSuccess}

$Lines = Get-Content "Makefile.am"
$InTests = $FALSE

Foreach (${Line} in ${Lines})
{
	If (${InTests})
	{
		If (-Not ${Line})
		{
			${InTests} = $FALSE

			Continue
		}
		${Line} = ${Line}.TrimStart()

		If (${Line}.EndsWith(" \"))
		{
			${Line} = ${Line}.Substring(0, ${Line}.Length - 2)
		}
		${Line} = ".\${Line}.ps1"

		Try
		{
			Invoke-Expression ${Line}
		}
		Catch
		{
			$LastExitCode = ${ExitIgnore}
		}
		If (${LastExitCode} -eq ${ExitFailure})
		{
			$Result = ${ExitFailure}
			Write-Host "FAIL" -foreground Red -nonewline
		}
		ElseIf (${LastExitCode} -eq ${ExitIgnore})
		{
			Write-Host "SKIP" -foreground Cyan -nonewline
		}
		Else
		{
			Write-Host "PASS" -foreground Green -nonewline
		}
		Write-Host ": ${Line}"
	}
	ElseIf (${Line}.StartsWith("check_AUTOTESTS = "))
	{
		${InTests} = $TRUE
	}
}

Set-Location -Path ".."

Exit ${Result}
