module mod;
namespace ns {
///Targeted comment
struct test {
	i32[42] arr;
}

impl test {
	fn string print_me() {
		//Ordinary comment
		return "hello world\n";
	}
}

}