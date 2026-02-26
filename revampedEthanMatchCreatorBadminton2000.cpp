#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <cctype>
#include <sstream>

/*
DEV LOG:
Day 0 (2/14/2026): Made ideas of normal round-robin matchmaking system and saving data. Learned about lambda functions to use
the algorithm library for std::tolower. Made the 'info' option.
Day 1  (2/15/2026): Made a continuation helper function, throughly checks input compared to other functions. Started commenting.
Started working on normal round-robin matchmaking by setting the area for it, making it so we get the list of player names & declaration of round #
(Note to self: maybe add a "quit" option to the check_continuation method)
Day 2 (2/16/2026): Made a check word function so you can check A or B. Changed some stuff to implement it.
Day 3 (2/26/2026): Setted up Github on computer for laptop to be able to access.
*/

std::ifstream fin("badmintonRecords.txt");
std::ofstream fout("badmintonRecords.txt", std::ios::app); // std::ios::app means for input out put string, making it do append instead of truncating

// big functions
void normal_round_robin_matchmaker(std::vector<std::string> player_list);

// helper functions
void fetch_data(std::string information); // going to be used for the save function and records
void check_continuation(bool& still_using, std::string continuation, bool& repeat_continuation); // used to see if they wish to continue or not
int check_word(std::string specific_user_pick, std::string user_option1, std::string user_option2); // variation of check_continuation but for A or B

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
        // funky lambda function to lowercaseify, frequent in this program
        std::transform(user_pick.begin(), user_pick.end(), user_pick.begin(), [](unsigned char hopefullyThisWork){return std::tolower(hopefullyThisWork);}); // hopefullyThisWork = variable for lower
        if (user_pick == "info" || user_pick == "i") { // lists  options available to use
            std::cout << "\nOptions (P.S. The first letter of each option also is valid): " << std::endl;
            std::cout << "'info' - Gives available options" << std::endl << "'records' - Allows you to view previous matches" << std::endl;
            std::cout << "'save' - Saves match information and sends it to records" << std::endl << "'matchmaking' - Creates a type of matchmaking format" << std::endl;
            std::cout << "\nDo you wish to continue or end?: " << std::endl;
            std::cin >> continuation;
            check_continuation(still_using, continuation, repeat_continuation);
        } 

        else if (user_pick == "matchmaking" || user_pick == "m") { // does matchmaking
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
}

void normal_round_robin_matchmaker(std::vector<std::string> player_list) {
    std::vector<std::string> shifted_player_list;
    if (player_list.size() % 2 == 1) {
        player_list.push_back("B");
    }
    for (int all_but_one_player = (player_list.size() / 2) - 1; all_but_one_player > 0; all_but_one_player--) {
        shifted_player_list.push_back(player_list[all_but_one_player]);
    }
    for (int all_but_one_player_jump = player_list.size() - 1; all_but_one_player_jump >= player_list.size() / 2; all_but_one_player_jump--) {
        shifted_player_list.push_back(player_list[all_but_one_player_jump]);
    }
    // above is me crafting a list thing that should work
    /*
    How it functions:
    So I will just give an example, with 8 players, I will basically have this list do in this order: player 4, 3, 2, 8, 7, 6, 5
    I'm gonna eventually use index thingies and hopefully it'll work out
    */
    for (int rounds = 1; rounds < player_list.size(); rounds++) { // although I couldve just done rounds = 1 & rounds < player_list.size(), I don't feel like it (Changed it)
        std::cout << "\nRound " << rounds <<  ": " << std::endl;
        std::cout << player_list[0] << " is playing against " << player_list[player_list.size() - rounds]; // first match-up
        for (int other_games = 1; other_games < (player_list.size() / 2); other_games++) { // other match-ups
            
        }
    }
}