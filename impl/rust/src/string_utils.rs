use std::ffi::{CStr, CString};

fn rust_trim(s: &str) -> String {
    s.trim_matches(|c: char| c == '\n' || c == '\r' || c.is_whitespace()).to_string()
}

#[no_mangle]
pub extern "C" fn alsh_std_strlen(s: *const std::os::raw::c_char) -> usize {
    if s.is_null() {
        return 0;
    }
    let s = unsafe { CStr::from_ptr(s).to_string_lossy() };
    s.len()
}

#[no_mangle]
pub extern "C" fn alsh_std_upper(s: *const std::os::raw::c_char) -> *mut std::os::raw::c_char {
    if s.is_null() {
        return CString::new("").unwrap().into_raw();
    }
    let s = unsafe { CStr::from_ptr(s).to_string_lossy().into_owned() };
    CString::new(s.to_uppercase()).unwrap_or_default().into_raw()
}

#[no_mangle]
pub extern "C" fn alsh_std_lower(s: *const std::os::raw::c_char) -> *mut std::os::raw::c_char {
    if s.is_null() {
        return CString::new("").unwrap().into_raw();
    }
    let s = unsafe { CStr::from_ptr(s).to_string_lossy().into_owned() };
    CString::new(s.to_lowercase()).unwrap_or_default().into_raw()
}

#[no_mangle]
pub extern "C" fn alsh_std_trim(s: *const std::os::raw::c_char) -> *mut std::os::raw::c_char {
    if s.is_null() {
        return CString::new("").unwrap().into_raw();
    }
    let s = unsafe { CStr::from_ptr(s).to_string_lossy() };
    CString::new(rust_trim(&s)).unwrap_or_default().into_raw()
}

#[no_mangle]
pub extern "C" fn alsh_std_contains(s: *const std::os::raw::c_char, sub: *const std::os::raw::c_char) -> i32 {
    if s.is_null() || sub.is_null() {
        return 0;
    }
    let s = unsafe { CStr::from_ptr(s).to_string_lossy() };
    let sub = unsafe { CStr::from_ptr(sub).to_string_lossy() };
    if s.contains(&*sub) { 1 } else { 0 }
}

#[no_mangle]
pub extern "C" fn alsh_std_replace(s: *const std::os::raw::c_char, from: *const std::os::raw::c_char, to: *const std::os::raw::c_char) -> *mut std::os::raw::c_char {
    if s.is_null() {
        return CString::new("").unwrap().into_raw();
    }
    let s = unsafe { CStr::from_ptr(s).to_string_lossy().into_owned() };
    let from = if from.is_null() { String::new() } else { unsafe { CStr::from_ptr(from).to_string_lossy().into_owned() } };
    let to = if to.is_null() { String::new() } else { unsafe { CStr::from_ptr(to).to_string_lossy().into_owned() } };
    if from.is_empty() {
        return CString::new(s).unwrap_or_default().into_raw();
    }
    CString::new(s.replace(&from, &to)).unwrap_or_default().into_raw()
}

#[no_mangle]
pub extern "C" fn alsh_std_startswith(s: *const std::os::raw::c_char, sub: *const std::os::raw::c_char) -> i32 {
    if s.is_null() || sub.is_null() { return 0; }
    let s = unsafe { CStr::from_ptr(s).to_string_lossy() };
    let sub = unsafe { CStr::from_ptr(sub).to_string_lossy() };
    if s.starts_with(&*sub) { 1 } else { 0 }
}

#[no_mangle]
pub extern "C" fn alsh_std_endswith(s: *const std::os::raw::c_char, sub: *const std::os::raw::c_char) -> i32 {
    if s.is_null() || sub.is_null() { return 0; }
    let s = unsafe { CStr::from_ptr(s).to_string_lossy() };
    let sub = unsafe { CStr::from_ptr(sub).to_string_lossy() };
    if s.ends_with(&*sub) { 1 } else { 0 }
}

#[no_mangle]
pub extern "C" fn alsh_std_repeat(s: *const std::os::raw::c_char, n: i32) -> *mut std::os::raw::c_char {
    if s.is_null() || n <= 0 {
        return CString::new("").unwrap().into_raw();
    }
    let s = unsafe { CStr::from_ptr(s).to_string_lossy().into_owned() };
    let repeated = s.repeat(n as usize);
    CString::new(repeated).unwrap_or_default().into_raw()
}

#[no_mangle]
pub extern "C" fn alsh_std_strip(s: *const std::os::raw::c_char, chars: *const std::os::raw::c_char) -> *mut std::os::raw::c_char {
    if s.is_null() {
        return CString::new("").unwrap().into_raw();
    }
    let s = unsafe { CStr::from_ptr(s).to_string_lossy() };
    if chars.is_null() {
        return CString::new(s.trim().to_string()).unwrap_or_default().into_raw();
    }
    let chars = unsafe { CStr::from_ptr(chars).to_string_lossy() };
    let trimmed = s.trim_matches(|c| chars.contains(c)).to_string();
    CString::new(trimmed).unwrap_or_default().into_raw()
}
