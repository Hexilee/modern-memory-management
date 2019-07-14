use modern_memory_management::A;

fn get_data() -> &i32 {
    let a = A::new(0);
    a.data()
}

fn main() {
    println!("a._data={}", get_data());
}