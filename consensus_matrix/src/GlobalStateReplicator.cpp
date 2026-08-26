#include "VectorClock.cpp"
#include "DoubleSpendDetector.cpp"
#include "RaftCoordinator.cpp"
#include <iostream>
#include <thread>
#include <chrono>

using namespace visa::aegis;

int main() {
    std::cout << "=========================================================\n";
    std::cout << "    VISA HYPERNET-AEGIS: DISTRIBUTED STATE KERNEL        \n";
    std::cout << "=========================================================\n";

    // 1. Initialize local logical clocks and firewall
    VectorClock local_region_clock("US-EAST-1");
    DoubleSpendDetector firewall;
    RaftCoordinator consensus_engine;

    local_region_clock.PrintClock();

    // 2. Simulate processing incoming global transactions
    std::vector<std::string> simulated_traffic = {
        "TX_HASH_88A9F1",
        "TX_HASH_4B2C99",
        "TX_HASH_88A9F1" // Intentional Double-Spend Replay
    };

    for (const auto& tx : simulated_traffic) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::cout << "\n[STATE-KERNEL] Processing inbound global state sync for: " << tx << "\n";

        // Step 1: Validate against Double Spends using Causal Ordering
        if (firewall.IsValidTransaction(tx, local_region_clock)) {
            
            // Step 2: Ensure cross-continent quorum is met before returning authorization
            if (consensus_engine.AchieveFastPathQuorum(tx)) {
                std::cout << "[STATE-KERNEL] Network sync complete. Ledger updated.\n";
                local_region_clock.PrintClock();
            }
        }
    }

    std::cout << "\n=========================================================\n";
    std::cout << " STATE KERNEL SYNCHRONIZATION COMPLETE.\n";
    std::cout << "=========================================================\n";

    return 0;
}
