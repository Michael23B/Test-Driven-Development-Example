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

//3.
//Allow the Add method to handle new lines between numbers (instead of commas).
//The following input is ok : “1\n2, 3”(will equal 6)
// - No change needed

//4.
//Support different delimiters
//To change a delimiter, the beginning of the string will contain a separate line that looks like this:
//“[delimiter]\n[numbers…]” for example “;\n1;2” should return three where the default delimiter is ‘;’.
//The first line is optional. All existing scenarios should still be supported
// - Added explicit delimiter check, if none is supplied any non-digit is considered a delimiter

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

	char delim = ' ';
	bool usingDelim = false;

	if (numbers.size() && !isdigit(numbers.front())) {	//if numbers isn't empty, check the front for a delimiter // Step 4.
		usingDelim = true;
		delim = numbers.front();
	}

	for (char c : numbers) {
		//if (converted.size() == 3) break;	// Step 2.
		if (isdigit(c) && !usingDelim) substring += c;	//check if user supplied a delim otherwise only check for digits // Step 4.
		else if (c != delim && usingDelim) substring += c;	// Step 4.
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
	std::cout << Add("1 2 3") << '\n';
	std::cout << Add(",2,3,4") << '\n';

	//Expected output:
	//6. 1 + 2 + 3, no delimiter so we only read digits.
	//9. Comma as delimiter

	return 0;
}