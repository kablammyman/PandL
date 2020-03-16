#include "StringUtils.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>      // std::setprecision
#include <map>

#ifdef _WIN32
char slash = '\\';
#else
char slash = '/';
#endif

using namespace std;

void parseArgs(int argc, const char* argv[], string& inputFile, string& outputDir)
{
	for (int i = 0; i < argc; i++)
	{
		string curArg = argv[i];

		if (curArg == "-i" || curArg == "--input")
		{
			if (i < argc - 1)
			{
				inputFile = argv[i + 1];
				i++;

				size_t found = inputFile.find_last_of("/\\");
				if (found == string::npos)
				{
					string mainPath = argv[0];
					inputFile = mainPath.substr(0, mainPath.find_last_of("/\\") + 1) + inputFile;
				}
			}
		}
		else if (curArg == "-o" || curArg == "--output")
		{
			if (i < argc - 1)
			{
				outputDir = argv[i + 1];
				i++;

				size_t found = outputDir.find_last_of("/\\");
				bool inSubdir = (outputDir[0] == '.' && outputDir.find_first_of("/\\") == 1);

				if (found == string::npos || inSubdir)
				{
					if (inSubdir)
					{
						outputDir.erase(0, 2);
					}
					string mainPath = argv[0];
					outputDir = mainPath.substr(0, mainPath.find_last_of("/\\") + 1) + outputDir;
				}
			}
		}
	}//end for loop
}
struct TransactionItem
{
	float amount;
	string name;
	string category;
};
int main(int argc, const char* argv[])
{
	vector<TransactionItem> expense;
	vector<TransactionItem> income;
	//vector<TransactionItem> ccPayments;
	map<string, float> expensesMap;
	map<string, float> incomeMap;
	map<string, float>::iterator it;

	string fileName = "PandL StatemntFor 2019.txt";
	string inputFile, outputDir;
	float creditCardPayments = 0.0f;
	float totalIncome = 0.0f;
	float totalExpenses = 0.0f;

	parseArgs(argc, argv, inputFile, outputDir);
	
	//read in the master csv...shjould be combined with cc and business acount csv
	string line;
	ifstream myfile(inputFile);

	int lineNum = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			lineNum++;
			if (lineNum == 1)//ignore first line
			{
				continue;
			}
			vector<string> tokens = StringUtils::Tokenize(line, ",");
			// later on move these toparams or something
			//Details,Posting Date,Description,Amount,Type,Balance,Check or Slip #,Category
			TransactionItem newItem;
			newItem.amount = stof(tokens[3]);
			newItem.name = tokens[2];
			newItem.category = tokens[6];
			

			if (newItem.category == "payment to card" && tokens[0] == "DEBIT")
			{
				creditCardPayments += newItem.amount;
			}
			else if (newItem.amount <= 0 || tokens[0] == "DEBIT")
			{
				expense.push_back(newItem);
				expensesMap[newItem.category] += newItem.amount;
			}
			else if(newItem.category != "payment to card")
			{
				income.push_back(newItem);
				incomeMap[newItem.category] += newItem.amount;
			}
		}
		myfile.close();
	}

	else
	{
		cout << "Unable to open input csv file: " <<inputFile <<endl;
		exit(0);
	}
	
	ofstream file;
	cout << "writing to " << outputDir +slash+ fileName << endl;
	file.open(outputDir+slash+fileName);
	if (!file.is_open())
	{
		cout << "Unable to open file " << outputDir + slash + fileName << endl;;
		exit(0);
	}
	file << "Profit and Loss statement for  2019" << endl;
	file << endl;
	file << "Income\n";
	file << "------------------------------------------------------------------------------\n";
	for (it = incomeMap.begin(); it != incomeMap.end(); it++)
	{
		if (it->second >= 0.0)
		{
			file << it->first  // string (key)
				<< ": $"
				<< std::fixed
				<< it->second
				<< setprecision(2)
				<< std::endl;
			totalIncome += it->second;
		}
	}
	file << "--------------\n";
	file << "Total Income: $" << totalIncome << endl;
	file << "------------------------------------------------------------------------------\n";
	file << "Expenses\n";
	file << "--------------\n";
	//now expenses
	for (it = expensesMap.begin(); it != expensesMap.end(); it++)
	{
		if (it->second < 0.0)
		{
			file << it->first  // string (key)
				<< ": $"
				<< std::fixed
				<< it->second   // string's value 
				<< setprecision(2)
				<< std::endl;
			totalExpenses += it->second;
		}
	}
	file << "--------------\n";
	file << "Total Expenses: $" << std::fixed << totalExpenses * -1 << setprecision(2) << endl;
	file << "------------------------------------------------------------------------------\n";
	file << "Income Details\n";
	file << "--------------\n";
	totalIncome = 0.0f;
	//income part first
	for (size_t i = 0; i < income.size(); i++)
	{
		file << income[i].name  // string (key)
		<< ": $"
		<< std::fixed
		<< income[i].amount
		<< setprecision(2)
		<< std::endl;
		totalIncome += income[i].amount;

	}
	file << "--------------\n";
	file << "Total Income: $" << totalIncome << endl;
	file << "------------------------------------------------------------------------------\n";
	file << "Expenses Details\n";
	file << "--------------\n";
	totalExpenses = 0.0f;
	//now expenses
	for (size_t i = 0; i < expense.size(); i++)
	{
		file << expense[i].name
		<< ": $"
		<< std::fixed
		<< expense[i].amount
		<< setprecision(2)
		<< std::endl;
		totalExpenses += expense[i].amount;
	}
	file << "--------------\n";
	file << "Total Expenses: $" << std::fixed << totalExpenses*-1 << setprecision(2) << endl;
	file << "------------------------------------------------------------------------------\n";
	file << "credit cardy payments: $" << std::fixed << creditCardPayments*-1 << setprecision(2) << " (this value is not included in other calculations) " << endl;
	file << "------------------------------------------------------------------------------\n";
	file << "Net Income: $" << std::fixed << (totalIncome + totalExpenses) << setprecision(2) << endl;
	file.close();
	return 0;
}