#pragma once
#include "VectorClock.cpp"
#include <unordered_set>
#include <shared_mutex>
#include <string>
#include <iostream>

namespace visa::aegis {

class DoubleSpendDetector {
private:
    // Tracks active transaction IDs within the current clearing window
    std::unordered_set<std::string> active_transaction_pool_;
    mutable std::shared_mutex pool_mutex_;

public:
    DoubleSpendDetector() {
        std::cout << "[CONSENSUS-FIREWALL] Cross-Continent Double Spend Detector Online.\n";
    }

    bool IsValidTransaction(const std::string& tx_hash, VectorClock& local_clock) {
        std::unique_lock lock(pool_mutex_);
        
        // If the hash already exists, this is a double-spend anomaly!
        if (active_transaction_pool_.find(tx_hash) != active_transaction_pool_.end()) {
            std::cout << "[CONSENSUS-FIREWALL] 🚨 ANOMALY DETECTED: Double-Spend attempt for TX " << tx_hash << "\n";
            return false;
        }

        // Register the transaction and advance the local logical clock
        active_transaction_pool_.insert(tx_hash);
        local_clock.Tick();
        
        return true;
    }

    void EvictClearedTransactions() {
        // Conceptually runs in a background thread to clear memory for settled batches
        std::unique_lock lock(pool_mutex_);
        active_transaction_pool_.clear();
    }
};

} // namespace visa::aegis
