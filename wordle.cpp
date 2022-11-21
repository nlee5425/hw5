// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here

bool word_validation  ( 
	const string& current,
	const string& floating,
	const std::set<string>& dict
);

void wordle_helper ( 
	const string& in, //word inputted
	string current, //current word
	const string& floating, //the yellow characters string
	int empty_ct,
	const std::set<string>& dict,
	std::set<string>& possible_words //reference to the set of possible solutions available

);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
		string current = ""; //set the current word as an empty string
		std::set<std::string> possible_words;
		bool completed = true;
		int empty_ct = 0;
		//need to check if the word has dashes, symbolizing it is not completed
		for (unsigned int i = 0 ; i < (unsigned int)in.size() ; ++i ) {
			if (in[i] == '-') {
				completed = false;
				++empty_ct;
			}
		}

		if ( in == "") { //empty input therefore there are no solutions 
			return possible_words;
		}
		else if ( completed == true ) { //if the input is completed
			possible_words.insert(in);
		}
		else { //if the cases above are not satisfied, the world still needs to be solved
			wordle_helper(in, current, floating, empty_ct , dict , possible_words );
		}
		return possible_words;
}
bool word_validation  ( 
	const string& current,
	const string& floating,
	const std::set<string>& dict
)
{
	int floating_found = 0; //integer to store the amount of floating characters in the word
	string copy_current = current; // have to make copy since the current word is passed as a const
	for (int i = floating.size() - 1 ; i >= 0 ; --i ) { //start at end of floating 
		char char_floating = floating[i]; //get character at index i inside floating string
		size_t loc = copy_current.find(char_floating); //see if there is location of the character in the current string
		if ( loc != string::npos ) { //if it was found
			copy_current.erase(copy_current.begin() + (int)loc ); //since character was found in current, need to erase from the current, prevent duplication
			++floating_found;
		}

	}

	if (dict.find(current) == dict.end()) { //if the word you are currently lookingat does not exist in the given dictionary, word not valid
		return false;
	}
	if ( (unsigned int)floating_found != floating.size()) { //if the amount of floating words found in current is not same as the size of flating string, word not valid
		return false;
	}
	return true;
}

void wordle_helper ( 
	const string& in, //word inputted
	string current, //current word
	const string& floating, //the yellow characters string
	int empty_ct,
	const std::set<string>& dict,
	std::set<string>& possible_words //reference to the set of possible solutions available

)
{
	if ( current.size() == in.size()) { //base case if the length of the current word recursed is same as input
		if ( word_validation(current, floating, dict) == true ) { //checking if the current word is a valid word
			possible_words.insert(current); //insert the current into the set of possible words
		}
		return;
	}
	if ( in[current.size()] != '-') { //checking if the index you are examining is green in the input, then add
		string copy_current = current + in[current.size()]; //add the char in the input to the current and then recurse
		wordle_helper (in , copy_current , floating , empty_ct , dict , possible_words);                                                                                                                 
	}
	else if ( empty_ct == (int)floating.size()) {
		for (int i = floating.size() - 1 ; i >= 0 ; --i ) {
			string copy_floating = floating;
			string copy_current = current + copy_floating[i];
			copy_floating.erase(i, 1);
			wordle_helper(in , copy_current, copy_floating, empty_ct - 1, dict, possible_words);
		}
	}
	else {
		for (char character = 'a' ; character <= 'z' ; ++character ) { //go through ascii table of all the digits
			string copy_floating = floating;
			if ( copy_floating.find(character) != string::npos ) { //means character exists inside of floating
				copy_floating.erase(copy_floating.find(character) , 1);
			}
			wordle_helper(in, current + character , copy_floating, empty_ct - 1, dict , possible_words );
		}

	}

}




