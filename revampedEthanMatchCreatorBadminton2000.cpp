#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <cctype>
#include <sstream>
#include <cstdlib>

/*
DEV LOG:
Day 0 (2/14/2026): Made ideas of normal round-robin matchmaking system and saving data. Learned about lambda functions to use
the algorithm library for std::tolower. Made the 'info' option.
Day 1  (2/15/2026): Made a continuation helper function, thoroughly checks input compared to other functions. Started commenting.
Started working on normal round-robin matchmaking by setting the area for it, making it so we get the list of player names & declaration of round #
(Note to self: maybe add a "quit" option to the check_continuation method)
Day 2 (2/16/2026): Made a check word function so you can check A or B. Changed some stuff to implement it.
Day 3 (2/26/2026): Setted up Github on computer for laptop to be able to access.
Day 4 (2/28/2026): Burned the shifted_player_list and replaced it with something that I need to change later as it doesn't work.
Day 5 (7/20/2026): Scrapped the entirety of the old round-robin system as I'm too lazy to reunderstand my old thought process
Day 6 (8/5/2026): Redid my normal round-robian matchmaking, now a lot simpler. Works but probably will implement some tweaks for design.
Day 6.1 (8/5/2026): Realized lots of flaws, turned "B" into a string of random digits, cleaned things up a bit, going on break.
*/

std::ifstream fin("badmintonRecords.txt");
std::ofstream fout("badmintonRecords.txt", std::ios::app); // std::ios::app means for input out put string, making it do append instead of truncating

// big functions
void normal_round_robin_matchmaker(std::vector<std::string> player_list);

// helper functions
void fetch_data(std::string information); // going to be used for the save function and records
void check_continuation(bool& still_using, std::string continuation, bool& repeat_continuation); // used to see if they wish to continue or not
int check_word(std::string specific_user_pick, std::string user_option1, std::string user_option2); // variation of check_continuation but for A or B
bool simplified_continuation_once(std::string word, std::string user_input); // checks once if user input matches a word
void nrr_player_list_shift(std::vector<std::string>& player_list, bool parity_odd); // moves everything but first guy in a circle (and the bye stays the same if there)

int main() {
    bool still_using = true; // variable made to keep this running
    bool first_run = true; // just for the welcome statement
    bool repeat_continuation = false; // for the check_continuation method
    std::string continuation; // input for continuation
    std::string user_pick; // input for option chosen
    std::string matchmaking_type; // input for type of matchmaking
    std::string round_robin_format; // input for type of round-robin
    std::string player_names; // string of player names thats gonna be turned into a list
    std::string placeholder_of_ss_player_names; // going to be used for putting player_names into a list
    std::vector<std::string> player_list;
    while (still_using) {
        if (first_run) {
            std::cout << "Welcome to ELC's Badminton Program!" << std::endl;
            first_run = false;
        }
        std::cout << "\nWhat do you wish to do? (Type: 'info' for options): " << std::endl;
        std::cin >> user_pick;
        /*
        below is what I used to have like w/o some continuation functions
        // funky lambda function to lowercaseify, frequent in this program
        std::transform(user_pick.begin(), user_pick.end(), user_pick.begin(), [](unsigned char hopefullyThisWork){return std::tolower(hopefullyThisWork);}); // hopefullyThisWork = variable for lower
        */
        if (simplified_continuation_once("info", user_pick)) {
            std::cout << "\nOptions (P.S. The first letter of each option also is valid): " << std::endl;
            std::cout << "'info' - Gives available options" << std::endl << "'records' - Allows you to view previous matches" << std::endl;
            std::cout << "'save' - Saves match information and sends it to records" << std::endl << "'matchmaking' - Creates a type of matchmaking format" << std::endl;
            std::cout << "\nDo you wish to continue or end?: " << std::endl;
            std::cin >> continuation;
            check_continuation(still_using, continuation, repeat_continuation);
        } 

        else if (simplified_continuation_once("matchmaking", user_pick)) { // does matchmaking
            std::cout << "\nWhat type of matchmaking event is this for? (Tryouts or Practice): " << std::endl; // allows choosing of matchmaking format
            std::cin >> matchmaking_type;
            int matchmaking_type_int = check_word(matchmaking_type, "tryouts", "practice");
            if (matchmaking_type_int == 1) { // if tryouts
                std::cout << "Do you wish for a normal or greedy round-robin format?" << std::endl;
                std::cin >> round_robin_format;
                int round_robin_format_int = check_word(round_robin_format, "normal round-robin", "greedy round-robin");
                if (round_robin_format_int == 1) { // if normal round-robin
                    std::cout << "Who are the players? (Input them in this format: 'name, name, name'): " << std::endl; // gets name of players
                    std::getline(std::cin >> std::ws, player_names);
                    std::stringstream stringstream_player_names(player_names);
                    int player_names_counter_for_ws = 0;
                    while(std::getline(stringstream_player_names, placeholder_of_ss_player_names, ',')) {
                        if (player_names_counter_for_ws >= 1) {
                            placeholder_of_ss_player_names.erase(0,1);
                        }
                        player_list.push_back(placeholder_of_ss_player_names);
                        player_names_counter_for_ws++;
                    } // this loop makes it so that we get rid of the whitespaces at the beginning and also add names to the list   
                    normal_round_robin_matchmaker(player_list);
                }
            }
        }
    }
    return 0;
}

bool simplified_continuation_once(std::string word, std::string user_input) {
    std::transform(user_input.begin(), user_input.end(), user_input.begin(), [](unsigned char lower_char){return std::tolower(lower_char);});
    std::string comparison_string = "";
    for (char character : word) {
        comparison_string += character;
        if (comparison_string == user_input) {
            return true;
        }
    }
    return false;
}

void check_continuation(bool& still_using, std::string continuation, bool& repeat_continuation) {
    // Purpose: Checks if you wish to continue with the program or not
    // P.S. This is probably an overcomplication
    std::transform(continuation.begin(), continuation.end(), continuation.begin(), [](unsigned char convertToLowerForContinuation){return std::tolower(convertToLowerForContinuation);});
    // lowercaseify
    bool continue_not_present = true;
    bool yes_not_present = true;
    std::string all_possible_string_continue;
    std::string all_possible_string_yes;
    std::string word_continue = "continue";
    std::string word_yes = "yes";
    for (int all_possible_string_continue_iterator = 0; all_possible_string_continue_iterator < 8; all_possible_string_continue_iterator++) {
        all_possible_string_continue = all_possible_string_continue + word_continue[all_possible_string_continue_iterator];
        if (continuation == all_possible_string_continue) {
            continue_not_present = false;
        }
    }   // this loop is checking if input is the same as continue or like cont or something like that

    for (int all_possible_string_yes_iterator = 0; all_possible_string_yes_iterator < 3; all_possible_string_yes_iterator++) {
        all_possible_string_yes = all_possible_string_yes + word_yes[all_possible_string_yes_iterator];
        if (continuation == all_possible_string_yes) {
            yes_not_present = false;
        }
    } // this loop is checking if input is yes

    if (continue_not_present == false) {
        repeat_continuation = false;
    } else if (yes_not_present == false) {
        repeat_continuation = false;
    } // this is done just in case if you put in an invalid thing first try so it wont loop forever

    if (continue_not_present && yes_not_present) {
        if (continuation == "n" || continuation == "no" || continuation == "e" || continuation == "en" || continuation == "end" || continuation == "quit" || continuation == "qui" || continuation == "qu" || continuation == "q") {
            still_using = false;
            repeat_continuation = false; // you said no/end and the program is stopping
        } else {
            std::cout << "That's an invalid input! (Do Yes/No OR Continue/End): " << std::endl;
            repeat_continuation = true;
            while (repeat_continuation) {
                std::cin >> continuation;
                check_continuation(still_using, continuation, repeat_continuation);
            }
        } // using recursion so that you put in a valid input of yes/no
    }
}

// two options basically, checks if one or the other and if none
int check_word(std::string specific_user_pick, std::string user_option1, std::string user_option2) { // mainly same logic as check_continuation but more broad usage for A or B
    bool validity_of_pick = false;
    bool user_option1_isTrue = false;
    bool user_option2_isTrue = false;
    std::string user_option1_portion;
    std::string user_option2_portion;
    std::string specific_user_pick_again;
    std::transform(specific_user_pick.begin(), specific_user_pick.end(), specific_user_pick.begin(), [](unsigned char convertToLowerForCheckWord){return std::tolower(convertToLowerForCheckWord);});
    for (int user_option1_iterator = 0; user_option1_iterator < user_option1.size(); user_option1_iterator++) {
        user_option1_portion = user_option1_portion + user_option1[user_option1_iterator];
        if (specific_user_pick == user_option1_portion) {
            return 1;
        }
    }
    for (int user_option2_iterator = 0; user_option2_iterator < user_option2.size(); user_option2_iterator++) {
        user_option2_portion = user_option2_portion + user_option2[user_option2_iterator];
        if (specific_user_pick == user_option2_portion) {
            return 2;
        }
    }
    if (!validity_of_pick) {
        std::cout << "That's not a valid input!" << std::endl;
        std::cout << "Please input a something valid (Valid inputs: " << user_option1 << " & " << user_option2 << "): " << std::endl;
        std::cin >> specific_user_pick_again;
        return check_word(specific_user_pick_again, user_option1, user_option2);
    }
    return 0;
}


int random_int_represents_bye() {
    int sum = 0;
    int temp_number = 0;
    for (int i = 0; i < 100; i++) { // 100 times
        temp_number = (rand() % 100) + 1; // can be 1 + 0-99
        sum += temp_number;
    }
    return sum; // big random number
}

void normal_round_robin_matchmaker(std::vector<std::string> player_list) {
// my idea 3: make a literal loop and just... start moving in a circle and that's the matchmaking. if odd, @ median # of the thing = bye
/*
Model With 8 Players (Clockwise):
0 7   0 6   0 5   0 4   0 3   0 2   0 1
6 1   5 7   4 6   3 5   2 4   1 3   7 2
5 2   4 1   3 7   2 6   1 5   7 4   6 3
4 3   3 2   2 1   1 7   7 6   6 5   5 4
*/  
    bool parity_odd = false; // frankly, I should probably change this variable's name
    std::string burner_var = ""; // just for the pause in between rounds
    int bye = random_int_represents_bye(); // big number
    int amount_of_rounds = (player_list.size());
    amount_of_rounds--; // for some reason it won't let me lower the player_list size in the same line
    std::string bye_string = std::to_string(bye); // stringify the big number
    std::cout << bye_string << std::endl;
    if (player_list.size() % 2 == 1) { // adds bye to make things even
        player_list.push_back(bye_string);
        amount_of_rounds--;
        parity_odd = true;
    }
    for (int i = 0; i < amount_of_rounds; i++) {
        std::cout << "Round " << i + 1 << ": " << std::endl;
        for (int j = 0; j < player_list.size(); j+=2) {
            // if else is for the bye statement, probably will tweak in the future to be more clean
            if (parity_odd == true && (player_list[j] == bye_string)) {
                std::cout << player_list[j + 1] << " is out for this round." << std::endl;
            } else if (parity_odd == true && (player_list[j + 1] == bye_string)) {
                std::cout << player_list[j] << " is out for this round." << std::endl;
            } else {
                std::cout << player_list[j] << " is playing against " << player_list[j + 1] << std::endl;
            }
        }
        nrr_player_list_shift(player_list, parity_odd);
        std::cout << "Type in anything and enter to continue" << std::endl;
        std::cin >> burner_var;
    }
}

void nrr_player_list_shift(std::vector<std::string>& player_list, bool parity_odd) {
    int amount_of_players_shifting = player_list.size() - 1; // cant include player[0]
    if (parity_odd == true) {
        amount_of_players_shifting--; // to remove the bye
    }
    std::string first_player = player_list[1]; // save what is the first player
    for (int i = 1; i < amount_of_players_shifting; i++) {
        player_list[i] = player_list[i + 1];
    }
    player_list[amount_of_players_shifting--] = first_player; // last player become first player (of the cycle)
}