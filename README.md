# SomaiyaSat & SomaiyaPod Mission - Operating Systems Mini-Project
## Framework: KJS-SRS-01 | Somaiya Vidyavihar University
### K J Somaiya School of Engineering / K J Somaiya Institute of Technology
**Department of Information Technology**  
**Academic Year:** 2026–2027 | **Semester:** V | **Batch:** A-2  

---

### Team Members
| Roll Number | Student Name | Assigned Core Modules |
|-------------|--------------|-----------------------|
| **16010424031** | Yash Dhanawde | Process Synchronization & Mutex Bounded-Buffer Concurrency |
| **16010424032** | Kashvi Dhankhar | Virtual Memory Management (Paging & Segmentation MMU) |
| **16010424033** | Omkar Dhatrak | Preemptive Priority Scheduling with Aging & Systems Lead |
| **16010424035** | Mehul Dwari | Virtual Memory LRU Cache Replacement & Benchmarking |

**Faculty Mentors:**
- **Dr. Umesh Shinde** (Associate Professor, Department of Basic Science & Humanities, KJSIT)
- **Dr. Shailesh Nikam** (Professor, Department of Mechanical Engineering, KJSSE)

---

## 🛰️ Project Overview & Aerospace Mapping

**SomaiyaSat** is an amateur radio PocketQube satellite released into Low-Earth Orbit (LEO) by its purpose-built orbital deployer **SomaiyaPod**. During brief 5-to-10-minute ground-station passes, the satellite must multiplex multiple data streams across a single shared half-duplex RF downlink carrier:
1. **TT&C Telemetry Beacon** (Priority 1 — Most urgent, health & survival telemetry)
2. **Codec2 Digital Voice** (Priority 2 — Live amateur voice repeater)
3. **M17 Digital Data Packets** (Priority 2/3 — Open amateur packet communication)
4. **SSTV Earth Observation Images** (Priority 4/5 — Slow-Scan TV images, large burst time)
5. **Housekeeping Logs** (Priority 5 — Battery, temperature, and bus health logs)

This project translates **four core Operating Systems concepts** directly into SomaiyaSat's onboard flight software stack:

| Exp # | OS Curriculum Experiment | SomaiyaSat Mission Role |
|-------|--------------------------|-------------------------|
| **06** | CPU Scheduling: Preemptive Priority with Aging | Radio downlink time-sharing among competing tasks with starvation prevention for SSTV imagery. |
| **07** | Process Synchronization: Bounded-Buffer with Mutex | Multi-threaded producer-consumer engine buffering concurrent EPS, OBC, and COMM sensor data into RF transmitter queues. |
| **08** | Memory Management: Paging & Segmentation MMU | Logical-to-physical address translation and segment memory protection preventing payload memory crashes from corrupting deployer burn-wire firmware. |
| **10** | Virtual Memory: LRU Page Replacement | Onboard RAM cache optimization preserving hot TT&C telemetry headers under strict 16 KB RAM limits. |

---

## 🛠️ Repository & File Structure

```
os mini p/
├── experiments/
│   ├── exp6_priority_aging_scheduler.cpp         # Exp 6: Priority scheduling + aging
│   ├── exp7_synchronization_producer_consumer.cpp # Exp 7: Multi-threaded bounded-buffer
│   ├── exp8_memory_management_paging_segmentation.cpp # Exp 8: Paging & segmentation MMU
│   ├── exp10_lru_page_replacement.cpp            # Exp 10: LRU cache replacement
│   └── Makefile                                  # Automated C++ build system
├── assets/
│   ├── somaiya_logo_left.jpg                     # Official university logo
│   ├── somaiya_logo_right.png                    # Somaiya Trust logo
│   └── somaiyasat_architecture.png               # End-to-end mission architecture
├── run_experiments.sh                            # One-click execution shell script
├── generate_pdf_report.py                        # Automated report compiler (HTML to PDF)
├── SomaiyaSat_OS_MiniProject_Report.html         # Consolidated report source
├── 16010424033_Omkar_OS_SomaiyaSat_Report.pdf    # Final consolidated submission PDF
└── README.md                                     # Documentation & execution guide
```

---

## 🚀 How to Build & Run on Linux / macOS

### Prerequisites
- **C++ Compiler:** `g++` or `clang++` supporting C++17
- **Make Utility:** `make`
- **POSIX Threads:** `pthread` (standard on modern Linux/macOS)
- **Python 3:** For compiling the PDF report (optional)

### 1. Execute All Experiments with One Command
```bash
chmod +x run_experiments.sh
./run_experiments.sh
```

### 2. Manual Compilation via Makefile
```bash
cd experiments
make all
make run-all
```

To clean generated binaries:
```bash
make clean
```

### 3. Running Individual Experiments
```bash
# Exp 6: Scheduling with Aging
./exp6_scheduler

# Exp 7: Mutex Bounded Buffer Telemetry
./exp7_sync

# Exp 8: Paging and Segmentation MMU
./exp8_memory

# Exp 10: LRU Page Replacement
./exp10_lru
```

---

## 📊 Summary of Experimental Results

### Experiment 06: Preemptive Priority Scheduling with Aging
- **Without Aging:** SSTV Image (P2) is starved by recurring high-priority telemetry passes, waiting **21 seconds** and delaying ground reception.
- **With Aging:** Every 3 seconds of queue waiting improves priority by 1 (down to a floor of Priority 2). P2's waiting time drops from **21s to 14s (33.3% reduction)**, ensuring full image transmission before ground horizon loss, while TT&C beacons maintain **0s latency**.

### Experiment 07: Bounded-Buffer Producer-Consumer
- Concurrently synchronizes 3 sensor producer threads (EPS, OBC, COMM) and 1 RF transmitter consumer thread.
- Circular 4-slot ring buffer protected by `std::mutex` and `std::condition_variable` (`notFull`, `notEmpty`).
- Output: 100% of 12 multi-mode telemetry frames transmitted with zero data corruption and zero race conditions.

### Experiment 08: Memory Management (Paging & Segmentation)
- Simulates 64 KB logical space with 1 KB page sizes translated by the MMU into 16 physical RAM frames.
- Identifies page hits, logical-to-physical address mappings, and page faults (e.g., swapped-out burn-wire firmware).
- Enforces segment boundary checks: accesses exceeding segment limits (e.g., offset 1600 on limit 1536) trigger a `SEGMENTATION FAULT` trap, protecting core flight code.

### Experiment 10: Virtual Memory LRU Replacement
- Tests telemetry buffer cache scaling across a 12-reference sequence during an orbital pass.
- **3 Frames:** 10 Page Faults, 2 Page Hits (**16.67% Hit Rate**).
- **4 Frames:** 8 Page Faults, 4 Page Hits (**33.33% Hit Rate** — **100% relative improvement**).
- Minimizes flash memory reads, significantly reducing satellite bus power consumption.

---

## 📄 Submission Package Checklist

- [x] **Consolidated Report PDF:** [`16010424033_Omkar_OS_SomaiyaSat_Report.pdf`](file:///Users/omkardhatrak/os%20mini%20p%20/16010424033_Omkar_OS_SomaiyaSat_Report.pdf) (Formatted per university submission requirements with student metadata, use case mapping, architecture diagrams, all 4 experiments, and performance comparisons).
- [x] **Source Code & Makefile:** Complete C++ implementations, clean build scripts, and execution harnesses.
- [x] **Reproducible Execution Script:** [`run_experiments.sh`](file:///Users/omkardhatrak/os%20mini%20p%20/run_experiments.sh) for Linux / macOS demonstration.
