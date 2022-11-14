// School Lunch ordering system.cpp : This file contains the 'main' function. Program execution begins and ends there.
// up to date as of 11/11/2022 1333

#include <iostream> //Needed to input and output
#include <fstream> //Enables file functionality 
#include <sstream> //enables string functionality for data streams
#include <string> // Enables string functionality
#include <vector>
#include<numeric> //used to calculcate sum of a vector
#include <cstdio>
using std::string;
using std::cout;
using std::cin;
using std::ios;
using std::vector;
using std::fstream;
using std::stringstream;
using std::ofstream;
using std::endl;


void registerUser();

string registerGuardian();

string registerStaff();

string basicInfo();

string childInfo();

string removeComma(string input);

string appendForTable(string toBeAddedTo, string added);

void beamStringToFile(string input, string fileToBeamTo);

void beamVectorToFile(vector<vector<string>> input, string filename);

void clearFile(string filename);

void login();

vector<vector<string>> csvToVector(string fileName);

void userMainMenu(string username);

void orderMenu(string username);

void paymentWindow(string username);

void feedbackForm(string username);

void billingHistory(string username);

void userBillingHistory(string username);

void cartOpen(string username);

void paymentStatus(string username);

void editMenu();

int main()
{
	string username = "juzzitube";

	paymentStatus(username);
	/*editMenu();
	int selection;
	cout << "oi mate ya wanna register(0) or login(1) bro???\n";
	cin >> selection;
	switch (selection) {
	case 0:
		registerUser();
		break;
	case 1:
		login();
		break;
	}*/
	//paymentStatus();

}
//--------------------------------------------------------------------------------------------------------------
//Primary registration function, determines the user type
void registerUser() {
	int userType;
	string info, username, password, info2;
	bool loop = true;
	while (loop == true) {
		cout <<
			"Are you a:\n"
			"(1)Child guardian, or\n"
			"(2)Staff member?\n";
		cin >> userType;
		cout << "Please enter your username: ";
		cin >> username;
		cout << "\nPlease enter your password: ";
		cin >> password;
		switch (userType) {
		case 1:
			info = registerGuardian();
			loop = false;
			cout << info;
			break;
		case 2:
			info = registerStaff();
			loop = false;
			cout << info;
			break;
		default:
			cout << "That is not a valid option!\n\n";
			break;
		}

	}

	info2 = appendForTable(username, password);
	info2 = appendForTable(info2, "0");
	info2 = info2 + "," + info;
	beamStringToFile(info2, "userDataBase");
}

//handles guardian registration
string registerGuardian() {
	string info, childInfo2;
	info = basicInfo();
	info = info + childInfo();
	return info;
}

//Handles staff registration
string registerStaff() {
	string info;
	info = basicInfo();
	return info;
}

//Info required for everyone
string basicInfo() {
	string input, output;
	cout << "Please enter your first name: ";
	cin >> input;
	output = removeComma(input);
	cout << "Please enter your last name: ";
	cin >> input;
	output = appendForTable(output, input);
	cout << "\nPlease enter your gender: ";
	cin >> input;
	output = appendForTable(output, input);
	cout << "\nPlease enter your date of birth (DD/MM/YYYY): ";
	cin >> input;
	output = appendForTable(output, input);
	cout << "\nPlease enter your visa card number: ";
	cin >> input;
	output = appendForTable(output, input);
	cout << "\nPlease enter your card's date of expirery: ";
	cin >> input;
	output = appendForTable(output, input);
	return output;
}

//Info only required for guardians
string childInfo() {
	string input, output;
	cout << "\nPLease enter your child's name: ";
	cin >> input;
	output = appendForTable(output, input);
	cout << "\nPlease enter your childs room number: ";
	cin >> input;
	output = appendForTable(output, input);
	return output;

}

//Takes any characters from input that aren't comma's and puts them into output
string removeComma(string input) {
	string output = "";
	char add = ' ';
	for (int i = 0; i < input.length(); i++) {
		add = input.at(i);
		if (add != ',') {

			output = output + add;
		}
	}
	return output;
}
//name = "bo,b"
//stuff = "t,est"
//stuff2 = "te,s,t2"
//food = "cho,co,la,te"
//
//output = removeComma(name);
//output = appendForTable(output, stuff);
//"bob,test"
//output = appendForTable(output, stuff2)

//Appends added to the end of toBeAddedTo, while removing comma's from added
string appendForTable(string toBeAddedTo, string added) {
	added = removeComma(added);
	toBeAddedTo.append(",");
	toBeAddedTo.append(added);
	return toBeAddedTo;
}

//Sends the information to the file
void beamStringToFile(string input, string fileToBeamTo) {
	fstream file(fileToBeamTo + ".csv", ios::in | ios::app);
	char out;
	if (!file.is_open()) {
		cout << "Warning (beamStringToFile): File is not open, terminating program" << '\n';
		return;
	}
	//Extremely monkey brain solution, probably not very efficient
	for (int i = 0; i < input.length(); i++) {
		out = input.at(i);
		file << out;
	}
	file << "\n";
	file.close();
}

void clearFile(string filename) {
	fstream file;
	file.open(filename + ".csv", ios::out | ios::trunc);
	file.close();
}

void beamVectorToFile(vector<vector<string>> input, string filename) {
	string output;
	for (vector<string> row : input) {

		output = appendForTable(row.at(0), row.at(1));
		output = row.at(0);
		for (int i = 1; i < row.size(); i++) {
			output = output + "," + row.at(i);
		}
		beamStringToFile(output, filename);
	}
}
//--------------------------------------------------------------------------------------------------------------


//function to convert the CSV database to a 2D vector of strings (and return)
//Arguments: Name of file to open, without extension
//Returns: Vector conversion of CSV document
vector<vector<string>> csvToVector(string fileName) {

	string line, word;
	vector<string> row; //Stores data of current row
	vector<vector<string>> output; //2D vector for storing 
	fstream csvFile;
	csvFile.open(fileName + ".csv", ios::in | ios::app);

	if (!csvFile.is_open()) {
		output = {
			{"error", "error", "error"},
			{"error", "error", "error"}
		};
		cout << "Warning (csvToVector): " + fileName + ".csv has not opened properly, terminating!" << '\n';
		return output;
	}


	while (getline(csvFile, line, '\n')) { //Iterates through each row until there are no more new line characters
		row.clear();
		std::stringstream stream(line);

		while (getline(stream, word, ',')) { //Iterates through each cell in the row until it hits a blank one
			row.push_back(word);//Adds the data found in the cells to the row vector
		}

		output.push_back(row); //adds each row to the vector
	}
	csvFile.close();
	//for (vector<string> row : output) {
	//	for (string word : row) {
	//		std::cout << word << "\t";
	//	}
	//	std::cout << "\n";
	//}
	return output;

}

void login() {
	fstream adminCredentials;
	vector<string> row;
	adminCredentials.open("userDataBase.csv", ios::in | ios::app);
	string username, password;

	//call createContentVector before the credentials are given for the sake of performance if we were to put the login into a "While not logged in"
	vector<vector<string>> content = csvToVector("userDataBase");

	cout << "Please enter your username ";
	cin >> username;
	cout << "\nPlease enter your password: ";
	cin >> password;

	for (vector<string> row : content) {//FOR loop that iterates through each line of the vector array
		if (username == row.at(0) && password == row.at(1)) {	//Checks the first cell to see if it matches the inputted username, and the second cell to see if it matches the inputted password
			cout << "Welcome " << row.at(3) << '\n';				//Tells the user they've logged in successfully

			switch (stoi(row.at(2))) {							//Checks the third cell, which tells the program whether the user is standard or an admin
			case 0:
				cout << "logged in as user" << '\n';
				userMainMenu(username);
				break;
			case 1:
				cout << "logged in as admin" << '\n';
				// run code for admin functionality
			}
		}
	}
}

void userMainMenu(string username) {
	//Outputs main menu options to a logged in user.

	int userChoice;

	cout << "******Logged in******\n";
	cout << "Main Menu\n\n";

	do {
		cout << "Select option: \n";
		cout << "1)Order menu\n2)Billing History\n3)Provide Feedback\n4)Logout\n";
		cin >> userChoice;


		switch (userChoice) {
		case 1:
			orderMenu(username);
			break;
		case 2:
			billingHistory(username);
			break;
		case 3:
			feedbackForm(username);
			break;
		case 4:
			cout << "Logged out";
			void login();
			break;
		default:
			cout << "Please select a valid option\n";
			break;
		}
	} while (userChoice != 4);
}

void orderMenu(string username) {
	//outputs order menu for user's and leads to 'paymentwindow' and 'cartopen' functions

	int userChoice;
	fstream menuDataBase;
	fstream cart;

	menuDataBase.open("menuDataBase.csv", ios::in);
	if (!menuDataBase.is_open()) {
		cout << "file not open";
		return;
	}

	//extracts 'menudatabase' data and pushes it to 'filecontent'.
	string line, word;
	vector<string> row;
	vector<vector<string>> fileContent;
	while (getline(menuDataBase, line, '\n')) {
		row.clear();
		std::stringstream stream(line);
		while (getline(stream, word, ',')) {
			row.push_back(word);
		}
		fileContent.push_back(row);
	}

	do {
		//outputs filecontent vector menu options
		cout << "Select option: \n\n";

		cout << "1) " << fileContent[0][0] << " " << fileContent[0][1] << "$\n";
		cout << "2) " << fileContent[1][0] << " " << fileContent[1][1] << "$\n";
		cout << "3) " << fileContent[2][0] << " " << fileContent[2][1] << "$\n\n";
		cout << "4) view cart\n";
		cout << "5) Proceed to payment\n";
		cout << "6) Back\n";

		//for (vector<string> row : fileContent) {//FOR loop that iterates through each line of the vector array
		//	cout << i++ << ") " << row.at(0) << " $" << row.at(1) << "\n";
		//}
		//cout
		//	<< "\n"
		//	<< i++ << ") view cart\n"
		//	<< i++ << ") Proceed to payment\n"
		//	<< i++ << ") Back\n"
		//	;

		//creates assigned variables from vector to be pushed into 'cart.csv' file
		string itemOne = fileContent[0][0];
		string itemTwo = fileContent[1][0];
		string itemThree = fileContent[2][0];
		float itemOnePrice = stof(fileContent[0][1]);
		float itemTwoPrice = stof(fileContent[1][1]);
		float itemThreePrice = stof(fileContent[2][1]);
		double quantityOne, quantityTwo, quantityThree;
		double totalPrice; //multiplies quantity with price in each switch case, and outputs equation to 'cart.csv' file.

		cin >> userChoice;
		menuDataBase.close();

		cart.open("cart.csv", ios::out | ios::app | ios::in | ios::beg);

		switch (userChoice) {
		case 1:
			//
			cout << "Enter Quantity: ";
			cin >> quantityOne;
			totalPrice = quantityOne * itemOnePrice;
			cout << totalPrice;
			cart << itemOne << "," << quantityOne << "," << totalPrice << '\n';
			cart.close();
			cout << "Added to Cart\n";
			break;
		case 2:
			//
			cout << "Enter Quantity: ";
			cin >> quantityTwo;
			totalPrice = quantityTwo * itemTwoPrice;
			cout << totalPrice;
			cart << itemTwo << "," << quantityTwo << "," << totalPrice << '\n';
			cart.close();
			cout << "Added to Cart\n";

			break;
		case 3:
			cout << "Enter Quantity: ";
			cin >> quantityThree;
			totalPrice = quantityThree * itemThreePrice;
			cout << totalPrice;
			cart << itemThree << "," << quantityThree << "," << totalPrice << '\n';
			cart.close();
			cout << "Added to Cart\n";
			break;
		case 4:
			cartOpen(username);
			break;
		case 5:
			menuDataBase.close();
			paymentWindow(username);
			break;
		case 6:
			menuDataBase.close();
			return;
		default:
			cout << "Please select a valid option.\n";
			break;
		}
	} while (userChoice != 7);
}

void cartOpen(string username) {
	//outputs cart content to user and gives user the option to proceed to payment, clear cart or return to order menu.

	string line, word;
	vector<string> row;
	vector<vector<string>> fileContent;
	int userChoice;
	fstream cart;

	cart.open("cart.csv", ios::in | ios::beg);
	if (!cart.is_open()) {
		cout << "file not open";
		return;
	}

	//iterate through file and pushes data into 'filecontent' vector
	while (getline(cart, line, '\n')) {
		row.clear();
		stringstream stream(line);
		while (getline(stream, word, ',')) {
			row.push_back(word);
		}
		fileContent.push_back(row);
	}

	cout << "My Cart: \n";
	cout << "Item  " << "Quantity " << "Price\n";

	//outputs the filecontent to the user
	for (int i = 0; i < fileContent.size(); i++) {
		for (int j = 0; j < fileContent[i].size(); j++) {
			cout << fileContent[i][j] << "\t";
		}
		cout << "\n";
	}

	cout << "\n\n";
	cout << "1)Proceed to payment\n2)Clear cart\n3)Back to order menu\n";

	cin >> userChoice;

	switch (userChoice) {
	case 1:
		cart.close();
		paymentWindow(username);
		break;
	case 2:
		cart.close();
		cart.open("cart.csv", ios::out | ios::trunc); //clears database
		if (!cart.is_open()) {
			cout << "file not open";
			return;
		}
		cout << "Cart has been cleared\n";
		cart.close();
		cartOpen(username);//re-opens cart function with the cart now cleared.
		break;
	case 3:
		cart.close();
		orderMenu(username);
		break;
	}

	cart.close();

}

void paymentWindow(string username) {
	//retrieve username, cart information and userinputs and pushes into billing database.

	cout << "paymentwindow\n\n\n";
	int userChoice;
	fstream billingDataBase;
	fstream cart;

	cart.open("cart.csv", ios::in); //cart outputs and stores data to be extracted and stored in billing database.
	if (!cart.is_open()) {
		cout << "file not open";
		return;
	}

	vector<float> totalCalc;
	double totalPrice;

	string line, word;
	vector<string> row;
	vector<vector<string>> fileContent;
	while (getline(cart, line, '\n')) {
		row.clear();
		std::stringstream stream(line);

		while (getline(stream, word, ',')) {
			row.push_back(word);
		}

		fileContent.push_back(row);
	}

	cart.close();
	cart.open("cart.csv", ios::out | ios::trunc); //clears cart
	if (!cart.is_open()) {
		cout << "file not open";
		return;
	}
	cart.close();

	for (int i = 0; i < fileContent.size(); i++) {
		totalCalc.push_back(stof(fileContent[i][2])); //extracts price info from 'fileContent' and stores in 'totalCalc' vector
	}

	totalPrice = accumulate(totalCalc.begin(), totalCalc.end(), 0.0); //calculates sum of 'totalcalc' vector and stores in 'totalPrice' variable to then be stored in billingDatabase.

	string date, childName, dietaryPref, paymentStatus;
	int roomNumber;

	cout << "\nEnter the date (dd/mm/yyyy): ";
	cin >> date;
	cout << "\nEnter the child you want to sent the order to: ";
	cin >> childName;
	cout << "\nEnter the child's room number: ";
	cin >> roomNumber;
	cout << "\nEnter any dietary prefences: ";
	cin >> dietaryPref;

	cout << "Do you want to pay now or later?\n";
	cout << "1) Pay now: \n2) Pay later: ";
	cin >> userChoice;

	if (userChoice == 1) {
		cout << "Payment complete.\n";
		paymentStatus = "paid";
	}
	else if (userChoice == 2) {
		cout << "Please complete your order within the billing menu.\n\n";
		paymentStatus = "unpaid";
	}

	billingDataBase.open("billingDataBase.csv", ios::app);
	if (!billingDataBase.is_open()) {
		cout << "file not open";
		return;
	}

	username = removeComma(username);
	date = removeComma(date);
	childName = removeComma(childName);
	dietaryPref = removeComma(dietaryPref);

	billingDataBase << username << "," << date << "," << childName << "," << roomNumber << "," << dietaryPref << "," << totalPrice << "," << paymentStatus << "\n";

	billingDataBase.close();
	userMainMenu(username);
}

void feedbackForm(string username) {
	int userChoice;
	fstream feedbackDataBase("feedbackDataBase.txt", ios::app);

	cout << "****Feedback****\n\n";
	string feedback;

	if (!feedbackDataBase.is_open()) {
		cout << "file is not open\n";
		return;
	}

	cout << "Write your feedback: \n";
	std::getline (std::cin,feedback);
	feedbackDataBase << feedback << "\n";
	cout << "Thank you, your feedback has been sent.\n\n\n";
	feedbackDataBase.close();

	userMainMenu(username);
}

void billingHistory(string username) {

	cout << "\n\n****Billing****\n\n";

	int userChoice;

	cout << "Select an option\n";
	cout << "1) Complete pending payment\n2) View billing history \n3) Return to main menu\n";
	cin >> userChoice;

	switch (userChoice) {
	case 1:
		paymentStatus(username);
		break;
	case 2:

		break;
	case 3:
		userMainMenu(username);
		break;
	}
}

void userBillingHistory(string username) {
	cout << "Your billing history: \n\n";

	vector<vector<string>> fileContent = csvToVector("billingDataBase");

	for (int i = 0; i < fileContent.size(); i++) {
		if (fileContent[i][0] == username) {
			cout << "Date: " << fileContent[i][1] << "\tAmount spent: " << fileContent[i][5] << "\tStatus: " << fileContent[i][6] << endl;
		}
	}
}

void paymentStatus(string username) {

				string paymentStatus;
				float amountOwed;
				int userChoice;
				username = "juzzitube";
	
				vector<vector<string>> fileContent = csvToVector("billingDataBase");
				for (int i = 0; i < fileContent.size(); i++) {
					for (int j = 0; j < fileContent[i].size(); j++) {
						cout << fileContent[i][j] << "\t";
					}
					std::cout << "\n";
				}


				for (int i = 0; i < fileContent.size(); i++) {
					for (int j = 0; j < fileContent.at(i).size(); j++) {

						if (fileContent[i][2] == username) {
							cout << fileContent[i][6];
							paymentStatus = fileContent[i][6];
							cout << paymentStatus;
						}
						if (paymentStatus == "unpaid") {
							cout << "Your payment status is unpaid. You owe: " << fileContent[i][5] << "$\n";
							cout << "Do you want to pay now?\n";
						}


						do {
							cout << "1) Yes\n2) No: ";
							cin >> userChoice;
							switch (userChoice) {
							case 1:
								paymentStatus = "paid";
								fileContent[i][6] = paymentStatus;
								cout << "Payment complete. Your payment status has been updated.\n";
								clearFile("billingDataBase");
								beamVectorToFile(fileContent, "billingDataBase");
								break;
							case 2:
								cout << "Your payment status will remain unpaid\n";
								break;
							default:
								cout << "Please select a valid option\n";
								break;
							}
						} while (userChoice < 0 || userChoice > 2);
					}
					}
				}

//void editMenu() {
//	int i = 1, input;
//	string newVal;
//	vector<vector<string>> menu = csvToVector("menuDataBase");
//	cout << "Current menu: \n";
//	for (vector<string> row : menu) {//FOR loop that iterates through each line of the vector array
//		cout << "(" << i++ << ") " << row.at(0) << "\t$" << row.at(1) << "\n";
//	}
//	cout << "\nWould you like to edit(1) or exit(2)?";
//	cin >> input;
//	switch (input) {
//	case 1:
//		cout << "select which one you want to edit: ";
//		cin >> input;
//
//		i = 1;
//		for (vector<string> row : menu) {
//			if (i == input) {
//				cout << row.at(0) << "\t$" << row.at(1)
//					<< "\nEdit name(1) or price(2)?";
//				cin >> input;
//				switch (input) {
//				case 1:
//					cout << "Please enter a new name: ";
//					cin >> newVal;
//					row.at(0) = newVal;
//					menu[i - 1] = row;
//					clearFile("menuDataBase");
//					beamVectorToFile(menu, "menuDataBase");
//					break;
//
//				case 2:
//					cout << "Please enter a new price (excluding $ character): ";
//					cin >> newVal;
//					row.at(1) = newVal;
//					menu[i - 1] = row;
//					clearFile("menuDataBase");
//					beamVectorToFile(menu, "menuDataBase");
//					break;
//				}
//			}
//			i++;
//		}
//		break;
//	case 2:
//		cout << "Cya admin bro";
//		return;
//	}
//
//
//}







