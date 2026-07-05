pub mod stdio;
pub mod fs;
pub mod string_utils;

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct alsh_string_ptr(pub *mut std::os::raw::c_char);

#[cfg(not(test))]
#[link(name = "c")]
extern "C" {
    pub fn alsh_std_split(input: *const std::os::raw::c_char, sep: *const std::os::raw::c_char) -> *mut std::os::raw::c_char;
    pub fn alsh_std_lines(input: *const std::os::raw::c_char) -> *mut std::os::raw::c_char;
    pub fn alsh_std_padleft(input: *const std::os::raw::c_char, width: i32, fill: std::os::raw::c_char) -> *mut std::os::raw::c_char;
    pub fn alsh_std_padright(input: *const std::os::raw::c_char, width: i32, fill: std::os::raw::c_char) -> *mut std::os::raw::c_char;
    pub fn alsh_std_format_gb(kb: f64) -> *mut std::os::raw::c_char;
    pub fn alsh_std_format_gib(kb: f64) -> *mut std::os::raw::c_char;
    pub fn alsh_std_format_mb(kb: f64) -> *mut std::os::raw::c_char;
    pub fn alsh_std_format_mib(kb: f64) -> *mut std::os::raw::c_char;
    pub fn alsh_std_sizeof(value: *const std::os::raw::c_char) -> usize;
    pub fn alsh_std_sizeoft(value: *const std::os::raw::c_char) -> usize;
    pub fn alsh_std_basename(path: *const std::os::raw::c_char) -> *mut std::os::raw::c_char;
    pub fn alsh_std_dirname(path: *const std::os::raw::c_char) -> *mut std::os::raw::c_char;
    pub fn alsh_std_joinpath(left: *const std::os::raw::c_char, right: *const std::os::raw::c_char) -> *mut std::os::raw::c_char;
    pub fn alsh_std_isfile(path: *const std::os::raw::c_char) -> i32;
    pub fn alsh_std_isdir(path: *const std::os::raw::c_char) -> i32;
    pub fn alsh_std_mkdir(path: *const std::os::raw::c_char) -> i32;
    pub fn alsh_std_listdir(path: *const std::os::raw::c_char) -> *mut std::os::raw::c_char;
    pub fn alsh_std_which(cmd: *const std::os::raw::c_char) -> *mut std::os::raw::c_char;
    pub fn alsh_std_pid() -> i32;
    pub fn alsh_std_sleep(ms: i32);
    pub fn alsh_std_getuser() -> *mut std::os::raw::c_char;
    pub fn alsh_std_min(a: f64, b: f64) -> f64;
    pub fn alsh_std_max(a: f64, b: f64) -> f64;
    pub fn alsh_std_clamp(value: f64, min: f64, max: f64) -> f64;
    pub fn alsh_std_even(n: i64) -> i32;
    pub fn alsh_std_odd(n: i64) -> i32;
    pub fn alsh_std_len(value: *const std::os::raw::c_char) -> usize;
    pub fn alsh_std_push(value: *const std::os::raw::c_char, item: *const std::os::raw::c_char) -> *mut std::os::raw::c_char;
    pub fn alsh_std_pop(value: *const std::os::raw::c_char) -> *mut std::os::raw::c_char;
    pub fn alsh_std_first(value: *const std::os::raw::c_char) -> *mut std::os::raw::c_char;
    pub fn alsh_std_last(value: *const std::os::raw::c_char) -> *mut std::os::raw::c_char;
    pub fn alsh_std_slice(value: *const std::os::raw::c_char, from: usize, to: usize) -> *mut std::os::raw::c_char;
    pub fn alsh_std_containsa(value: *const std::os::raw::c_char, item: *const std::os::raw::c_char) -> i32;
    pub fn alsh_std_reverse(value: *const std::os::raw::c_char) -> *mut std::os::raw::c_char;
    pub fn alsh_std_join(value: *const std::os::raw::c_char, sep: *const std::os::raw::c_char) -> *mut std::os::raw::c_char;
    pub fn alsh_std_free_c_string(value: *mut std::os::raw::c_char);
}
