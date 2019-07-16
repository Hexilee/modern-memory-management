#[derive(Clone, Copy)]
pub struct C {
    data: i32
}

impl C {
    pub fn new(value: i32) -> Self {
        Self { data: value }
    }

    pub fn data(&self) -> &i32 {
        &self.data
    }
}


fn consume_c(_c: C) {}

fn main() {
    let c = C::new(1);
    consume_c(c);
    println!("c = {}", c.data());
}