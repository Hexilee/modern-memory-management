fn main() {
    let a = String::from("hello, world");
    let ref_a = &a;
    println!("{}", ref_a);
    let b = a;
    println!("{}", ref_a);
}