#include <iostream>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>

using namespace std;

// Payload packet reference sequence representing multi-mode amateur transmissions
// P1: TT&C (Page 1)
// P2: SSTV Tile A (Page 2)
// P3: Codec2 Voice (Page 3)
// P4: SSTV Tile B (Page 4)
// P5: M17 Data (Page 5)
struct PageRef {
    int pageId;
    string taskName;
};

// Simulate LRU Page Replacement
void simulateLRU(const vector<PageRef>& refs, int capacity) {
    cout << "\n======================================================================\n";
    cout << "  SOMAIYASAT LRU CACHE SIMULATION (FRAME CAPACITY = " << capacity << " FRAMES)\n";
    cout << "======================================================================\n";

    vector<int> frames; // current frames in RAM
    unordered_map<int, int> lastUsedTime; // pageId -> timestamp
    int pageFaults = 0;
    int pageHits = 0;

    cout << left << setw(6) << "Time"
         << setw(8) << "Ref P#"
         << setw(24) << "Task Payload"
         << setw(12) << "Access"
         << setw(26) << "Physical Frames in RAM"
         << setw(16) << "Evicted Page" << "\n";
    cout << string(92, '-') << "\n";

    for (int t = 0; t < (int)refs.size(); ++t) {
        int page = refs[t].pageId;
        string task = refs[t].taskName;
        bool isHit = false;
        int evicted = -1;

        // Check if page already in frames
        auto it = find(frames.begin(), frames.end(), page);
        if (it != frames.end()) {
            isHit = true;
            pageHits++;
        } else {
            pageFaults++;
            // If cache full, evict LRU page
            if ((int)frames.size() >= capacity) {
                int lruPage = frames[0];
                int minTime = lastUsedTime[lruPage];
                for (int p : frames) {
                    if (lastUsedTime[p] < minTime) {
                        minTime = lastUsedTime[p];
                        lruPage = p;
                    }
                }
                evicted = lruPage;
                frames.erase(find(frames.begin(), frames.end(), lruPage));
            }
            frames.push_back(page);
        }
        lastUsedTime[page] = t;

        // Format frame string
        string frameStr = "[ ";
        for (int p : frames) {
            frameStr += "P" + to_string(p) + " ";
        }
        frameStr += "]";

        cout << left << setw(6) << t
             << setw(8) << ("P" + to_string(page))
             << setw(24) << task
             << setw(12) << (isHit ? "HIT" : "FAULT (MISS)")
             << setw(26) << frameStr
             << setw(16) << (evicted != -1 ? ("Evicted P" + to_string(evicted)) : "-") << "\n";
    }

    int totalAccesses = refs.size();
    double faultRatio = (double)pageFaults / totalAccesses * 100.0;
    double hitRatio = (double)pageHits / totalAccesses * 100.0;

    cout << string(92, '-') << "\n";
    cout << "PERFORMANCE SUMMARY (Capacity = " << capacity << " Frames):\n";
    cout << "Total Memory Accesses : " << totalAccesses << "\n";
    cout << "Page Faults           : " << pageFaults << " (" << fixed << setprecision(2) << faultRatio << "%)\n";
    cout << "Page Hits             : " << pageHits << " (" << setprecision(2) << hitRatio << "%)\n";
}

int main() {
    cout << "======================================================================\n";
    cout << "  SOMAIYASAT ONBOARD PACKET CACHE MANAGEMENT (LRU PAGE REPLACEMENT)\n";
    cout << "  EXPERIMENT 10: VIRTUAL MEMORY LRU PAGE REPLACEMENT ALGORITHM\n";
    cout << "======================================================================\n";

    // Sequence of downlink transmission buffer accesses during a ground-station pass
    vector<PageRef> stream = {
        {1, "TT&C Beacon"},
        {2, "SSTV Tile #1"},
        {3, "Codec2 Voice"},
        {1, "TT&C Beacon"},
        {4, "SSTV Tile #2"},
        {2, "SSTV Tile #1"},
        {5, "M17 Data Packets"},
        {3, "Codec2 Voice"},
        {1, "TT&C Beacon"},
        {2, "SSTV Tile #1"},
        {6, "Housekeeping Log"},
        {1, "TT&C Beacon"}
    };

    // Run with small onboard frame budget (3 frames)
    simulateLRU(stream, 3);

    // Run with expanded frame budget (4 frames) to analyze cache scaling
    simulateLRU(stream, 4);

    cout << "\n======================================================================\n";
    cout << "CACHE BEHAVIORAL ANALYSIS:\n";
    cout << "Expanding frame capacity from 3 to 4 reduces page faults significantly,\n";
    cout << "keeping frequent TT&C and Codec2 audio packets hot in satellite RAM.\n";
    cout << "======================================================================\n";
    return 0;
}
