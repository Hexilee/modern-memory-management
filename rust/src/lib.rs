pub struct A {
    _data: i32,
}

impl A {
    pub fn new(data: i32) -> Self {
        A { _data: data }
    }

    pub fn data(&self) -> &i32 {
        &self._data
    }

    pub fn mut_data(&mut self) -> &mut i32 {
        &mut self._data
    }
}