use modern_memory_management::A;

fn get_data(a: &A) -> &i32 {
    let data = 1;
    &data
}

fn main() {
    println!("a._data={}", get_data(&A::new(0)));
}