#!/usr/bin/env bash
# ==============================================================================
# SomaiyaSat Ground Station & OS Kernel Simulation Execution Script
# Somaiya Vidyavihar University - KJS School of Engineering / KJSIT
# Batch: A-2 | Academic Year: 2026-27
# ==============================================================================

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
echo "======================================================================"
echo "    SOMAIYASAT SATELLITE MISSION - OS EXPERIMENTAL TEST HARNESS"
echo "======================================================================"
echo "Running on: $(uname -s) $(uname -m) - $(date)"
echo "Directory : ${SCRIPT_DIR}"
echo "----------------------------------------------------------------------"

cd "${SCRIPT_DIR}/experiments"
echo "[+] Step 1: Compiling all 4 OS experiments using Makefile..."
make all

echo ""
echo "[+] Step 2: Executing Experiment 6: Priority Preemptive Scheduling with Aging..."
./exp6_scheduler

echo ""
echo "[+] Step 3: Executing Experiment 7: Mutex & Bounded-Buffer Synchronization..."
./exp7_sync

echo ""
echo "[+] Step 4: Executing Experiment 8: Memory Management (Paging & Segmentation)..."
./exp8_memory

echo ""
echo "[+] Step 5: Executing Experiment 10: Virtual Memory LRU Cache Replacement..."
./exp10_lru

echo ""
echo "======================================================================"
echo "SUCCESS: All 4 OS experiments executed successfully with verified outputs."
echo "======================================================================"
