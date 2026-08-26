//! Ephemeral Token Generation and PAN Substitution

use rand::Rng;
use std::time::{SystemTime, UNIX_EPOCH};

#[derive(Debug, Clone)]
pub struct EphemeralToken {
    pub original_pan: String,
    pub tokenized_pan: String,
    pub expiration_timestamp: u64,
}

pub struct TokenGenerator;

impl TokenGenerator {
    /// Generates a single-use 16-digit token using Format-Preserving constraints
    pub fn issue_dynamic_token(real_pan: &str) -> EphemeralToken {
        let mut rng = rand::thread_rng();
        
        // Keep the same 6-digit BIN (Bank Identification Number) and generate the rest
        let bin = &real_pan[0..6];
        let random_suffix: u64 = rng.gen_range(1_000_000_000..9_999_999_999);
        
        let tokenized_pan = format!("{}{}", bin, random_suffix);
        
        // Token is strictly ephemeral - it lives for exactly 45 seconds
        let now = SystemTime::now().duration_since(UNIX_EPOCH).unwrap().as_secs();
        let expiration = now + 45;

        println!("[TOKEN-GEN] Issued ephemeral token masking PAN ending in {} (TTL: 45s)", &real_pan[12..16]);

        EphemeralToken {
            original_pan: real_pan.to_string(),
            tokenized_pan,
            expiration_timestamp: expiration,
        }
    }
}
