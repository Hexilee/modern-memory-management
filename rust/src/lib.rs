#![feature(ptr_internals)]

use core::ptr::{self, Unique};
use core::mem;
use std::alloc::{dealloc, Layout, alloc};

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

pub struct B {
    data: Unique<i32>
}

static I32_LAYOUT: Layout = unsafe { Layout::from_size_align_unchecked(mem::size_of::<i32>(), mem::align_of::<i32>()) };

impl B {
    pub fn new(value: i32) -> Self {
        unsafe {
            let raw_ptr = alloc(I32_LAYOUT) as *mut i32;
            ptr::write(raw_ptr, value);
            Self { data: Unique::new_unchecked(raw_ptr) }
        }
    }

    pub fn as_ref(&self) -> &i32 {
        unsafe {
            self.data.as_ref()
        }
    }
}

impl Drop for B {
    fn drop(&mut self) {
        println!("drop raw ptr: {:?}", self.data);
        unsafe {
            dealloc(self.data.as_ptr() as *mut u8, I32_LAYOUT);
        }
    }
}

impl Clone for B {
    fn clone(&self) -> Self {
        B::new(*self.as_ref())
    }
}
