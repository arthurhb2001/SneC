[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
$repo = 'libsdl-org/SDL'
$latest = (Invoke-WebRequest -Uri "https://api.github.com/repos/$repo/releases/latest" | ConvertFrom-Json)
$asset = $latest.assets | Where-Object { $_.name -like '*-VC.zip' } | Select-Object -First 1
if ($null -eq $asset) { exit 1 }

if (!(Test-Path 'SneC++\lib')) { mkdir 'SneC++\lib' }
if (!(Test-Path 'x64\Debug')) { mkdir 'x64\Debug' }

Invoke-WebRequest -Uri $asset.browser_download_url -OutFile 'sdl.zip'
Expand-Archive -Path 'sdl.zip' -DestinationPath 'sdl_temp' -Force
$sdlFolder = Get-ChildItem -Path 'sdl_temp' -Directory | Select-Object -First 1

Copy-Item -Path "sdl_temp\$($sdlFolder.Name)\include\*" -Destination 'SneC++\include' -Recurse -Force
Copy-Item -Path "sdl_temp\$($sdlFolder.Name)\lib\x64\SDL3.dll" -Destination 'x64\Debug' -Force
Copy-Item -Path "sdl_temp\$($sdlFolder.Name)\lib\x64\SDL3.lib" -Destination 'SneC++\lib' -Force
Copy-Item -Path "sdl_temp\$($sdlFolder.Name)\lib\x64\SDL3_test.lib" -Destination 'SneC++\lib' -Force

try {
    if (Test-Path 'sdl.zip') {
        [System.GC]::Collect()
        [System.GC]::WaitForPendingFinalizers()
        Remove-Item -Path 'sdl.zip' -Force
    }
    
    if (Test-Path 'sdl_temp') {
        [System.GC]::Collect()
        [System.GC]::WaitForPendingFinalizers()
        Remove-Item -Path 'sdl_temp' -Recurse -Force -ErrorAction Stop
    }
} catch {
    Write-Warning "Failed to remove temporary files: $_"
    Start-Sleep -Seconds 1
    try {
        Remove-Item -Path 'sdl_temp' -Recurse -Force -ErrorAction Stop
    } catch {
        Write-Error "Could not remove temporary files. Please delete 'sdl_temp' directory manually."
    }
}
