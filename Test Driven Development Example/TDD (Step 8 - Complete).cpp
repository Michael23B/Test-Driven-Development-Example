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

//8.
//Allow multiple delimiters like this: “//[delim1][delim2]\n” for example “//[-][%]\n1-2%3” should return 6.
//Make sure you can also handle multiple delimiters with length longer than one char
// - Changed the delimiter to a vector of delimiters
//	 Moved code for checking delimiters in the string to a new function
//	 Removed single character delimiters without []

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
	if (result < 0) throw NegativeNumberException(result);
	if (result > 1000) result = 0;
	return result;
}

bool CheckDelim(const std::string& delim, const std::string& numbers, std::string& substring, std::vector<int>& converted, int& i) {
	if (numbers[i] == delim.front()) {	//character matches the start of users delim
		for (int j = 0; j < delim.size(); ++j) {
			if ((i + j) >= numbers.size() || numbers[i + j] != delim[j]) return false;	//we are at the end of the string or character doesn't match, delim not found
		}
		//we found users delim, get an int from the current substring
		if (substring != "") {
			converted.push_back(StringToNumber<int>(substring));
			substring = "";
		}
		i += delim.size() - 1;	//now skip over the substring
		return true;
	}
	else return false;
}

int Add(std::string numbers) {
	std::vector<int> converted;
	std::string substring = "";
	int result = 0;

	std::vector<std::string> delimiters;
	bool usingDelim = false;
	bool readingDelim = false;

	if (numbers.size() && !isdigit(numbers.front())) {	//if numbers isn't empty, check the front for a delimiter // Step 4.
		usingDelim = true;
		readingDelim = true;
		delimiters.push_back("");
	}

	for (int i = 0; i < numbers.size(); ++i) {
		if (readingDelim) {
			if (numbers[i] == '[') continue;	//skip this character
			if (numbers[i] == ']') { //finished reading delim
				if ((i + 1) < numbers.size() && numbers[i + 1] != '[') readingDelim = false;	//range check first, then if we don't find another delim declaration stop checking
				else delimiters.push_back("");
				continue; 
			}	
			delimiters[delimiters.size() - 1] += numbers[i];
			continue;
		}
		
		if (isdigit(numbers[i]) && !usingDelim) substring += numbers[i];	//check if user supplied a delim otherwise only check for digits // Step 4.
		else if (usingDelim) {
			bool foundDelim = false;
			for (std::string delim : delimiters) {	//try each delim in the delim vector
				if (CheckDelim(delim, numbers, substring, converted, i)) {
					foundDelim = true; 
					break;
				}
			}
			if (!foundDelim) substring += numbers[i]; //didnt find delim, just add this character to the substring
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

int main()
{
	try{

		std::cout << "Accepts the following syntax:\n**\nstring-of-numbers\n**\n[delimiter]\n[more delimiters...]\nstring-of-numbers\n**\n";
		std::cout << Add("1 2 3") << '\n';
		std::cout << Add("[,,][..]1..2,,3") << '\n';
		std::cout << Add("[\nn][...]1\nn1001|\nn1\n1 ,.(\nn1...1\n") << '\n';
		std::cout << Add("[;]23;/4;;7") << '\n';
		std::cout << Add("[;]") << '\n';
		std::cout << Add("[\n]3\n9\n-1") << '\n';

		//Expected output:
		//6. evaluates to 1 + 2 + 3
		//6. evaluates to 1 + 2 + 3 using two seperate delimiters
		//4. 1001 is ignored (over the 1000 limit), 1\n1 is converted to 1 since \n is invalid but not a delimiter, 1...1 is two more ones seperated by the 2nd delimiter
		//30. the / character invalidates the 4 since it is not a digit or delimiter
		//0. just a delimiter by itself
		//Exception. Negative number
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << '\n';
	}
	system("pause");	//prevent cmd window from closing on windows
    return 0;
}