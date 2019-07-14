use modern_memory_management::A;

fn main() {
    let mut a = A::new(0);
    let data = a.data();
    let mut mut_a = &mut a;
    println!("data={}", data);
}