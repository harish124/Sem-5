struct f {
	int n;
};

program fibonacci {
	version fib_vers {
		int fib(f) =1;
	}=1;
}=0x12345678;