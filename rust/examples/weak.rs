use std::rc::{Rc, Weak};

fn get_dead_data() -> Weak<&'static str> {
    Rc::downgrade(&Rc::new("dead"))
}

fn main() {
    if let Some(alive) = Rc::downgrade(&Rc::new("alive")).upgrade() {
        println!("{}", alive);
    }

    if let Some(dead) = get_dead_data().upgrade() {
        println!("{}", dead);
    }
}
