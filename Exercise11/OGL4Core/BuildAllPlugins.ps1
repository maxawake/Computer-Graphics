[CmdletBinding()]
Param(
    [ValidateSet("Debug", "Release", "All")]
    [string]$configuration = "Debug",
    [ValidateSet("Build", "Rebuild", "Clean")]
    [string]$action = "Build",
    [ValidateSet("Win32", "x64", "All")]
    [string]$platform = "Win32",
    [string]$MSBuildDir = "C:\Windows\Microsoft.NET\Framework\v4.0.30319")

PROCESS {
If (! (Test-Path $MSBuildDir)) {
    Write-Error "Cannot find MSBuild Directory, specify via -MSBuildDir"
    Exit
}
Write-Output "${action}ing $configuration for ${platform}:"

#for /r Plugins %%i in (*.sln) do MSBuild %%i /t:%action% /p:Configuration=%conf% /p:Platform=%platf%
$solutions = gci Plugins -Recurse -Include "*.sln" -Exclude "Template.sln"

$totalwarn = 0
$totalerr = 0

if ($configuration -eq "All") {
    $confs = @("Debug", "Release")
} else {
    $confs = @($configuration)
}

if ($platform -eq "All") {
    $plats = @("Win32", "x64")
} else {
    $plats = @($platform)
}

foreach ($sol in $solutions) {
    foreach ($conf in $confs) {
        foreach ($plat in $plats) {
            Write-Host -NoNewline "${action}ing ${sol} ${conf}|${plat}:  "
            $localwarn = 0
            $localerr = 0
            [String]$res = &"$MSBuildDir\MSBuild.exe" $sol /t:$action /p:Configuration=$conf /p:Platform=$plat
            Write-Verbose -Message $res
            [String]$line = $res -match "\s+(\d+)\s+Warning\(s\)"
            $null = $line -match "\s+(\d+)\s+Warning\(s\)"
            if ($Matches.Count -gt 1) {
                $localwarn = $Matches[1]
            }
            $Matches.Clear()
            [String]$line = $res -match "\s+(\d+)\s+Error\(s\)"
            $null = $line -match "\s+(\d+)\s+Error\(s\)"
            if ($Matches.Count -gt 1) {
                $localerr = $Matches[1]
            }
            $Matches.Clear()
            if ($localerr -gt 0) {
                Write-Error "$localwarn Warning(s), $localerr Error(s)"
            } elseif ($localwarn -gt 0) {
                Write-Warning "$localwarn Warning(s), $localerr Error(s)"
            } else {
                Write-Output "$localwarn Warning(s), $localerr Error(s)"
            }
            $totalwarn += $localwarn
            $totalerr += $localerr
            #Write-Output $res
        }
    }
}

Write-Output ""
Write-Output "Summary:"
Write-Output "--------"
Write-Output "$totalwarn Warning(s), $totalerr Error(s)"
}