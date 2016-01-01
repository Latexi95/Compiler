module Traits;
import std::ConsoleIO;

struct Struct {
	i32 val;
	String name;
}

trait Print {
	fn print();
}

impl Print for Struct {
	fn print() {
		StdOut.printLine("Struct { val: {{this.val}}, name: \"{{this.name}}\"}");
	}
}

fn main() {
	Struct s;
	
}
