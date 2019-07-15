use modern_memory_management::B;

fn boxed(value: i32) -> B {
    let b = B::new(value);
    return b;
}

fn main() {
    let b = boxed(1);
    println!("b = {}", b.as_ref());
}