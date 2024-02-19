#include <iostream>

#include "System.h"

// program description and first user's answer
void welcome();
// searching some file or exiting? it is decided here.
void control_center(const char answer);
// file searching, yaaay!
void file_searching_approved();

int main()
{
    // I'm polite and tired,
    // so greeting should be along with yawning.
    welcome();
    return 0;
}

void welcome()
{
    std::cout << "this is a program to search files on your computer. "
        "please take into consideration that if you have a couple of files "
        "with same names and extensions, the program will list them all, "
        "although it may take a while.\n"
        "ready to proceed? enter P. want to quit? enter Q.\n";

    // taking the answer
    char answer{};
    std::cin >> answer;

    // clearing the buffer
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    control_center(answer);
}

void control_center(const char answer)
{
    // user wants to find something!
    if (answer == 'p' || answer == 'P')
    {
        file_searching_approved();
    }
    // user wants to quit.
    if (answer == 'q' || answer == 'Q')
    {
        exit(0);
    }
    // user does not understand what a character is.
    // or user is testing the code. hi, if you are reading this.
    if (!(answer == 'p' || answer == 'P') &&
        !(answer == 'q' || answer == 'Q'))
    {
        std::cout << "wrong input. please enter P or Q.\n";
        welcome();
    }
}

void file_searching_approved()
{
    // getting file name
    std::string input_file{};
    std::cout << "enter file name here (with extension): ";
    std::cin >> input_file;

    // clearing the buffer
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    /*
     * search logic:
     *
     * thread_search creates threads for all subdirectories,
     * each of them is calling find_file_path.
     * when some thread(s) has found the file, it notifies others to stop.
     *  -- im using plural form here, because if there are
     * many identical files, the program is likely to found a couple of them --
     * then, all threads close.
     */
    System::thread_search(input_file);
    for (auto& thread : System::threads) {
        std::cout << "closing thread " << thread.get_id() << "...\n";
        thread.join();
    }
}