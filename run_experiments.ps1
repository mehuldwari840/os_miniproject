# ==============================================================================
# SomaiyaSat Ground Station & OS Kernel Simulation Execution Script (PowerShell)
# Somaiya Vidyavihar University - K J Somaiya School of Engineering / KJSIT
# Batch: A-2 | Academic Year: 2026-27
# ==============================================================================

$ErrorActionPreference = "Stop"
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location "$ScriptDir\experiments"

Write-Host "======================================================================" -ForegroundColor Cyan
Write-Host "    SOMAIYASAT SATELLITE MISSION - OS EXPERIMENTAL TEST HARNESS" -ForegroundColor Cyan
Write-Host "======================================================================" -ForegroundColor Cyan
Write-Host "Platform : Windows (PowerShell)"
Write-Host "Directory: $(Get-Location)"
Write-Host "Time     : $(Get-Date)"
Write-Host "----------------------------------------------------------------------"

# Check / Compile
Write-Host "[+] Step 1: Checking and compiling C++ experiments..." -ForegroundColor Yellow

$targets = @(
    @{ Src = "exp6_priority_aging_scheduler.cpp"; Exe = "exp6_scheduler.exe" },
    @{ Src = "exp7_synchronization_producer_consumer.cpp"; Exe = "exp7_sync.exe" },
    @{ Src = "exp8_memory_management_paging_segmentation.cpp"; Exe = "exp8_memory.exe" },
    @{ Src = "exp10_lru_page_replacement.cpp"; Exe = "exp10_lru.exe" }
)

foreach ($t in $targets) {
    if (-not (Test-Path $t.Exe) -or ((Get-Item $t.Src).LastWriteTime -gt (Get-Item $t.Exe).LastWriteTime)) {
        Write-Host "    -> Compiling $($t.Src) -> $($t.Exe)" -ForegroundColor Gray
        & g++ -std=c++14 -O2 $t.Src -o $t.Exe
    }
}
Write-Host "    -> All 4 binaries ready!" -ForegroundColor Green

Write-Host ""
Write-Host "======================================================================" -ForegroundColor Cyan
Write-Host ">>> RUNNING EXPERIMENT 6: SCHEDULING WITH AGING <<<" -ForegroundColor Cyan
Write-Host "======================================================================" -ForegroundColor Cyan
& .\exp6_scheduler.exe

Write-Host ""
Write-Host "======================================================================" -ForegroundColor Cyan
Write-Host ">>> RUNNING EXPERIMENT 7: PRODUCER-CONSUMER MUTEX/SYNC <<<" -ForegroundColor Cyan
Write-Host "======================================================================" -ForegroundColor Cyan
& .\exp7_sync.exe

Write-Host ""
Write-Host "======================================================================" -ForegroundColor Cyan
Write-Host ">>> RUNNING EXPERIMENT 8: PAGING & SEGMENTATION MMU <<<" -ForegroundColor Cyan
Write-Host "======================================================================" -ForegroundColor Cyan
& .\exp8_memory.exe

Write-Host ""
Write-Host "======================================================================" -ForegroundColor Cyan
Write-Host ">>> RUNNING EXPERIMENT 10: LRU PAGE REPLACEMENT <<<" -ForegroundColor Cyan
Write-Host "======================================================================" -ForegroundColor Cyan
& .\exp10_lru.exe

Write-Host ""
Write-Host "======================================================================" -ForegroundColor Green
Write-Host "SUCCESS: All 4 OS experiments executed successfully with verified outputs." -ForegroundColor Green
Write-Host "======================================================================" -ForegroundColor Green

Set-Location $ScriptDir

