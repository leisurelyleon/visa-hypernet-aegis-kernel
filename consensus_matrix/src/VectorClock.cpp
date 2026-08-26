#pragma once
#include <unordered_map>
#include <string>
#include <algorithm>
#include <iostream>

namespace visa::aegis {

class VectorClock {
private:
    std::unordered_map<std::string, uint64_t> clock_map_;
    std::string local_node_id_;

public:
    explicit VectorClock(const std::string& node_id) : local_node_id_(node_id) {
        clock_map_[local_node_id_] = 0;
    }

    // Increment the logical clock for a local event
    void Tick() {
        clock_map_[local_node_id_]++;
    }

    // Merge incoming state from another global region
    void Merge(const std::unordered_map<std::string, uint64_t>& remote_clock) {
        for (const auto& [node, timestamp] : remote_clock) {
            clock_map_[node] = std::max(clock_map_[node], timestamp);
        }
    }

    // Determines if this transaction causally happened BEFORE another transaction
    bool HappensBefore(const VectorClock& other) const {
        bool strictly_smaller = false;
        for (const auto& [node, timestamp] : clock_map_) {
            auto it = other.clock_map_.find(node);
            uint64_t other_timestamp = (it != other.clock_map_.end()) ? it->second : 0;
            
            if (timestamp > other_timestamp) return false;
            if (timestamp < other_timestamp) strictly_smaller = true;
        }
        return strictly_smaller;
    }

    void PrintClock() const {
        std::cout << "[VECTOR-CLOCK] " << local_node_id_ << " State: { ";
        for (const auto& [node, timestamp] : clock_map_) {
            std::cout << node << ":" << timestamp << " ";
        }
        std::cout << "}\n";
    }
};

} // namespace visa::aegis
