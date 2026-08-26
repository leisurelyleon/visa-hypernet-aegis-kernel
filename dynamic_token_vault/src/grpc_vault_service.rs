//! gRPC Interface for the Dynamic Token Vault

use std::time::Duration;
use tokio::time::sleep;

use crate::token_generator::TokenGenerator;
use crate::ml_dsa_signatures::PostQuantumSigner;
use crate::ephemeral_state::VaultState;

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("=========================================================");
    println!("     VISA HYPERNET-AEGIS: DYNAMIC TOKEN VAULT            ");
    println!("=========================================================");

    let signer = PostQuantumSigner::new();
    let vault_state = VaultState::new();
    
    // Boot the background memory sweeper
    vault_state.start_ttl_sweeper().await;

    println!("[gRPC] Vault Service listening on 127.0.0.1:50051...");

    // Simulating inbound gRPC requests from the C++ routing kernel
    for i in 1..=3 {
        sleep(Duration::from_millis(500)).await;
        
        let real_pan = format!("441712345678901{}", i);
        println!("\n[gRPC-INBOUND] Tokenization requested for new transaction flow.");
        
        // 1. Generate Token
        let ephemeral_token = TokenGenerator::issue_dynamic_token(&real_pan);
        
        // 2. Sign mathematically with Post-Quantum Lattice cryptography
        let _signature = signer.sign_token_payload(ephemeral_token.tokenized_pan.as_bytes());
        
        // 3. Store in active memory
        vault_state.store_token(ephemeral_token.clone());
    }

    // Simulate an authorization returning to burn a token
    sleep(Duration::from_secs(2)).await;
    println!("\n[gRPC-INBOUND] Authorization clearing received. Attempting to burn token...");
    // Mocking the burn with a fake token PAN
    vault_state.retrieve_and_burn_token("4417129988776655");

    println!("=========================================================");
    println!(" VAULT SERVICE SHUTTING DOWN.");
    println!("=========================================================");

    Ok(())
}
