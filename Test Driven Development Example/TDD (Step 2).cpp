#include <string>
#include <vector>
#include <iostream>
#include <sstream>

//An example of test driven development. Following code requirements from here:
//https://technologyconversations.com/2013/12/20/test-driven-development-tdd-example-walkthrough/

//1.
//Create a simple String calculator with a method int Add(string numbers)
//The method can take 0, 1 or 2 numbers, and will return their sum (for an empty string it will return 0) for example “” or “1” or “1,2”
// - Added T StringToNumber() and the Add() function

//2.
//Allow the Add method to handle an unknown amount of numbers
// - Removed the size check for the Add() function

template <typename T>
T StringToNumber(const std::string& s) {
	std::stringstream ss(s);
	T result = T();
	ss >> result;
	return result;
}

int Add(std::string numbers) {
	std::vector<int> converted;
	std::string substring = "";
	int result = 0;

	for (char c : numbers) {
		//if (converted.size() == 3) break;	// Step 2.
		if (isdigit(c)) substring += c;
		else if (substring != "") {
			converted.push_back(StringToNumber<int>(substring));
			substring = "";
		}
	}
	converted.push_back(StringToNumber<int>(substring));

	for (int i : converted) result += i;
	return result;
}

int main()
{
	std::cout << Add("1, 2, 3, 4") << '\n';

	//Expected output:
	//10. 1 + 2 + 3 + 4, 4 is not ignored since size limit is removed

	return 0;
}