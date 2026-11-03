@echo off
echo ======================================================================
echo     SOMAIYASAT SATELLITE MISSION - OS EXPERIMENTAL TEST HARNESS
echo ======================================================================
cd /d "%~dp0experiments"

echo [+] Checking and running binaries...
echo.
echo ======================================================================
echo >>> RUNNING EXPERIMENT 6: SCHEDULING WITH AGING <<<
echo ======================================================================
exp6_scheduler.exe
echo.
echo ======================================================================
echo >>> RUNNING EXPERIMENT 7: PRODUCER-CONSUMER MUTEX/SYNC <<<
echo ======================================================================
exp7_sync.exe
echo.
echo ======================================================================
echo >>> RUNNING EXPERIMENT 8: PAGING ^& SEGMENTATION MMU <<<
echo ======================================================================
exp8_memory.exe
echo.
echo ======================================================================
echo >>> RUNNING EXPERIMENT 10: LRU PAGE REPLACEMENT <<<
echo ======================================================================
exp10_lru.exe
echo.
echo ======================================================================
echo SUCCESS: All 4 OS experiments executed successfully with verified outputs.
echo ======================================================================
pause

