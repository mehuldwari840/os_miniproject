#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

// SomaiyaSat Onboard Flight Computer Memory Specification:
// Page Size = 1 KB (1024 Bytes) = 2^10 bytes (Offset bits = 10)
// Virtual Address Space = 64 KB (64 Pages, 16-bit virtual address)
// Physical RAM Capacity = 16 Frames (16 KB Physical Memory, 14-bit physical address)
const int PAGE_SIZE = 1024;
const int OFFSET_BITS = 10;
const int NUM_FRAMES = 16;

struct PageEntry {
    int pageNum;
    int frameNum;
    bool valid;
    string allocatedTask;
};

// Segment Table Entry for SomaiyaSat Memory Segments
struct SegmentEntry {
    int segmentId;
    string name;
    int baseAddress;
    int limit; // size in bytes
};

void runPagingSimulation() {
    cout << "\n======================================================================\n";
    cout << "  PART 1: SOMAIYASAT PAGING MEMORY ARCHITECTURE & MMU TRANSLATION\n";
    cout << "======================================================================\n";
    cout << "Page Size: " << PAGE_SIZE << " Bytes (1 KB) | Offset: " << OFFSET_BITS << " bits\n";
    cout << "Physical Frames: " << NUM_FRAMES << " Frames (16 KB Flight Computer RAM)\n\n";

    // Setup Page Table for Downlink Tasks
    vector<PageEntry> pageTable = {
        {0, 3,  true,  "TT&C Beacon Buffer"},
        {1, 7,  true,  "Codec2 Voice Frame Buffer"},
        {2, 12, true,  "M17 Data Packet Ring"},
        {3, 1,  true,  "SSTV High-Res Tile Cache"},
        {4, 15, true,  "Housekeeping & EPS Telemetry"},
        {5, -1, false, "SomaiyaPod Burn-Wire Firmware (Swapped Out)"},
        {6, 5,  true,  "AI Decision Weight Matrix"}
    };

    cout << "--- PAGE TABLE (OBC MMU) ---\n";
    cout << left << setw(8) << "Page #"
         << setw(10) << "Frame #"
         << setw(10) << "Status"
         << setw(35) << "Task Payload Description" << "\n";
    cout << string(65, '-') << "\n";
    for (const auto& pe : pageTable) {
        cout << left << setw(8) << pe.pageNum
             << setw(10) << (pe.valid ? to_string(pe.frameNum) : "PAGE-FAULT")
             << setw(10) << (pe.valid ? "VALID" : "INVALID")
             << setw(35) << pe.allocatedTask << "\n";
    }

    // Translate incoming logical addresses
    vector<int> testAddresses = {512, 1200, 2048, 3500, 5200, 6500};

    cout << "\n--- MMU ADDRESS TRANSLATION (LOGICAL -> PHYSICAL) ---\n";
    cout << left << setw(18) << "Logical Addr (Hex)"
         << setw(10) << "Page #"
         << setw(10) << "Offset"
         << setw(10) << "Frame #"
         << setw(20) << "Physical Addr (Hex)"
         << setw(15) << "MMU Status" << "\n";
    cout << string(85, '-') << "\n";

    for (int logical : testAddresses) {
        int pageNum = logical / PAGE_SIZE;
        int offset = logical % PAGE_SIZE;

        if (pageNum < (int)pageTable.size() && pageTable[pageNum].valid) {
            int frameNum = pageTable[pageNum].frameNum;
            int physicalAddr = (frameNum * PAGE_SIZE) + offset;
            cout << "0x" << hex << uppercase << setw(14) << left << logical << dec
                 << setw(10) << pageNum
                 << setw(10) << offset
                 << setw(10) << frameNum
                 << "0x" << hex << uppercase << setw(16) << left << physicalAddr << dec
                 << setw(15) << "TRANSLATED" << "\n";
        } else {
            cout << "0x" << hex << uppercase << setw(14) << left << logical << dec
                 << setw(10) << pageNum
                 << setw(10) << offset
                 << setw(10) << "-"
                 << setw(18) << "TRAP / DISK"
                 << setw(15) << "PAGE FAULT" << "\n";
        }
    }
}

void runSegmentationSimulation() {
    cout << "\n======================================================================\n";
    cout << "  PART 2: SOMAIYASAT SEGMENTATION & ACCESS PROTECTION\n";
    cout << "======================================================================\n";

    vector<SegmentEntry> segmentTable = {
        {0, "Flight Code Segment (RX-Only)", 0x1000, 4096},
        {1, "AI Weight Model Segment",        0x2000, 2048},
        {2, "Amateur RF Downlink Stack",      0x3000, 1536},
        {3, "Telemetry Heap & Buffers",       0x4000, 3072}
    };

    cout << "--- SEGMENT TABLE ---\n";
    cout << left << setw(12) << "Segment ID"
         << setw(35) << "Segment Name"
         << setw(16) << "Base Address"
         << setw(12) << "Limit (Bytes)" << "\n";
    cout << string(75, '-') << "\n";
    for (const auto& seg : segmentTable) {
        cout << left << setw(12) << seg.segmentId
             << setw(35) << seg.name
             << "0x" << hex << uppercase << setw(14) << left << seg.baseAddress << dec
             << setw(12) << seg.limit << "\n";
    }

    struct AccessRequest { int segId; int offset; };
    vector<AccessRequest> requests = {
        {0, 1024},  // Valid inside code
        {1, 2040},  // Valid inside AI weights
        {2, 1600},  // Illegal: exceeds limit 1536 -> Trap!
        {3, 512}    // Valid telemetry access
    };

    cout << "\n--- SEGMENT BOUNDS CHECKING & ADDRESS RESOLUTION ---\n";
    cout << left << setw(12) << "Segment ID"
         << setw(10) << "Offset"
         << setw(12) << "Limit"
         << setw(20) << "Physical Addr"
         << setw(20) << "Bounds Status" << "\n";
    cout << string(75, '-') << "\n";

    for (const auto& req : requests) {
        if (req.segId < (int)segmentTable.size()) {
            const auto& seg = segmentTable[req.segId];
            if (req.offset < seg.limit) {
                int physical = seg.baseAddress + req.offset;
                cout << left << setw(12) << req.segId
                     << setw(10) << req.offset
                     << setw(12) << seg.limit
                     << "0x" << hex << uppercase << setw(16) << left << physical << dec
                     << setw(20) << "VALID (PASSED)" << "\n";
            } else {
                cout << left << setw(12) << req.segId
                     << setw(10) << req.offset
                     << setw(12) << seg.limit
                     << setw(18) << "-"
                     << setw(20) << "SEGMENTATION FAULT" << "\n";
            }
        }
    }
}

int main() {
    cout << "======================================================================\n";
    cout << "  SOMAIYASAT SATELLITE ONBOARD FLIGHT COMPUTER MEMORY MANAGEMENT\n";
    cout << "  EXPERIMENT 8: SIMULATION OF PAGING AND SEGMENTATION WITH MMU\n";
    cout << "======================================================================\n";

    runPagingSimulation();
    runSegmentationSimulation();

    cout << "\n======================================================================\n";
    cout << "CONCLUSION: Successfully simulated Paging address translation and\n";
    cout << "Segmentation boundary protection for SomaiyaSat onboard flight memory.\n";
    cout << "======================================================================\n";
    return 0;
}
