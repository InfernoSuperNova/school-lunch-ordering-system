// School Lunch ordering system.cpp : This file contains the 'main' function. Program execution begins and ends there.
// up to date as of 11/11/2022 1333

#include <iostream> //Needed to input and output
#include <fstream>	//Enables file functionality 
#include <sstream>	//Enables string functionality for data streams
#include <string>	//Enables string functionality
#include <vector>	//Enables vector functionality
#include <numeric>	//Used to calculcate sum of a vector
#include <cstdio>	
#include <limits>	//Used to determine the end of a file

using std::string;
using std::cout;
using std::cin;
using std::ios;
using std::vector;
using std::fstream;
using std::stringstream;
using std::ofstream;
using std::endl;


//------------------------------------------------HELPER FUNCTIONS-----------------------------------------------//

//Removes all commas in a string
//Arguments: input string with commas
//Returns: output string sans commas
string removeComma(string input);

//Appends two strings in a way that is friendly for CSV formats, removes commas from second string
//Arguments: Initial string, appended string
//Returns: Both strings separated by a comma with existing commas removed form the second string
string appendForTable(string toBeAddedTo, string added);

//Sends a string to a given CSV file
//Arguments: String to send to file, name of file to send to
//Returns: void
void beamStringToFile(string input, string fileToBeamTo);

//Sends a vector of a vector of strings to a given CSV file
//Arguments: vector to beam, name of file to beam to
//Returns: void
void beamVectorToFile(vector<vector<string>> input, string filename);

//Copies all content from one file to another file
//Arguments: File from, file to
//Returns: void
void copyFileContentToFile(string filenameFrom, string filenameTo);

//Copies all content from filename.csv to filename_backup.csv
//Arguments: filename
//Returns: void
void backupFile(string filename);

//Copies all content from filename_backup.csv to filename.csv
//Arguments: filename
//Returns: void
void restoreFileFromBackup(string filename);

//Scorched earth policy
//Arguments: filename
//Returns: void
void clearFile(string filename);

//Returns true if a file is empty
//Arguments: file ifstream
//Returns: true/false
bool isEmpty(std::ifstream& pFile);

//function to convert the CSV database to a 2D vector of strings (and return)
//Arguments: Name of file to open, without extension
//Returns: Vector conversion of CSV document
vector<vector<string>> csvToVector(string fileName);

//--------------------------------------------REGISTRATION FUNCTIONS---------------------------------------------//

//Function for handling login
//Arguments: none
//Returns: void
void login();

//Handles registration of new users
//Arguments: none
//Returns: void
void registerUser();

//Registration of guardians
//Arguments: none
//Returns: guardian user information
string registerGuardian();

//Registration of staff
//Arguments: none
//Returns: Staff user information
string registerStaff();

//Info required for all new users
//Arguments: none
//Returns: basic info separated by commas
string basicInfo();

//Info required by just guardians (in addition to basicInfo)
//Arguments: none
//Returns: child info separated by commas
string childInfo();

//-------------------------------------------------USER FUNCTIONS------------------------------------------------//

//Main menu for users, acts as a "home base" of sorts
//Arguments: username
//Returns: void
void userMainMenu(string username);

//Ordering menu for users
//Arguments: username
//Returns: none
void orderMenu(string username);

//Handles user payment and places it in the database
//Arguments: Username
//Returns: void
void paymentWindow(string username);

//Takes in user input and sends it to the feedback plain text document
//Arguments: username
//Returns: void
void feedbackForm(string username);

//Billing menu for users
//Arguments: username
//Returns: void
void billingHistory(string username);

//shows the billing history of a user with a given username
//Arguments: username
//Returns: void
void viewBillingHistory(string username);

//Outputs cart content to user and gives user the option to proceed to payment, clear cart or return to order menu.
//Arguments: username
//Returns: void
void cartOpen(string username);

//Handles payment
//Arguments: username
//Returns: void
void paymentStatus(string username);

//------------------------------------------------ADMIN FUNCTIONS------------------------------------------------//

//Main menu for admins, acts as a "home base" of sorts
//Arguments: none
//Returns: void
void adminMainMenu();

//Groups sale reports by day and allows you to resolve them
//Arguments: none
//Returns: void
void dailySaleReports();

//Groups sale reports by month and allows you to resolve them
//Arguments: none
//Returns: void
void monthlySaleReports();

//Function for editing the menu
//Arguments: none
//Returns: void
void editMenu();

//Function for viewing (and optionally clearing) the feedback text file
//Arguments: none
//Returns: void
void viewFeedback();


//--------------------------------------------------MAIN FUNCTION------------------------------------------------//


int main()
{
	bool loop = true;
	int selection;
	while (true) {
		system("cls");
		cout << "School lunch ordering system\n\nWould you like to:\n(1)Register\n(2)Login\n(3)Quit\n";
		cin >> selection;
		switch (selection) {
		case 1:
			registerUser();
			break;
		case 2:
			login();
			break;
		case 3:
			return 0;
		default:
			cout << "\nThat is not a valid response!\n";
		}
	}
}


//------------------------------------------------HELPER FUNCTIONS-----------------------------------------------//


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


string appendForTable(string toBeAddedTo, string added) {
	added = removeComma(added);
	toBeAddedTo.append(",");
	toBeAddedTo.append(added);
	return toBeAddedTo;
}


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


void copyFileContentToFile(string filenameFrom, string filenameTo) {
	vector<vector<string>> temp = csvToVector(filenameFrom);
	beamVectorToFile(temp, filenameTo);
}


void backupFile(string filename) {
	clearFile(filename + "_backup");
	copyFileContentToFile(filename, filename + "_backup");
}


void restoreFileFromBackup(string filename) {
	copyFileContentToFile(filename + "_backup", filename);
}


void clearFile(string filename) {
	fstream file;
	file.open(filename + ".csv", ios::out | ios::trunc);
	file.close();
}


bool isEmpty(std::ifstream& pFile)
{
	return pFile.peek() == std::ifstream::traits_type::eof();
}



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


//--------------------------------------------REGISTRATION FUNCTIONS---------------------------------------------//


void login() {
	fstream adminCredentials;
	vector<string> row;
	adminCredentials.open("userDataBase.csv", ios::in | ios::app);
	string username, password;

	//call createContentVector before the credentials are given for the sake of performance if we were to put the login into a "While not logged in"
	vector<vector<string>> content = csvToVector("userDataBase");

	cout << "Please enter your username: ";
	cin >> username;
	cout << "\nPlease enter your password: ";
	cin >> password;
	system("cls");
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
				adminMainMenu();
			}
		}
	}
}


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
			break;
		case 2:
			info = registerStaff();
			loop = false;
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


string registerGuardian() {
	string info, childInfo2;
	info = basicInfo();
	info = info + childInfo();
	return info;
}


string registerStaff() {
	string info;
	info = basicInfo();
	return info;
}


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


//-------------------------------------------------USER FUNCTIONS------------------------------------------------//


void userMainMenu(string username) {
	//Outputs main menu options to a logged in user.

	int userChoice;

	do {
		cout << "\nMain Menu\n\n";

		cout << "Select option: \n";
		cout << "1)Order menu\n2)Billing\n3)Provide Feedback\n4)Logout\n";
		cin >> userChoice;
			

		switch (userChoice) {
		case 1:
			//Takes user to the order menu.
			orderMenu(username);
			break;
		case 2:
			//Takes user to view their 'billing history' 
			billingHistory(username);
			break;
		case 3:
			//Takes user to where they can provide feedback.
			feedbackForm(username);
			break;
		case 4:
			//Takes user back to login.
			cout << "Logged out\n";
			main();
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
	system("cls");
	do {
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

		//outputs three menu items and their prices.
		cout << "Select option: \n\n";

		cout << "1) " << fileContent[0][0] << " " << fileContent[0][1] << "$\n";
		cout << "2) " << fileContent[1][0] << " " << fileContent[1][1] << "$\n";
		cout << "3) " << fileContent[2][0] << " " << fileContent[2][1] << "$\n\n";
		cout << "4) view cart\n";
		cout << "5) Proceed to payment\n";
		cout << "6) Back\n";

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
		system("cls");
		menuDataBase.close();

		cart.open("cart.csv", ios::app | ios::in);

		switch (userChoice) {
		case 1:
			// Case 1, 2 and 3 match menu items 1, 2 and 3. Each item's price is multiplied by quanity and pushed into the cart csv file.
			cout << "Enter Quantity: ";
			cin >> quantityOne;
			totalPrice = quantityOne * itemOnePrice;
			//cout << "\n" << "$" << totalPrice;
			cart << itemOne << "," << quantityOne << "," << totalPrice << '\n';
			cart.close();
			cout << "\n\nAdded to Cart\n\n\n";
			break;
		case 2:
			//
			cout << "Enter Quantity: ";
			cin >> quantityTwo;
			totalPrice = quantityTwo * itemTwoPrice;
			//cout << totalPrice;
			cart << itemTwo << "," << quantityTwo << "," << totalPrice << '\n';
			cart.close();
			cout << "\n\nAdded to Cart\n\n\n";
			break;
		case 3:
			cout << "Enter Quantity: ";
			cin >> quantityThree;
			totalPrice = quantityThree * itemThreePrice;
			//cout << totalPrice;
			cart << itemThree << "," << quantityThree << "," << totalPrice << '\n';
			cart.close();
			cout << "\n\nAdded to Cart\n\n\n";
			break;
		case 4:
			//Allows user's to view their cart
			cartOpen(username);
			break;
		case 5:
			//Proceeds user to payment window once confirming their menu items.
			menuDataBase.close();
			paymentWindow(username);
			break;
		case 6:
			//returns user to the main menu.
			menuDataBase.close();
			return;
		default:
			cout << "Please select a valid option.\n";
			break;
		}
	} while (userChoice != 6);
}


void paymentWindow(string username) {
	//retrieve username, cart information and userinputs and pushes data into billing database.

	system("cls");
	cout << "\nPayment Window\n\n";
	int userChoice;
	fstream billingDataBase;
	fstream cart;
	char anyKey;

	cart.open("cart.csv", ios::in); //cart outputs and stores data to be extracted and stored in billing database.
	if (!cart.is_open()) {
		cout << "file not open";
		return;
	}

	vector<float> totalCalc;
	double totalPrice;

	//pushes cart information into 'filecontent' vector.
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
	cart.open("cart.csv", ios::out | ios::trunc); //clears cart and closes it.
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

	//prompts user inputs to be stored in billingDataBase
	cout << "\nEnter the date (dd/mm/yyyy): ";
	cin >> date;
	cout << "\nEnter the child you want to send the order to: ";
	cin >> childName;
	cout << "\nEnter the child's room number: ";
	cin >> roomNumber;
	cout << "\nEnter any dietary prefences: ";
	cin >> dietaryPref;

	//Prompts user to decide whether to pay now or later.
	do {
		cout << "Do you want to pay now or later?\n";
		cout << "1) Pay now: \n2) Pay later: ";
		cin >> userChoice;
		//assigns paymentStatus to 'paid' or 'unpaid' depending on userChoice.
		switch (userChoice) {
		case 1:
			cout << "Payment complete.\n";
			paymentStatus = "paid";
			cout << "Press any key to exit\n";
			cin >> anyKey;
			break;
		case 2:
			cout << "Please complete your order in the Billing menu.\n\n";
			paymentStatus = "unpaid";
			break;
		default:
			cout << "Please select a valid option\n";
			break;
		}
	} while (userChoice < 1 || userChoice > 2);

	//opens billingDataBase file.
	billingDataBase.open("billingDataBase.csv", ios::app);
	if (!billingDataBase.is_open()) {
		cout << "file not open";
		return;
	}

	//removes any user inputted commas be using the 'removeComma' function.
	username = removeComma(username);
	date = removeComma(date);
	childName = removeComma(childName);
	dietaryPref = removeComma(dietaryPref);

	//pushes all required data into the billingDataBase
	billingDataBase << username << "," << date << "," << childName << "," << roomNumber << "," << dietaryPref << "," << totalPrice << "," << paymentStatus << "\n";

	billingDataBase.close();
	//Sends user back to the main menu.
	userMainMenu(username);
}


void feedbackForm(string username) {
	char userChoice;
	//opens the feedbackDataBase
	fstream feedbackDataBase("feedbackDataBase.txt", ios::app);

	cout << "****Feedback****\n\n";
	string feedback = "bogus";

	if (!feedbackDataBase.is_open()) {
		cout << "file is not open\n";
		return;
	}

	cout << "Please write your feedback: \n";
	//'getline' function used to enable user inputted spacing when pushing to file.

	std::cin.ignore(INT_MAX, '\n');
	getline(cin, feedback);

	//pushes user feedback into file.
	feedbackDataBase << username << "," << feedback << "\n";
	cout << "Thank you, your feedback has been sent.\n\n\n";
	feedbackDataBase.close();

	cout << "Press any key to return to main menu\n";
	cin >> userChoice;

	return;
}


void billingHistory(string username) {

	system("cls");
	cout << "\n****Billing****\n\n";

	int userChoice;

	//outputs options to user.
	cout << "Select an option\n";
	cout << "1) Complete pending payment\n2) View billing history \n3) Return to main menu\n";

	do {
		cin >> userChoice;
		switch (userChoice) {
		case 1:
			//Takes user to view their payment status 
			paymentStatus(username);
			break;
		case 2:
			//Takes user to view their billing history
			viewBillingHistory(username);
			break;
		case 3:
			//Returns user to main menu.
			userMainMenu(username);
			break;
		default:
			cout << "Please choose a valid option.\n";
		}
	} while (userChoice < 1 || userChoice > 3);
}


void viewBillingHistory(string username) {
	system("cls");
	cout << "Your billing history: \n\n";
	char userChoice;
	//Extracts data from billingDataBase file and pushes it int 'filecontent' vector using csvToVector function.
	vector<vector<string>> fileContent = csvToVector("billingDataBase");

	//Iterates through 'filecontent' and checks for column matching username.
	for (int i = 0; i < fileContent.size(); i++) {
		if (fileContent[i][0] == username) {
			//Outputs user's billing history.
			cout << "Date: " << fileContent[i][1] << "\tAmount spent: " << fileContent[i][5] << "\tStatus: " << fileContent[i][6] << endl;
		}
	}
	//Prompts user to go back to main menu when user is ready.
	cout << "\nPress any key to go back\n";
	cin >> userChoice;
	billingHistory(username);
}


void cartOpen(string username) {
	//outputs cart content to user and gives user the option to proceed to payment, clear cart or return to order menu.

	string line, word;
	vector<string> row;
	vector<vector<string>> fileContent;
	int userChoice;
	fstream cart;
	fileContent = csvToVector("cart");

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
	cout << "Item  " << "\tQuantity " << "Price\n";

	//outputs the filecontent to the user
	for (int i = 0; i < fileContent.size(); i++) {
		for (int j = 0; j < fileContent[i].size(); j++) {
			cout << fileContent[i][j] << "\t";
		}
		cout << "\n";
	}

	cout << "\n\n";
	cout << "1)Proceed to payment\n2)Clear cart\n3)Back to order menu\n";
	do {
		cin >> userChoice;

		switch (userChoice) {
		case 1:
			//Allows user to confirm their cart choices and proceed to payment.
			cart.close();
			if (fileContent.size() == 0) {
				cout << "\n\nYour have nothing in the cart, returning to order menu.\n";
				return;
			}
			paymentWindow(username);
			break;
		case 2:
			//Clears all items from the cart.
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
			//Returns user to order menu.
			cart.close();
			return;
			break;
		default:
			cout << "Please Choose a valid option.\n";
			break;
		}
	} while (userChoice < 1 || userChoice > 3); //Loops through switch case until a valid choice is made.

	cart.close();
}


void paymentStatus(string username) {

	system("cls");
	string paymentStatus;
	float amountOwed;
	int userChoice;
	char anyKey;


	//Extracts billingDataBase file and pushes it into 'filecontent' vector using 'csvToVector' function.
	vector<vector<string>> fileContent = csvToVector("billingDataBase");

	//iterates through fileContent until a collumn matches the user's 'username'.
	for (int i = 0; i < fileContent.size(); i++) {
		for (int j = 0; j < fileContent.at(i).size(); j++) {

			if (fileContent[i][0] == username) {

				//assigns the user's paymentstatus in vector to the paymentStatus variable.
				paymentStatus = fileContent[i][6];

				//checks paymentstatus and prompts user with 2 scenario's depending on whether its 'unpaid' or 'paid'
				if (paymentStatus == "unpaid") {

					//outputs options to user if 'unpaid'.
					cout << "Your payment status is unpaid. You owe: " << fileContent[i][5] << "$\n";
					cout << "Do you want to pay now?\n";
					cout << "1) Yes\n2) No: ";
					cin >> userChoice;

					do {
						switch (userChoice) {
							//if user chooses to pay, paymentstatus is changed and replaces old status in the 'fileContent'
						case 1:
							paymentStatus = "paid";
							fileContent[i][6] = paymentStatus;
							cout << "Payment complete. Your payment status has been updated.\n";
							clearFile("billingDataBase");
							beamVectorToFile(fileContent, "billingDataBase");
							break;
						case 2:
							//outputs message to user if they choose not to pay.
							cout << "Your payment status will remain unpaid\n";
							return;
						default:
							cout << "Please select a valid option\n";
							break;
						}
					} while (userChoice < 0 || userChoice > 2);
				}

				//Outputs message to user if they're already paid and prompts them to return to billing menu.
				else if (paymentStatus == "paid") {
					cout << "You have no outstanding payments to pay\n\n";
					cout << "Press any key to go back.\n";
					cin >> anyKey;
					billingHistory(username);
				}
			}
		}
	}
}


//------------------------------------------------ADMIN FUNCTIONS------------------------------------------------//


void adminMainMenu() {
	int userChoice = 0;
	while (userChoice < 1 || userChoice > 5) {

		while (true) {
			cout << "Select option:\n1)View daily sale reports\n2)View monthy sale reports\n3)Edit menu options\n4)View Feedback\n5)Logout\n";
			cin >> userChoice;
			switch (userChoice) {
			case 1:
				dailySaleReports();
				break;
			case 2:
				monthlySaleReports();
				break;
			case 3:
				editMenu();
				break;
			case 4:
				viewFeedback();
				break;
			case 5:
				return;
			default:
				cout << "Please select a valid option\n";
				break;
			}
		}
	}
}


void dailySaleReports() {
	bool check = false;
	int userChoice;
	string dateSelected;
	vector<vector<string>> database = csvToVector("billingDataBase");
	vector<string> datesToPrint; //stores the applicable dates that can be printed out

	//loops through dates and adds them to the applicable dates vector
	for (vector<string> row : database) {
		check = false;
		//loop through dates to print
		for (int i = 0; i < datesToPrint.size(); i++) {
			//if the current item in rows matches any of the stored items in datesToPrint...
			if (row.at(1) == datesToPrint.at(i)) {
				//set check to true
				check = true;
			}
		}
		//If the check switch was never triggered, then
		if (check == false) {
			//add the date to the list of dates to print
			datesToPrint.push_back(row.at(1));
		}
	}
	//all of this just to remove duplicate entries


	//date printing
	cout << "Please select a date: \n";
	for (int i = 0; i < datesToPrint.size(); i++) {
		cout << "\n" << i + 1 << ") " << datesToPrint.at(i);
	}
	cout << "\nSelection: ";

	//date selection
	cin >> userChoice;
	for (int i = 0; i < datesToPrint.size(); i++) {
		if (userChoice == i + 1) {
			dateSelected = datesToPrint.at(i);
		}
	}
	dateSelected = datesToPrint.at(userChoice - 1);

	//info printing
	for (vector<string> row : database) {

		//cout << row.at(1) << " == " << dateSelected;
		if (row.at(1) == dateSelected) {
			cout << "\n";
			for (int i = 0; i < row.size(); i++) {
				cout << row.at(i) << "\t";
			}
		}
	}
	cout << "\n\n";
}


void monthlySaleReports() {
	bool check = false;
	int userChoice;
	string dateSelected;
	vector<vector<string>> database = csvToVector("billingDataBase");
	vector<string> datesToPrint; //stores the applicable dates that can be printed out

	//loops through dates and adds them to the applicable dates vector
	for (vector<string> row : database) {
		check = false;
		//loop through dates to print
		for (int i = 0; i < datesToPrint.size(); i++) {
			//if the current item in rows matches any of the stored items in datesToPrint...
			//substring(3, 7) creates a new string starting at index 3 and continuing for 7 chars (IE, removing the day)
			if (row.at(1).substr(3, 7) == datesToPrint.at(i)) {
				//set check to true
				check = true;
			}
		}
		//If the check switch was never triggered, then
		if (check == false) {
			//add the date to the list of dates to print
			datesToPrint.push_back(row.at(1).substr(3, 7));
		}
	}
	//all of this just to remove duplicate entries


	//date printing
	cout << "Please select a date: \n";
	for (int i = 0; i < datesToPrint.size(); i++) {
		cout << "\n" << i + 1 << ") " << datesToPrint.at(i);
	}
	cout << "\nSelection: ";

	//date selection
	cin >> userChoice;
	for (int i = 0; i < datesToPrint.size(); i++) {
		if (userChoice == i + 1) {
			dateSelected = datesToPrint.at(i);
		}
	}

	//info printing
	for (vector<string> row : database) {

		//cout << row.at(1) << " == " << dateSelected;
		if (row.at(1).substr(3, 7) == dateSelected) {
			cout << "\n";
			for (int i = 0; i < row.size(); i++) {
				cout << row.at(i) << "\t";
			}
		}
	}
	cout << "\n\n";
}


void editMenu() {
	int i = 1, input;
	string newVal;
	vector<vector<string>> menu = csvToVector("menuDataBase");

	cout << "Current menu: \n";
	for (vector<string> row : menu) {//FOR loop that iterates through each line of the vector array
		cout << "(" << i++ << ") " << row.at(0) << "\t$" << row.at(1) << "\n";
	}
	cout << "\nWould you like to edit(1) or exit(2)?";
	cin >> input;
	switch (input) {
	case 1:
		cout << "select which one you want to edit: ";
		cin >> input;

		i = 1;
		for (vector<string> row : menu) {
			if (i == input) {
				cout << row.at(0) << "\t$" << row.at(1)
					<< "\nEdit name(1) or price(2)?";
				cin >> input;
				switch (input) {
				case 1:
					cout << "Please enter a new name: ";
					cin >> newVal;
					row.at(0) = newVal;
					menu[i - 1] = row;
					clearFile("menuDataBase");
					beamVectorToFile(menu, "menuDataBase");
					break;

				case 2:
					cout << "Please enter a new price (excluding $ character): ";
					cin >> newVal;
					row.at(1) = newVal;
					menu[i - 1] = row;
					clearFile("menuDataBase");
					beamVectorToFile(menu, "menuDataBase");
					break;
				}
			}
			i++;
		}
		break;
	case 2:
		cout << "Cya admin bro";
		return;
	}
}


void viewFeedback() {
	char input = 'n';
	std::ifstream feedback("feedbackDataBase.txt");
	if (!feedback.is_open()) {
		cout << "Error (feedback()), file is not open, exiting!\n";
		return;

	}
	if (isEmpty(feedback)) {
		cout << "No feedback to view!\n";
		return;
	}

	//outputs EVERYTHING in the file
	cout << feedback.rdbuf();
	while (true) {
		cout << "\n\nClear feedback? y/n: ";
		cin >> input;
		switch (input) {
		case 'y':
			feedback.close();
			//clear the file
			feedback.open("feedbackDataBase.txt", ios::out | ios::trunc);
			feedback.close();
			return;
		case 'n':
			feedback.close();
			return;
		default:
			cout << "That is not a valid response!\n";
			break;
		}
	}
}
