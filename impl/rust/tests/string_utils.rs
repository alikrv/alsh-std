use alsh_std::{stdio, string_utils};

#[test]
fn simple_upper_lower_trim() {
    let s = "  Hello WoRld\n";
    let trimmed = string_utils::alsh_std_trim(std::ffi::CString::new(s).unwrap().as_ptr());
    unsafe {
        let t = std::ffi::CStr::from_ptr(trimmed);
        assert_eq!(t.to_str().unwrap(), "Hello WoRld");
        stdio::alsh_std_free_string(trimmed as *mut _);
    }

    let up = string_utils::alsh_std_upper(std::ffi::CString::new("abc").unwrap().as_ptr());
    unsafe {
        let u = std::ffi::CStr::from_ptr(up);
        assert_eq!(u.to_str().unwrap(), "ABC");
        stdio::alsh_std_free_string(up as *mut _);
    }
}
