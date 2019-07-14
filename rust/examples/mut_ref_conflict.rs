fn main() {
    let mut data = 0;
    let ref_data = &data;
    let mut mut_ref = &mut data;
    println!("data={}", ref_data);
}
