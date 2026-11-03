# 5-Minute Demonstration & Viva Script
## SomaiyaSat & SomaiyaPod Operating Systems Mini-Project (KJS-SRS-01)
**Department of Information Technology — Somaiya Vidyavihar University**  
**Batch:** A-2 | **Target Video Duration:** ~4:30 – 5:00 Minutes

---

### Video & Presentation Plan
| Timestamp | Segment | On-Screen Action | Speaker |
|-----------|---------|------------------|---------|
| **0:00 – 0:40** | Introduction & Mission Problem Statement | Show Title Slide / Architecture Diagram | Student 1 (e.g. Omkar) |
| **0:40 – 1:45** | Exp 6: Priority Preemptive Scheduling with Aging | Terminal running `./exp6_scheduler` & Gantt chart | Student 1 (Omkar) |
| **1:45 – 2:40** | Exp 7: Telemetry Bounded-Buffer Concurrency | Terminal running `./exp7_sync` | Student 2 (Yash) |
| **2:40 – 3:35** | Exp 8: Flight Memory Paging & Segmentation MMU | Terminal running `./exp8_memory` | Student 3 (Kashvi) |
| **3:35 – 4:25** | Exp 10: Virtual Memory LRU Cache Replacement | Terminal running `./exp10_lru` | Student 4 (Mehul) |
| **4:25 – 5:00** | Conclusion & Submission Summary | Show Consolidated PDF & GitHub Repo | Lead / Team |

---

## 🎙️ Spoken Script

### 1. Introduction & Mission Problem Statement (0:00 – 0:40)
> *"Respected faculty and evaluators, good day!  
> We are Batch A-2: Yash Dhanawde, Kashvi Dhankhar, Omkar Dhatrak, and Mehul Dwari.  
> Today, we present our Operating Systems Mini-Project under the KJS-SRS-01 framework guided by Dr. Umesh Shinde: **Autonomous AI-Based Inter-Satellite Data Routing and Downlink Management for the SomaiyaSat and SomaiyaPod PocketQube Mission.**  
>
> SomaiyaSat operates in Low Earth Orbit with severe size, weight, and power limits. During short 5-to-10-minute ground passes, critical health beacons, voice, digital packets, and high-resolution Earth images all compete for a single shared radio downlink.  
> We solved this by mapping **four core Operating System fundamentals** directly into the satellite's software architecture."*

---

### 2. Experiment 6: Priority CPU Scheduling with Dynamic Aging (0:40 – 1:45)
**[ACTION: Open terminal and execute `./exp6_scheduler` or `./run_experiments.sh`.]**

> *"Let’s examine Experiment 6.  
> In our model, the half-duplex RF downlink represents the CPU core, and downlink tasks represent processes.  
> Priority 1 is reserved for TT&C Beacons—the satellite’s critical health heartbeat. Codec2 Voice is Priority 2, M17 Data is Priority 2 or 3, while SSTV Earth images are Priority 5 with long transmission bursts.  
>
> In a busy orbital pass without aging, notice how the SSTV image is continuously preempted and starves, waiting **21 full seconds**. If the satellite passes the horizon, that image is lost forever!  
>
> To solve this, we implemented **Preemptive Priority Scheduling with Dynamic Aging**.  
> Every 3 seconds a task waits in the ready queue, its effective priority improves by 1, with an aging floor capped at Priority 2 so it never overtakes emergency TT&C beacons.  
> As seen in the output table, SSTV’s maximum waiting time plummets from **21 seconds down to 14 seconds—a 33.3% latency reduction**—guaranteeing image completion before loss-of-signal, while TT&C beacons maintain **zero wait time**."*

---

### 3. Experiment 7: Process Synchronization & Mutex Bounded Buffer (1:45 – 2:40)
**[ACTION: Switch to or scroll to Experiment 7 output on terminal.]**

> *"Next is Experiment 7: Concurrency and Thread Synchronization.  
> SomaiyaSat has multiple onboard hardware subsystems: Electrical Power System (EPS), Onboard Computer (OBC), and Communications (COMM).  
> These sensors sample telemetry asynchronously as multi-threaded producers into a shared 4-slot circular ring buffer. A dedicated RF downlink thread acts as the consumer.  
>
> Without OS synchronization, race conditions cause buffer overflow and dropped telecommand acknowledgments.  
> Here we used **POSIX mutex locks (`std::mutex`) and condition variables (`notFull`, `notEmpty`)**.  
> In the live trace on screen, when the buffer reaches capacity (4/4 slots), producer threads safely block until the ground transmitter pops a packet. All 12 telemetry frames across EPS, OBC, and COMM are transmitted with 100% data integrity and zero race conditions."*

---

### 4. Experiment 8: Memory Management — Paging and Segmentation MMU (2:40 – 3:35)
**[ACTION: Scroll to Experiment 8 output on terminal.]**

> *"Experiment 8 models the satellite's flight computer memory architecture using Paging and Segmentation.  
> SomaiyaSat has a 64 KB logical address space with 1 KB page sizes translated by the Memory Management Unit into 16 KB of physical RAM frames.  
>
> In Part 1, the MMU translates logical addresses into physical addresses—for example, Logical `0x0200` correctly resolves to Frame 3 at Physical Address `0x0E00`. Swapped-out routines, like the SomaiyaPod burn-wire deployer firmware, correctly trigger a `PAGE FAULT` trap.  
>
> In Part 2, we simulate Segmentation with base and limit bounds checking. Critical flight control code is strictly isolated from experimental amateur payloads. When a simulated payload tries to access offset 1600 on a segment limit of 1536, the OS immediately triggers a **Segmentation Fault Trap**, preventing payload buffer overflows from crashing core satellite flight avionics."*

---

### 5. Experiment 10: Virtual Memory LRU Cache Replacement (3:35 – 4:25)
**[ACTION: Scroll to Experiment 10 output on terminal.]**

> *"Experiment 10 optimizes the onboard telemetry buffer cache using the **Least Recently Used (LRU)** page replacement algorithm.  
> Because flash memory writes consume significant battery power in space, keeping frequent data hot in RAM is essential.  
>
> We benchmarked a realistic 12-reference downlink stream under two RAM budget scenarios:  
> - With a tight budget of **3 physical frames**, the cache experienced 10 faults and 2 hits—a **16.67% hit ratio**.  
> - By scaling to **4 frames**, the cache achieved 4 hits—a **33.33% hit ratio**, which is a **100% relative improvement in cache efficiency!**  
> Critical TT&C telemetry headers remain cached in RAM, drastically reducing flash memory bus read cycles and extending mission battery life."*

---

### 6. Conclusion & Submission Deliverables (4:25 – 5:00)
**[ACTION: Show the 6-page PDF report and the GitHub repository link.]**

> *"To conclude:  
> Through these four integrated experiments, we proved that classical Operating Systems concepts—Preemptive Scheduling with Aging, Mutex Synchronization, Paging/Segmentation MMU protection, and LRU Caching—are essential for real-world autonomous aerospace systems.  
>
> All source code, Makefiles, automated bash harnesses, and the consolidated 6-page university report have been compiled and pushed to our official GitHub repository:  
> `github.com/omkardhatrak796-cpu/miniproj`.  
>
> Thank you!"*

---

### 💡 Quick Tips for the 5-Minute Video Recording
1. **Resolution:** Record your screen at 1080p (Full HD) so terminal text is clear.
2. **Terminal Theme:** Use a dark or high-contrast background with font size ~14–16 pt.
3. **Run Script:** You can run `./run_experiments.sh` to output all four experiments sequentially or run each executable one-by-one as you explain each slide.
4. **Link Sharing:** Upload the MP4 video to Google Drive or unlisted YouTube, make sure permissions are set to *"Anyone with the link can view"*, and paste the link into your report submission.
