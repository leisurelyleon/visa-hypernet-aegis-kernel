//! In-Memory TTL Token State Manager

use dashmap::DashMap;
use std::sync::Arc;
use tokio::time::{sleep, Duration};
use std::time::{SystemTime, UNIX_EPOCH};

use crate::token_generator::EphemeralToken;

pub struct VaultState {
    // Maps Tokenized PAN -> Original Ephemeral Token Data
    active_tokens: Arc<DashMap<String, EphemeralToken>>,
}

impl VaultState {
    pub fn new() -> Self {
        println!("[VAULT-STATE] Booting In-Memory Ephemeral Storage Tracker...");
        Self {
            active_tokens: Arc::new(DashMap::new()),
        }
    }

    pub fn store_token(&self, token: EphemeralToken) {
        self.active_tokens.insert(token.tokenized_pan.clone(), token);
    }

    pub fn retrieve_and_burn_token(&self, tokenized_pan: &str) -> Option<String> {
        // Once a token is used for an authorization, it is instantly burned (removed)
        if let Some((_, token_data)) = self.active_tokens.remove(tokenized_pan) {
            println!("[VAULT-STATE] Token authorized and successfully burned from memory.");
            return Some(token_data.original_pan);
        }
        None
    }

    /// Background daemon to sweep memory for expired tokens that were never used
    pub async fn start_ttl_sweeper(&self) {
        let tokens_ref = self.active_tokens.clone();
        tokio::spawn(async move {
            loop {
                sleep(Duration::from_secs(5)).await;
                let now = SystemTime::now().duration_since(UNIX_EPOCH).unwrap().as_secs();
                
                tokens_ref.retain(|_, token| {
                    let is_valid = token.expiration_timestamp > now;
                    if !is_valid {
                        println!("[VAULT-STATE] ⚠️ TTL Expired! Swept abandoned token from memory.");
                    }
                    is_valid
                });
            }
        });
    }
}
