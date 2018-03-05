# Test-Driven-Development-Example
C++ implementation of a trivial TDD example.

Original article found here:
https://technologyconversations.com/2013/12/20/test-driven-development-tdd-example-walkthrough/

The code in the article is written in java and goes through a small number of steps, showing how you might approach each using TDD. I decided to just follow the requirements and wrote my code in C++. The steps are as follows:

*Try to read only one requirement, write the tests and the implementation yourself and compare it with the results from this article.*

**Requirements**

    1. Create a simple String calculator with a method int Add(string numbers)
    The method can take 0, 1 or 2 numbers, and will return their sum (for an empty string it will return 0) for example “” or “1” or “1,2”
    
    2. Allow the Add method to handle an unknown amount of numbers
    
    3. Allow the Add method to handle new lines between numbers (instead of commas).
    The following input is ok: “1\n2,3” (will equal 6)
    
    4. Support different delimiters
    To change a delimiter, the beginning of the string will contain a separate line that looks like this: “//[delimiter]\n[numbers…]” for example “//;\n1;2” should return three where the default delimiter is ‘;’ .
    The first line is optional. All existing scenarios should still be supported
    
    5. Calling Add with a negative number will throw an exception “negatives not allowed” – and the negative that was passed. If there are multiple negatives, show all of them in the exception message stop here if you are a beginner.
    
    6. Numbers bigger than 1000 should be ignored, so adding 2 + 1001 = 2
    
    7. Delimiters can be of any length with the following format: “//[delimiter]\n” for example: “//[—]\n1—2—3” should return 6
    
    8. Allow multiple delimiters like this: “//[delim1][delim2]\n” for example “//[-][%]\n1-2%3” should return 6.
    Make sure you can also handle multiple delimiters with length longer than one char

Solution includes two sets of files. One uses boost auto test and requires boost to run. The other is an edited version with manual expected output which requires no additional headers.
