use std::ffi::{CStr, CString};
use std::fs;
use std::io::Write;

#[no_mangle]
pub extern "C" fn alsh_std_env(key: *const std::os::raw::c_char) -> *mut std::os::raw::c_char {
    if key.is_null() {
        return CString::new("").unwrap().into_raw();
    }
    let k = unsafe { CStr::from_ptr(key).to_string_lossy().into_owned() };
    match std::env::var(&k) {
        Ok(v) => CString::new(v).unwrap_or_default().into_raw(),
        Err(_) => CString::new("").unwrap().into_raw(),
    }
}

#[no_mangle]
pub extern "C" fn alsh_std_readfile(path: *const std::os::raw::c_char) -> *mut std::os::raw::c_char {
    if path.is_null() {
        return CString::new("").unwrap().into_raw();
    }
    let p = unsafe { CStr::from_ptr(path).to_string_lossy().into_owned() };
    match fs::read_to_string(&p) {
        Ok(s) => CString::new(s).unwrap_or_default().into_raw(),
        Err(_) => CString::new("").unwrap().into_raw(),
    }
}

#[no_mangle]
pub extern "C" fn alsh_std_writefile(path: *const std::os::raw::c_char, contents: *const std::os::raw::c_char) -> i32 {
    if path.is_null() || contents.is_null() {
        return -1;
    }
    let p = unsafe { CStr::from_ptr(path).to_string_lossy().into_owned() };
    let c = unsafe { CStr::from_ptr(contents).to_string_lossy().into_owned() };
    match fs::File::create(&p) {
        Ok(mut f) => match f.write_all(c.as_bytes()) {
            Ok(_) => 0,
            Err(_) => -1,
        },
        Err(_) => -1,
    }
}

#[no_mangle]
pub extern "C" fn alsh_std_appendfile(path: *const std::os::raw::c_char, contents: *const std::os::raw::c_char) -> i32 {
    if path.is_null() || contents.is_null() {
        return -1;
    }
    let p = unsafe { CStr::from_ptr(path).to_string_lossy().into_owned() };
    let c = unsafe { CStr::from_ptr(contents).to_string_lossy().into_owned() };
    match fs::OpenOptions::new().create(true).append(true).open(&p) {
        Ok(mut f) => match f.write_all(c.as_bytes()) {
            Ok(_) => 0,
            Err(_) => -1,
        },
        Err(_) => -1,
    }
}

#[no_mangle]
pub extern "C" fn alsh_std_exists(path: *const std::os::raw::c_char) -> i32 {
    if path.is_null() {
        return 0;
    }
    let p = unsafe { CStr::from_ptr(path).to_string_lossy().into_owned() };
    if fs::metadata(&p).is_ok() { 1 } else { 0 }
}
