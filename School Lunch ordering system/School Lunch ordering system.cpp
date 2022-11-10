// School Lunch ordering system.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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


void userMainMenu();

void orderMenu();

void paymentWindow();

void feedbackForm();
	
void billingHistory();

void cartOpen();

void paymentStatus();

	int main()
	{
		
		paymentStatus();
	}

	void userMainMenu() {
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
				orderMenu();
				break;
			case 2:
				billingHistory();
				break;
			case 3:
				feedbackForm();
				break;
			case 4:
				cout << "Logged out";
				break;
			default:
				cout << "Please select a valid option\n";
				break;
			}
		} while (userChoice != 4);
	}

	void orderMenu() {
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
			//outputs filecontent vector has menu options
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
			menuDataBase.close();

			cart.open("cart.csv", ios::out | ios::app | ios::in | ios::beg );

			switch (userChoice) {
			case 1:
				cout << "Enter Quantity: ";
				cin >> quantityOne;
				totalPrice = quantityOne * itemOnePrice;
				cout << totalPrice;
				cart << itemOne << "," << quantityOne << "," << totalPrice << '\n';
				cart.close();
				cout << "Added to Cart\n";
				break;
			case 2:
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
				cartOpen();
				break;
			case 5:
				menuDataBase.close();
				paymentWindow();
				break;
			case 6:
				menuDataBase.close();
				userMainMenu();
			default:
				cout << "Please select a valid option.\n";
				break;
			}
		} while (userChoice != 5 || 6);
	}

	void cartOpen() {
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
			paymentWindow();
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
			cartOpen();//re-opens cart function with the cart now cleared.
			break;
		case 3:
			cart.close();
			orderMenu();
			break;
		}

		cart.close();

	}

	void paymentWindow() {
		//retrieve username, cart information and userinputs and push into billing database.

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

		string username, date, childName, dietaryPref, paymentStatus;
		int roomNumber;

		cout << "Enter your username: ";
		cin >> username;
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
	 
	billingDataBase << username << "," << date << "," << childName << "," << roomNumber << "," << dietaryPref << "," << totalPrice << "," << paymentStatus << "\n";
		
		billingDataBase.close();
		userMainMenu();
	}

	void feedbackForm() {
		int userChoice;
		string feedback;
		fstream feedbackDataBase("feedbackDataBase.txt", ios::app);

		cout << "****Feedback****\n\n";

		if (!feedbackDataBase.is_open()) {
			cout << "file is not open\n";
			return;
		}

		cout << "Write your feedback: \n";
		cin >> feedback;
		feedbackDataBase << feedback << ",";
		cout << "Thank you, your feedback has been sent.\n";
		feedbackDataBase.close();
		return;
	}

	void billingHistory() {

		cout << "\n\n****Billing****\n\n";

		int userChoice;

		cout << "Select an option\n";
		cout << "1) Complete pending payment\n2) View billing history \n3) Return to main menu\n";
		cin >> userChoice;

		switch (userChoice) {
		case 1:
			paymentStatus();
			break;
		case 2: 

			break;
		case 3:
			userMainMenu();
			break;
		}
	}

	void paymentStatus() {

		string username;
		string paymentStatus;
		float amountOwed;
		int userChoice;

		fstream billingDataBase;
		billingDataBase.open("billingDataBase.csv", ios::in | ios::beg);

		string line, word;
		vector<string> row;
		vector <vector<string>> fileContent;

		if (!billingDataBase.is_open()) {
			cout << "File failed to open\n";
		}

		while (getline(billingDataBase, line, '\n')) {
			row.clear();
			stringstream stream(line);
			while (getline(stream, word, ',')) {
				row.push_back(word);
			}
			fileContent.push_back(row);
		}

		cout << "\nEnter your username: ";
		cin >> username;

		for (int i = 0; i < fileContent.size(); i++) {
			if (fileContent[i][0] == username) {
				paymentStatus = fileContent[i][6];
				if (paymentStatus == "unpaid") {
					cout << "Your payment status is unpaid. You owe: " << fileContent[i][5] << "$\n";
					cout << "Do you want to pay now?\n";
					cout << "1) Yes\n2) No: ";
					cin >> userChoice;

					if (userChoice == 1) {
						
					}
				}
			}
		}

		billingDataBase.close();


	}