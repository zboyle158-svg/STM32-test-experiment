$utf8 = [System.Text.UTF8Encoding]::new($false)
$gbk = [System.Text.Encoding]::GetEncoding(936)
$root = (Resolve-Path '..\Core').Path
foreach ($file in Get-ChildItem $root -Recurse -File | Where-Object { $_.Extension -in '.c','.h' }) {
    $old = [IO.File]::ReadAllText($file.FullName, $utf8)
    $new = [regex]::Replace($old, '(?s)/\*.*?\*/|//[^\r\n]*', { param($m)
        $v = $m.Value
        $han = [regex]::Matches($v, '[\u4e00-\u9fff]').Count
        $bad = [regex]::Matches($v, '[闂婃娊鍣烘径姘遍獓閻㈤潧]').Count
        if ($bad -gt 2 -and $bad -gt $han) { return [System.Text.Encoding]::UTF8.GetString($gbk.GetBytes($v)) }
        return $v
    })
    if ($new -cne $old) { [IO.File]::WriteAllText($file.FullName, $new, $utf8); Write-Output $file.FullName }
}
