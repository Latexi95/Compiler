module mod;
namespace ns {
///Targeted comment
struct test {
	i32[42] arr;
}

impl test {
	fn print_me() string {
		//Ordinary comment
		return "hello world\n";
	}
}

}