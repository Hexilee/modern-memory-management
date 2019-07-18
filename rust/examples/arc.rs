use std::sync::Arc;

fn main() {
    let a = Arc::new(1);
    let b = a.clone();
    println!("a={}", a);
    println!("b={}", b);
}