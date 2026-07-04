use std::io::Write;
use std::process::{Command, Stdio};

#[test]
fn stdio_example_emits_stdout_and_stderr() {
    let output = Command::new(env!("CARGO"))
        .arg("run")
        .arg("--quiet")
        .arg("--example")
        .arg("stdio")
        .arg("io")
        .output()
        .expect("example should run");

    assert!(output.status.success());
    assert_eq!(String::from_utf8_lossy(&output.stdout), "stdout newline\n");
    assert_eq!(String::from_utf8_lossy(&output.stderr), "stderr newline\n");
}

#[test]
fn input_example_reads_stdin_and_echoes_value() {
    let mut child = Command::new(env!("CARGO"))
        .arg("run")
        .arg("--quiet")
        .arg("--example")
        .arg("stdio")
        .arg("input")
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
        .spawn()
        .expect("example should run");

    child
        .stdin
        .as_mut()
        .unwrap()
        .write_all(b"hello from stdin\n")
        .expect("stdin should be writable");

    let output = child.wait_with_output().expect("child should finish");
    assert!(output.status.success());
    assert_eq!(String::from_utf8_lossy(&output.stdout), "hello from stdin\n");
    assert!(output.stderr.is_empty());
}

#[test]
fn exit_example_returns_requested_status_code() {
    let output = Command::new(env!("CARGO"))
        .arg("run")
        .arg("--quiet")
        .arg("--example")
        .arg("stdio")
        .arg("exit")
        .output()
        .expect("example should run");

    assert_eq!(output.status.code(), Some(7));
    assert!(output.stdout.is_empty());
    assert!(output.stderr.is_empty());
}
