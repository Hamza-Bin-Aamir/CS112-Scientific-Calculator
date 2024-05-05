#include <iostream> // contains the standard cin and cout implementations
#include <exception> // need this for standard exceptions
#include <stdint.h> // this has different integer types

using namespace std; // so we dont have to repeatedly write std::cin, std::cout

// we return different error codes for main function based on program behavior
enum ErrorCodes{
	NO_ERRORS = 0,
	UNKNOWN_ERR,
	OUT_OF_RANGE_ERR,
	INVALID_ARG_ERR,
	UNDERFLOW_ERR,
	OVERFLOW_ERR,
	LOGIC_ERR,
	RUNTIME_ERR,
	DOMAIN_ERR,
	RANGE_ERR,
	LENGTH_ERR,
	BAD_ALLOC_ERR
};

// Splits our string into three strings
string* SplitString(string IN) {
	int position = 0;	
	string* Result = new string[3];

	if(IN.length() > 1000) {
		throw bad_alloc();
	}
	if(IN.length() > 20){
		throw length_error("Refused to process, string too long");
	}

	for(int i = 0; i < IN.length(); i++)
	{
		if(IN[i] == ' ')
		{
			position++;
			continue;
		}

		if(position > 2)
		{
			throw out_of_range("Your string has too many arguments"); 
		}

		if(IN[i] == '.'){
			throw domain_error("The valid domain of the function is integers, this input is out of range");
		}

		Result[position] += IN[i];
	}

	if(position != 2){
		throw out_of_range("Your string has too little arguments. Did you forget to use a space between each element?");
	}

	return Result;
}

int Calculate(string IN){
	// Declarations
	uint16_t Result = -1; 	// We need the result to be a *signed* integer of 16 bits
	uint8_t Operand1; 		// We need the operands to be *unsigned* integers of 8 bits
	uint8_t Operand2;
	int preCheckOp1;
	int preCheckOp2;
	string* sections;

	sections = SplitString(IN);

	// negative values cannot be assigned to unsigned
	if(sections[0][0] == '-' || sections[2][0] == '-')
	{
		throw underflow_error("Please make sure that your values are unsigned (positive only).");
	}

	// try to convert the string to integer, and if it fails, throw the relevant errors (stoi automatically throws invalid arg)
	try{
		preCheckOp1 = stoi(sections[0]);
		preCheckOp2 = stoi(sections[2]);
	}
	catch (const invalid_argument& E){
		throw E;
	}
	catch (const out_of_range& E){
		throw E;
	}
	
	// check if the lengths of the integers is within 8 bits
	if(preCheckOp1 > 255 || preCheckOp2 > 255){
		throw overflow_error("Please make sure that your values are 8-bit (less than 255).");
	}

	// We have verified that we can transfer these values safely
	Operand1 = preCheckOp1;
	Operand2 = preCheckOp2;

	switch(sections[1][0]){
		case '-':
			if((Operand1 - Operand2) < 0){
				throw range_error("This calculator cannot return negative values");
			}
			Result = Operand1 - Operand2;
			break;
		case '+':
			if((Operand1 + Operand2) > 255){
				throw range_error("This calculator cannot return values higher than 255");
			}
			Result = Operand1 + Operand2;
			break;
		case '*':
			if((Operand1 * Operand2) > 255){
				throw range_error("This calculator cannot return values higher than 255");
			}
			Result = Operand1 * Operand2;
			break;
		case '/':
			if(Operand2 == 0){
				throw runtime_error("Cannot divide by zero");
			}
			if((Operand1 / Operand2) > 255){
				throw range_error("This calculator cannot return values higher than 255");
			}
			Result = Operand1 / Operand2;
			break;
		default:
			throw logic_error("Invalid Operator. (Valid operators are: - + * / )");
	}

	return Result;
}

int main()
{
	ErrorCodes ErrorCode;
	string UserPrompt;
	while(true){
		UserPrompt = ""; // Good practice to reset the variable at every iteration
		cin.clear(); // clears any errors in the input stream, as we will handle erroneous behavior 

		cout << "Please enter your calculation, separated by spaces [Example: 2 + 2]. Write 'Exit' to Exit: ";
		getline(cin, UserPrompt);

		if(UserPrompt == "Exit"){
			return ErrorCode;
		}

		try {
			cout << Calculate(UserPrompt) << endl;
		} 
		
		// When you try to give a decimal value, you will get a range error
		catch(const domain_error& E){
			ErrorCode = DOMAIN_ERR;
			cout << "DOMAIN ERROR: " << E.what() << endl;
		}

		// Will be thrown when calculations result in negative values
		catch(const range_error& E){
			ErrorCode = RANGE_ERR;
			cout << "RANGE ERROR: " << E.what() << endl;
		}

		// Can be thrown by stoi or our user defined SplitString() function
		catch(const out_of_range& E){
			ErrorCode = OUT_OF_RANGE_ERR;
			cout << "INPUT OUT OF RANGE ERROR: " << E.what() << endl;
		}
		
		// Can be thrown by stoi
		catch(const invalid_argument& E){
			ErrorCode = INVALID_ARG_ERR;
			if(E.what() == "stoi"){
				cout << "INVALID ARGUMENT: " << "Failed to convert from string to integer" << endl;
			}
			else{
				cout << "INVALID ARGUMENT: " << E.what() << endl;
			}
		}

		// We are dealing with unsigned integers, so negative numbers will cause underflow
		catch(const underflow_error& E){
			ErrorCode = UNDERFLOW_ERR;
			cout << "UNDERFLOW ERROR: " << E.what() << endl;
		}

		// We are dealing with 8 bit integers, so values above 255 will cause overflow
		catch(const overflow_error& E){
			ErrorCode = OVERFLOW_ERR;
			cout << "OVERFLOW ERROR: " << E.what() << endl;
		}

		// We will refuse to process overly long strings (as they cannot contain less than 255 digits)
		catch(const length_error& E){
			ErrorCode = LENGTH_ERR;
			cout << "LENGTH ERROR: " << E.what() << endl; 
		}


		// When the operator is unknown, we will throw a logic error
		catch(const logic_error& E){
			ErrorCode = LOGIC_ERR;
			cout << "LOGIC ERROR: " << E.what() << endl;
		}

		// When you divide by zero, you will get a runtime error
		catch(const runtime_error& E){
			ErrorCode = RUNTIME_ERR;
			cout << "RUNTIME ERROR: " << E.what() << endl;
		}

		// When the strings are so long that trying to allocate them to heap might cause errors, we throw bad alloc instead
		catch(const bad_alloc& E){
			ErrorCode = BAD_ALLOC_ERR;
			cout << "BAD ALLOC: " << "Your string is too long, for security, it is not being processed" << endl;
		}

		// "Catch all" for general errors
		catch(...){
			ErrorCode = UNKNOWN_ERR;
			cout << "Unknown error while processing your calculation, please try again." << endl;
		}
	}

	return ErrorCode; // this macro term "NO ERRORS" helps us clarify code
}