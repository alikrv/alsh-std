use std::io::{self, Read};

fn main() {
    let args: Vec<String> = std::env::args().collect();
    if args.len() < 2 {
        std::process::exit(2);
    }

    match args[1].as_str() {
        "io" => {
            alsh_std::stdio::print("stdout");
            alsh_std::stdio::println(" newline");
            alsh_std::stdio::eprint("stderr");
            alsh_std::stdio::eprintln(" newline");
        }
        "input" => {
            let mut buf = String::new();
            io::stdin().read_to_string(&mut buf).unwrap();
            let trimmed = buf.trim_end_matches('\n').trim_end_matches('\r');
            println!("{}", trimmed);
        }
        "exit" => {
            alsh_std::stdio::exit(7);
        }
        _ => std::process::exit(2),
    }
}
