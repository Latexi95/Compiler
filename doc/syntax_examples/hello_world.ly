module hello;
import std::ConsoleIO;

// defaults to main
@entry_point

///Main entry point      <-- targeted comment

fn main() i32 {
	StdOut.printLine("Hello world");
	return 0;
}