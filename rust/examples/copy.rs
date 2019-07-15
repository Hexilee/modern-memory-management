use modern_memory_management::C;
use std::rc::Rc;

#[derive(Clone, Copy)]
pub struct C {
    data: i32
}

impl C {
    pub fn new(value: i32) -> Self {
        Self { data: value}
    }

    pub fn data(&self) -> &i32 {
        &self.data
    }
}



fn get_c() -> C {
    let c = C::new(1);
    c
}

fn main() {
    let c = get_c();
    println!("c = {}", c.as_ref());
}