use std::sync::{Arc, Weak};

fn get_dead_data() -> Weak<&'static str> {
    Arc::downgrade(&Arc::new("dead"))
}

fn main() {
    if let Some(alive) = Arc::downgrade(&Arc::new("alive")).upgrade() {
        println!("{}", alive);
    }

    if let Some(dead) = get_dead_data().upgrade() {
        println!("{}", dead);
    }
}
