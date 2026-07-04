use std::ffi::{CStr, CString};
use std::io::{self, Write};

pub fn print(value: &str) {
    let _ = io::stdout().write_all(value.as_bytes());
}

pub fn println(value: &str) {
    print(value);
    let _ = io::stdout().write_all(b"\n");
}

pub fn eprint(value: &str) {
    let _ = io::stderr().write_all(value.as_bytes());
}

pub fn eprintln(value: &str) {
    eprint(value);
    let _ = io::stderr().write_all(b"\n");
}

pub fn input(prompt: &str) -> String {
    print(prompt);
    let mut input = String::new();
    let _ = io::stdin().read_line(&mut input);
    if input.ends_with('\n') {
        input.pop();
    }
    if input.ends_with('\r') {
        input.pop();
    }
    input
}

pub fn exit(code: i32) -> ! {
    std::process::exit(code)
}

#[no_mangle]
pub extern "C" fn alsh_std_print(value: *const std::os::raw::c_char) {
    if value.is_null() {
        return;
    }
    let c_str = unsafe { CStr::from_ptr(value) };
    print(&c_str.to_string_lossy());
}

#[no_mangle]
pub extern "C" fn alsh_std_println(value: *const std::os::raw::c_char) {
    if value.is_null() {
        return;
    }
    let c_str = unsafe { CStr::from_ptr(value) };
    println(&c_str.to_string_lossy());
}

#[no_mangle]
pub extern "C" fn alsh_std_eprint(value: *const std::os::raw::c_char) {
    if value.is_null() {
        return;
    }
    let c_str = unsafe { CStr::from_ptr(value) };
    eprint(&c_str.to_string_lossy());
}

#[no_mangle]
pub extern "C" fn alsh_std_input(prompt: *const std::os::raw::c_char) -> *mut std::os::raw::c_char {
    let prompt_str = if prompt.is_null() {
        String::new()
    } else {
        unsafe { CStr::from_ptr(prompt).to_string_lossy().into_owned() }
    };
    let value = input(&prompt_str);
    CString::new(value).unwrap_or_default().into_raw()
}

#[no_mangle]
pub extern "C" fn alsh_std_exit(code: i32) -> ! {
    exit(code)
}

#[no_mangle]
pub extern "C" fn alsh_std_free_string(value: *mut std::os::raw::c_char) {
    if !value.is_null() {
        unsafe {
            let _ = CString::from_raw(value);
        }
    }
}
