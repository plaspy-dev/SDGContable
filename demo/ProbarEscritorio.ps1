param(
    [string]$BinaryDirectory = 'bin\Release',
    [ValidateRange(1,65535)][int]$Port = 33067
)

$ErrorActionPreference = 'Stop'
$binaries = (Resolve-Path -LiteralPath $BinaryDirectory).ProviderPath
$profile = 'Software\SDG\SDGContable'
$currentUser = [Microsoft.Win32.Registry]::CurrentUser
$machine = [Microsoft.Win32.RegistryKey]::OpenBaseKey(
    [Microsoft.Win32.RegistryHive]::LocalMachine, [Microsoft.Win32.RegistryView]::Registry32)
$oldUserProfile = $currentUser.OpenSubKey($profile)
$installedProfile = $machine.OpenSubKey($profile)
try {
    if ($oldUserProfile -or $installedProfile) {
        throw 'Use a Windows test profile without an existing SDGContable installation.'
    }
}
finally {
    if ($oldUserProfile) { $oldUserProfile.Dispose() }
    if ($installedProfile) { $installedProfile.Dispose() }
    $machine.Dispose()
}

$moduleNames = 'SDGContable.exe', 'LAppc.dll', 'appt.dll', 'Componentes.dll', 'Conector.dll', 'IOFile.dll'
$moduleFiles = foreach ($name in $moduleNames) {
    Get-Item -LiteralPath (Join-Path $binaries $name)
}
Get-Item -LiteralPath (Join-Path $binaries 'libmysql.dll') | Out-Null
$mark = [guid]::NewGuid().ToString('N')
$root = $currentUser.CreateSubKey($profile)
$root.SetValue('__demo_profile', $mark)

try {
    $server = $root.CreateSubKey('conector\servidor0')
    try {
        $server.SetValue('servidor', '127.0.0.1')
        $server.SetValue('db', 'sdgcontable_demo')
        $server.SetValue('puerto', $Port, [Microsoft.Win32.RegistryValueKind]::DWord)
        $server.SetValue('incrustado', 0, [Microsoft.Win32.RegistryValueKind]::DWord)
    }
    finally { $server.Dispose() }
    $checksums = $root.CreateSubKey('llaves')
    try {
        foreach ($module in $moduleFiles) {
            $digest = [System.Security.Cryptography.IncrementalHash]::CreateHash(
                [System.Security.Cryptography.HashAlgorithmName]::MD5)
            try {
                $digest.AppendData([Text.Encoding]::ASCII.GetBytes('10rd'))
                $digest.AppendData([IO.File]::ReadAllBytes($module.FullName))
                $digest.AppendData([Text.Encoding]::ASCII.GetBytes('10rdJuHU'))
                $hex = [Convert]::ToHexString($digest.GetHashAndReset()).ToLowerInvariant()
                $checksums.SetValue($hex, $module.FullName.ToLowerInvariant())
            }
            finally { $digest.Dispose() }
        }
    }
    finally { $checksums.Dispose() }
    $desktop = Start-Process -FilePath (Join-Path $binaries 'SDGContable.exe') -WorkingDirectory $binaries -PassThru
    $desktop.WaitForExit()
}
finally {
    $owned = $root.GetValue('__demo_profile') -ceq $mark
    $root.Dispose()
    if ($owned) { $currentUser.DeleteSubKeyTree($profile) }
}
