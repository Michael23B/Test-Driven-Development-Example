#define BOOST_TEST_MODULE AddStringTest

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "boost\test\unit_test.hpp"

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

//5.
//Calling Add with a negative number will throw an exception “negatives not allowed” – and the negative that was passed.
//If there are multiple negatives, show all of them in the exception message.
// - Added NegativeNumberException and try catch block


//6.
//Numbers bigger than 1000 should be ignored, so adding 2 + 1001 = 2
// - Added check in StringToNumber()

//7.
//Delimiters can be of any length with the following format: “//[delimiter]\n” for example: “//[—]\n1—2—3” should return 6
// - Range-based for loop changed to be a standard for loop so we can keep track of the iterator and use it to find the delimiter substring
//	 Added a check if we are using a single or multi character delimiter at the top of Add(). Multi character delims are then read in at the start of the for loop
//	 Added a for loop once we encounter the first character of the user set delimiter. Checks if the full delimiter is there

struct NegativeNumberException : public std::exception {
	NegativeNumberException(const int& number) :msg("Negative numbers not allowed! (" + std::to_string(number) + ")") {}

	virtual char const* what() const
	{
		return msg.c_str();
	}
private:
	std::string msg;
};

template <typename T>
T StringToNumber(const std::string& s) {
	std::stringstream ss(s);
	T result = T();
	ss >> result;
	if (result < 0) throw NegativeNumberException(result);	// Step 5.
	if (result > 1000) result = 0;	// Step 6.
	return result;
}

int Add(std::string numbers) {
	std::vector<int> converted;
	std::string substring = "";
	int result = 0;

	std::string delim = "";
	bool usingDelim = false;
	bool readingDelim = false;

	if (numbers.size() && !isdigit(numbers.front())) {	//if numbers isn't empty, check the front for a delimiter // Step 4.
		usingDelim = true;

		if (numbers.front() == '[') readingDelim = true;	//we are reading in multiple character delimiter
		else delim = numbers.front();	//single character delimiter
	}

	for (int i = 0; i < numbers.size(); ++i) {

		if (readingDelim) {
			if (numbers[i] == '[') continue;	//skip this character
			if (numbers[i] == ']') { readingDelim = false; continue; }	//finished reading delim
			delim += numbers[i];
			continue;
		}

		//if (converted.size() == 3) break;	// Step 2.
		if (isdigit(numbers[i]) && !usingDelim) substring += numbers[i];	//check if user supplied a delim otherwise only check for digits // Step 4.
		else if (usingDelim) {
			if (numbers[i] == delim.front()) {	//character matches the start of users delim
				bool foundDelim = true;
				for (int j = 0; j < delim.size(); ++j) {
					if ((i + j) >= numbers.size() || numbers[i + j] != delim[j]) {	//we are at the end of the string, delim not found
						foundDelim = false;											//or not all characters match, delim not found
						break;
					}
				}
				if (foundDelim) {	//we found users delim, get an int from the current substring
					if (substring != "") {
						converted.push_back(StringToNumber<int>(substring));
						substring = "";
					}
					i += delim.size() - 1;	//now skip over the substring
				}
				else substring += numbers[i]; //didnt find delim, just add this character to the substring
			}
			else substring += numbers[i];
		}
		else if (substring != "") {
			converted.push_back(StringToNumber<int>(substring));
			substring = "";
		}
	}
	converted.push_back(StringToNumber<int>(substring));

	for (int i : converted) result += i;
	return result;
}

BOOST_AUTO_TEST_CASE(test7) {
	BOOST_CHECK(Add("[delim]\n2delim3delim4") == 9);
	BOOST_CHECK(Add("[\nn]1\nn1001|\nn1\n1 ,.(\nn1) .\n") == 3);
	BOOST_CHECK(Add(";]2;;3;4") == 7);
}