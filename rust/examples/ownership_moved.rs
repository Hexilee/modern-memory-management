fn main() {
    let a = String::from("hello, world");
    println!("{}", &a);
    let b = a;
    println!("{}", &a);
}