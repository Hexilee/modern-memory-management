use modern_memory_management::A;

fn main() {
    let a = Box::new(A::new(1));
    println!("a._data={}", a.data());
}
