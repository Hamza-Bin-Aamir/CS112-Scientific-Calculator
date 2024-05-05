#include <iostream> // contains the standard cin and cout implementations
#include <exception> // need this for standard exceptions

using namespace std; // so we dont have to repeatedly write std::cin, std::cout

enum ErrorCodes{
	NO_ERRORS = 0,
	UNKNOWN_ERR
};

string Calculate(string IN){

}

int main()
{
	ErrorCodes ErrorCode;
	string UserPrompt;
	while(true){
		UserPrompt = ""; // Good practice to reset the variable at every iteration
		cin.clear(); // clears any errors in the input stream, as we will handle erroneous behavior 

		cout << "Please enter your calculation. Write 'Exit' to Exit:";
		cin >> UserPrompt;

		if(UserPrompt == "Exit"){
			return ErrorCode;
		}

		try {
			Calculate(UserPrompt);
		} 
		
		catch(...){
			ErrorCode = UNKNOWN_ERR;
			cout << "Unknown error while processing your calculation, please try again.";
		}
	}

	return ErrorCode; // this macro term "NO ERRORS" helps us clarify code
}