fn main() {
    let crate_dir = std::path::Path::new(env!("CARGO_MANIFEST_DIR"));
    let c_dir = crate_dir.join("..").join("c");
    let c_dir = c_dir.canonicalize().unwrap_or(c_dir);

    println!("cargo:rerun-if-changed={}", c_dir.display());
    cc::Build::new()
        .file(c_dir.join("alsh_std_common.c"))
        .file(c_dir.join("alsh_std_string.c"))
        .file(c_dir.join("alsh_std_fs.c"))
        .file(c_dir.join("alsh_std_array.c"))
        .file(c_dir.join("alsh_std_math.c"))
        .include(&c_dir)
        .compile("alsh_std_c");
}
