#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/in.h>
#include <bpf/bpf_helpers.h>

// VisaNet strict port enforcement
#define VISANET_SECURE_PORT 8443

SEC("xdp_router")
int xdp_packet_inspector(struct xdp_md *ctx) {
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;

    // 1. Parse Ethernet Header
    struct ethhdr *eth = data;
    if (data + sizeof(struct ethhdr) > data_end)
        return XDP_DROP;

    // Only process IPv4 traffic
    if (eth->h_proto != __constant_htons(ETH_P_IP))
        return XDP_PASS;

    // 2. Parse IP Header
    struct iphdr *iph = data + sizeof(struct ethhdr);
    if ((void *)iph + sizeof(struct iphdr) > data_end)
        return XDP_DROP;

    // Only process TCP traffic
    if (iph->protocol != IPPROTO_TCP)
        return XDP_PASS;

    // 3. Parse TCP Header
    struct tcphdr *tcph = (void *)iph + iph->ihl * 4;
    if ((void *)tcph + sizeof(struct tcphdr) > data_end)
        return XDP_DROP;

    // 4. Strict Port Enforcement
    // If it is destined for our routing process but not on the secure port, drop it instantly at the hardware level.
    if (tcph->dest != __constant_htons(VISANET_SECURE_PORT)) {
        // Log unauthorized access attempts conceptually
        bpf_printk("[eBPF-XDP] Unauthorized port access blocked. Packet dropped.\n");
        return XDP_DROP;
    }

    // Packet is valid, pass it up to the DPDK zero-copy rings
    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
