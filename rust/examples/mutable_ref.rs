use modern_memory_management::A;

fn main() {
    let mut a = A::new(0);
    let data = a.mut_data();
    *data = 1;
    println!("a._data={}", data);
}