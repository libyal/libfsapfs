# Script that runs the tests
#
# Version: 20161106

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
		If (-Not (${Line}.EndsWith(".sh")))
		{
			Continue
		}
		${Line} = ${Line}.Substring(0, ${Line}.Length - 3)
		${Line} = ".\${Line}.ps1"

		Try
		{
			Invoke-Expression ${Line}

			If (${LastExitCode} -ne ${ExitSuccess})
			{
				$Result = ${ExitFailure}

				Write-Host "FAIL" -foreground Red -nonewline
			}
			Else
			{
				Write-Host "PASS" -foreground Green -nonewline
			}
		}
		Catch
		{
			Write-Host "SKIP" -foreground Cyan -nonewline
		}
		Write-Host ": ${Line}"
	}
	ElseIf (${Line}.StartsWith("TESTS = "))
	{
		${InTests} = $TRUE
	}
}

Set-Location -Path ".."

Exit ${Result}
