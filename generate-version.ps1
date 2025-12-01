
# Add native so 32-bit PS (as called from VS2019) can use 64-bit wsl.exe
$env:PATH = "$env:SystemRoot\sysnative;$env:PATH"

function Get-GitCommit {
    try {
        # Try Windows Git first
        git describe --always --dirty 2>$null
    } catch {
        try {
            # Try WSL Git
            wsl git describe --always --dirty 2>$null
        } catch {
            return "unknown"
        }
    }
}

$commit = Get-GitCommit
$buildTime = (Get-Date).ToUniversalTime().ToString("yyyy-MM-ddTHH:mm:ssZ")

@"
#pragma once

#define GIT_COMMIT "$commit"
#define BUILD_TIME_UTC "$buildTime"
"@ | Out-File -Encoding UTF8 -Force version.h
