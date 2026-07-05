#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

case "${1:-build}" in
  build)
    echo "Building Rust stdlib crate..."
    cargo build --manifest-path "$ROOT_DIR/impl/rust/Cargo.toml"
    ;;
  test)
    echo "Running Rust tests..."
    cargo test --manifest-path "$ROOT_DIR/impl/rust/Cargo.toml"
    ;;
  clean)
    echo "Cleaning Rust build artifacts..."
    cargo clean --manifest-path "$ROOT_DIR/impl/rust/Cargo.toml"
    ;;
  *)
    echo "Usage: $0 {build|test|clean}" >&2
    exit 1
    ;;
esac
