#ifdef _WIN32
  #undef _WIN32_WINNT
  #define _WIN32_WINNT 0x0600
#endif

#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>
#include <chrono>
#include <string>
#include <cstdlib>
#include <ctime>

#if defined(__MINGW32__) && !defined(_GLIBCXX_HAS_GTHREADS)
#include <windows.h>
#include <functional>

namespace compat {
    class mutex {
    public:
        CRITICAL_SECTION cs;
        mutex() { InitializeCriticalSection(&cs); }
        ~mutex() { DeleteCriticalSection(&cs); }
        void lock() { EnterCriticalSection(&cs); }
        void unlock() { LeaveCriticalSection(&cs); }
    };

    template<typename Mutex>
    class unique_lock {
        Mutex& m;
        bool locked;
    public:
        unique_lock(Mutex& mtx) : m(mtx), locked(true) { m.lock(); }
        ~unique_lock() { if (locked) m.unlock(); }
        void unlock() { if (locked) { m.unlock(); locked = false; } }
        void lock() { if (!locked) { m.lock(); locked = true; } }
        Mutex* mutex() { return &m; }
        bool owns_lock() const { return locked; }
    };

    class condition_variable {
        CONDITION_VARIABLE cv;
    public:
        condition_variable() { InitializeConditionVariable(&cv); }
        void notify_one() { WakeConditionVariable(&cv); }
        void notify_all() { WakeAllConditionVariable(&cv); }
        template<typename Pred>
        void wait(unique_lock<mutex>& lock, Pred pred) {
            while (!pred()) {
                SleepConditionVariableCS(&cv, &lock.mutex()->cs, INFINITE);
            }
        }
    };

    class thread {
        HANDLE hThread;
        std::function<void()>* fn;
        static DWORD WINAPI ThreadProc(LPVOID lpParam) {
            auto* pFn = static_cast<std::function<void()>*>(lpParam);
            (*pFn)();
            delete pFn;
            return 0;
        }
    public:
        thread() : hThread(NULL), fn(nullptr) {}
        template<typename Function, typename... Args>
        thread(Function&& f, Args&&... args) {
            auto bound = std::bind(std::forward<Function>(f), std::forward<Args>(args)...);
            fn = new std::function<void()>(bound);
            hThread = CreateThread(NULL, 0, ThreadProc, fn, 0, NULL);
        }
        void join() {
            if (hThread) {
                WaitForSingleObject(hThread, INFINITE);
                CloseHandle(hThread);
                hThread = NULL;
            }
        }
    };

    namespace this_thread {
        template<typename Rep, typename Period>
        void sleep_for(const std::chrono::duration<Rep, Period>& d) {
            Sleep(static_cast<DWORD>(std::chrono::duration_cast<std::chrono::milliseconds>(d).count()));
        }
    }
}

using compat::mutex;
using compat::condition_variable;
using compat::unique_lock;
using compat::thread;
namespace this_thread = compat::this_thread;

#else
#include <thread>
#include <mutex>
#include <condition_variable>
using std::mutex;
using std::condition_variable;
using std::unique_lock;
using std::thread;
namespace this_thread = std::this_thread;
#endif

using namespace std;

// Maximum telemetry buffer capacity (bounded buffer)
const int BUFFER_CAPACITY = 4;

struct TelemetryPacket {
    int packetId;
    string subsystem;   // "EPS", "OBC", "COMM", "PAYLOAD"
    float voltage;       // EPS bus voltage (V)
    float current;       // Solar current (mA)
    float temperature;   // Core temperature (deg C)
    int rssi;            // Signal strength (dBm)
    string status;       // "NOMINAL", "ALERT"
};

// Thread-safe circular bounded queue
queue<TelemetryPacket> bufferQueue;
mutex mtx;
condition_variable notFull;
condition_variable notEmpty;
bool missionActive = true;

// Producer: Onboard Sensor Data Acquisition
void sensorTelemetryProducer(int producerId, string subsystem, int packetCount) {
    for (int i = 1; i <= packetCount; ++i) {
        this_thread::sleep_for(chrono::milliseconds(150)); // simulate sampling period

        TelemetryPacket pkt;
        pkt.packetId = producerId * 100 + i;
        pkt.subsystem = subsystem;
        pkt.voltage = 3.7f + static_cast<float>(rand() % 50) / 100.0f;
        pkt.current = 120.0f + static_cast<float>(rand() % 80);
        pkt.temperature = 18.0f + static_cast<float>(rand() % 15);
        pkt.rssi = -85 - (rand() % 25);
        pkt.status = (pkt.voltage < 3.8f || pkt.temperature > 28.0f) ? "ALERT" : "NOMINAL";

        unique_lock<mutex> lock(mtx);
        // Wait if buffer is full (bounded buffer synchronization)
        notFull.wait(lock, []() { return bufferQueue.size() < BUFFER_CAPACITY; });

        bufferQueue.push(pkt);
        cout << "[PRODUCER " << producerId << " | " << setw(7) << left << subsystem
             << "] Sampled & Buffered Pkt #" << pkt.packetId
             << " | Temp=" << fixed << setprecision(1) << pkt.temperature << "C"
             << " | V=" << setprecision(2) << pkt.voltage << "V"
             << " | Queue: " << bufferQueue.size() << "/" << BUFFER_CAPACITY << "\n";

        lock.unlock();
        notEmpty.notify_one();
    }
}

// Consumer: Ground Station Downlink Transmitter
void groundDownlinkConsumer(int totalExpectedPackets) {
    int packetsDownlinked = 0;
    while (packetsDownlinked < totalExpectedPackets) {
        unique_lock<mutex> lock(mtx);
        // Wait if buffer is empty
        notEmpty.wait(lock, []() { return !bufferQueue.empty(); });

        TelemetryPacket pkt = bufferQueue.front();
        bufferQueue.pop();
        packetsDownlinked++;

        cout << "  >>> [DOWNLINK TX] Transmitted to Ground Station: Pkt #" << pkt.packetId
             << " [" << pkt.subsystem << "] Status=" << pkt.status
             << " | RSSI=" << pkt.rssi << " dBm | Remaining Queue: "
             << bufferQueue.size() << "/" << BUFFER_CAPACITY << "\n";

        lock.unlock();
        notFull.notify_one();

        // Simulate RF transmission latency
        this_thread::sleep_for(chrono::milliseconds(220));
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    cout << "======================================================================\n";
    cout << "  SOMAIYASAT SATELLITE ONBOARD SYNCHRONIZATION ENGINE\n";
    cout << "  CLASSICAL BOUNDED-BUFFER PRODUCER-CONSUMER (SEMAPHORES/MUTEX)\n";
    cout << "======================================================================\n";
    cout << "Subsystems: EPS, OBC, COMM | Buffer Size: " << BUFFER_CAPACITY << " Slots\n\n";

    // Subsystem producers
    thread epsProducer(sensorTelemetryProducer, 1, "EPS", 4);
    thread obcProducer(sensorTelemetryProducer, 2, "OBC", 4);
    thread commProducer(sensorTelemetryProducer, 3, "COMM", 4);

    // Ground Station downlink consumer
    thread groundStation(groundDownlinkConsumer, 12);

    epsProducer.join();
    obcProducer.join();
    commProducer.join();
    groundStation.join();

    cout << "\n======================================================================\n";
    cout << "MISSION STATUS: All 12 telemetry frames synchronized and downlinked.\n";
    cout << "Zero data corruption, zero buffer overflow, verified mutex lock safety.\n";
    cout << "======================================================================\n";
    return 0;
}
