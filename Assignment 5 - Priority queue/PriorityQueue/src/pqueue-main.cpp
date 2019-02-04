/*
 * CS 106B Priority Queue
 * This file contains the main program and user interface for testing your
 * priority queues.
 *
 * You can modify this file if you like, to add other tests and so on.
 * But your turned-in files should work properly with an unmodified version of
 * all provided code files.
 *
 * Author : Marty Stepp
 * Version: Mon 2015/05/04
 * - modified to use ArrayPQ for 15sp version
 */

#include <algorithm>  // For sort, reverse
#include <iostream>
#include <sstream>
#include <string>

#include "console.h"
#include "random.h"
#include "simpio.h"
#include "vector.h"

#include "ArrayPriorityQueue.h"
#include "LinkedPriorityQueue.h"
#include "HeapPriorityQueue.h"
#include "ExtraPriorityQueue.h"

using namespace std;

static const int RANDOM_STRING_LENGTH = 6;    // max length of random strings
static const bool RIG_RANDOM_NUMBERS = true;  // true to use same random sequence every time

// function prototype declarations
static string randomString(int maxLength);
static void easterEgg();
template <typename T>
void test(T& queue);

int main() {
    cout << "CS 106B PriorityQueue test program" << endl;
    cout << "==================================" << endl;

    if (RIG_RANDOM_NUMBERS) {
        setRandomSeed(42);   // life, the universe, and everything
    }

    while (true) {
        cout << endl;
        cout << "A) Array, L) Linked, H) Heap, E)xtra, Q)uit" << endl;
        string choice = toUpperCase(trim(getLine(
                "Choose a queue: ")));
        if (choice.empty() || choice == "Q") {
            break;
        } else if (choice == "A") {
            ArrayPriorityQueue pq;
            test(pq);
        } else if (choice == "L") {
            LinkedPriorityQueue pq;
            test(pq);
        } else if (choice == "H") {
            HeapPriorityQueue pq;
            test(pq);
        } else if (choice == "E") {
            ExtraPriorityQueue pq;
            test(pq);
        } else if (choice == "TNG") {
            easterEgg();
        }
    }

    cout << endl;
    cout << "Exiting." << endl;
    return 0;
}

/*
 * Returns a randomly generated string of letters up to the given length.
 */
static string randomString(int maxLength) {
    int length = randomInteger(1, maxLength);
    ostringstream out;
    for (int i = 0; i < length; i++) {
        char ch = (char) ('a' + randomInteger(0, 25));
        out << ch;
    }
    return out.str();
}

/*
 * Prompts the user to perform tests on any type of queue.
 * Each method of the queue has a corresponding letter or symbol to type that
 * will call that method on the queue and display the results as appropriate.
 */
template <typename T>
void test(T& queue) {
    cout << endl;
    cout << "E) enqueue    I) isEmpty    P) peek             BE) bulk enQ" << endl;
    cout << "D) dequeue    S) size       R) peekPriority     BD) bulk deQ" << endl;
    cout << "<) print      L) clear      C) changePriority    Q) quit" << endl;
    while (true) {
        cout << endl;
        string choice = toUpperCase(trim(getLine("Choose an operation: ")));
        if (choice.empty() || choice == "Q") {
            break;
        } else if (choice == "C") {
            string value = getLine("Value? ");
            int newPriority = getInteger("New priority? ");
            queue.changePriority(value, newPriority);
        } else if (choice == "L") {
            queue.clear();
        } else if (choice == "D") {
            string value = queue.dequeue();
            cout << "Value returned: \"" << value << "\"" << endl;
        } else if (choice == "E") {
            string value = getLine("Value? ");
            int priority = getInteger("Priority? ");
            queue.enqueue(value, priority);
            cout << "Enqueued \"" << value << "\" with priority " << priority << endl;
        } else if (choice == "I") {
            bool empty = queue.isEmpty();
            cout << "Value returned: " << boolalpha << empty << endl;
        } else if (choice == "P") {
            string value = queue.peek();
            cout << "Value returned: \"" << value << "\"" << endl;
        } else if (choice == "R") {
            int priority = queue.peekPriority();
            cout << "Value returned: " << priority << endl;
        } else if (choice == "S") {
            int size = queue.size();
            cout << "Value returned: " << size << endl;
        } else if (choice == "<") {
            cout << "Console output from << operator:" << endl;
            cout << queue << endl;
        } else if (choice == "BE") {
            bulkEnqueue(queue);
        } else if (choice == "BD") {
            int count = getInteger("How many elements? ");
            for (int i = 1; i <= count; i++) {
                string value = queue.dequeue();
                cout << "Dequeue #" << i << ", value returned: \"" << value << "\"" << endl;
            }
        }
    }
}

/*
 * Bulk enqueue is hairy enough that we pull it out as its own function.
 * Prompts the user for a number and order of elements and enqueues them.
 */
template <typename T>
void bulkEnqueue(T& queue) {
    int count = getInteger("How many elements? ");
    string choice2 = trim(toUpperCase(getLine("R)andom, A)scending, D)escending? ")));
    if (choice2 == "R") {
        for (int i = 0; i < count; i++) {
            string value = randomString(5);
            int priority = randomInteger(1, count);
            queue.enqueue(value, priority);
            cout << "Enqueued \"" << value << "\" with priority " << priority << endl;
        }
    } else if (choice2 == "A" || choice2 == "D") {
        Vector<string> toAdd;
        for (int i = 0; i < count; i++) {
            toAdd.add(randomString(RANDOM_STRING_LENGTH));
        }
        if (choice2 == "A") {
            for (int i = 0; i < toAdd.size(); i++) {
                string value = toAdd[i];
                int priority = i + 1;
                queue.enqueue(value, priority);
                cout << "Enqueued \"" << value << "\" with priority " << priority << endl;
            }
        } else {
            for (int i = toAdd.size() - 1; i >= 0; i--) {
                string value = toAdd.get(i);
                int priority = i + 1;
                queue.enqueue(value, priority);
                cout << "Enqueued \"" << value << "\" with priority " << priority << endl;
            }
        }
    }
}

/*
 * A silly hidden function that prints some things about the character Q
 * from the TV show, "Star Trek: The Next Generation".
 * Text courtesy of: http://en.memory-alpha.org/wiki/Q
 * ASCII art courtesy of: http://www.chris.com/ascii/index.php?art=television/star%20trek
 */
static void easterEgg() {
    cout << "                                _____..---========+*+==========---.._____" << endl;
    cout << "   ______________________ __,-='=====____  =================== _____=====`=" << endl;
    cout << "  (._____________________I__) - _-=_/    `---------=+=--------'" << endl;
    cout << "      /      /__...---===='---+---_'" << endl;
    cout << "     '------'---.___ -  _ =   _.-'    *    *    *   *" << endl;
    cout << "                    `--------'" << endl;
    cout << endl;
    cout << "                 _____.-----._____" << endl;
    cout << "    ___----~~~~~~. ... ..... ... .~~~~~~----___" << endl;
    cout << " =================================================" << endl;
    cout << "    ~~~-----......._____________.......-----~~~" << endl;
    cout << "     (____)          \\   |   /          (____)" << endl;
    cout << "       ||           _/   |   \\_           ||" << endl;
    cout << "        \\\\_______--~  //~~~\\\\  ~--_______// " << endl;
    cout << "         `~~~~---__   \\\\___//   __---~~~~'     " << endl;
    cout << "                   ~~-_______-~~" << endl;
    cout << endl;
    cout << "                  xxxXRRRMMMMMMMMMMMMMMMxxx,." << endl;
    cout << "              xXXRRRRRXXXVVXVVXXXXXXXRRRRRMMMRx," << endl;
    cout << "            xXRRXRVVVVVVVVVVVVVVVXXXXXRXXRRRMMMMMRx." << endl;
    cout << "          xXRXXXVVVVVVVVVVVVVVVVXXXXVXXXXXXRRRRRMMMMMxx." << endl;
    cout << "        xXRRXXVVVVVttVtVVVVVVVVVtVXVVVVXXXXXRRRRRRRMMMMMXx" << endl;
    cout << "      xXXRXXVVVVVtVttttttVtttttttttVXXXVXXXRXXRRRRRRRMMMMMMXx" << endl;
    cout << "     XRXRXVXXVVVVttVtttVttVttttttVVVVXXXXXXXXXRRRRRRRMMMMMMMMVx" << endl;
    cout << "    XRXXRXVXXVVVVtVtttttVtttttittVVVXXVXVXXXRXRRRRRMRRMMMMMMMMMX," << endl;
    cout << "   XRRRMRXRXXXVVVXVVtttittttttttttVVVVXXVXXXXXXRRRRRMRMMMMMMMMMMM," << endl;
    cout << "   XXXRRRRRXXXXXXVVtttttttttttttttttVtVXVXXXXXXXRRRRRMMMMMMMMMMMMM," << endl;
    cout << "   XXXXRXRXRXXVXXVtVtVVttttttttttttVtttVXXXXXXXRRRRRMMMMMMMMMMMMMMMR" << endl;
    cout << "   VVXXXVRVVXVVXVVVtttititiitttttttttttVVXXXXXXRRRRRMRMMMMMMMMMMMMMMV" << endl;
    cout << "   VttVVVXRXVVXtVVVtttii|iiiiiiittttttttitXXXRRRRRRRRRRMMMMMMMMMMMMMM" << endl;
    cout << "   tiRVVXRVXVVVVVit|ii||iii|||||iiiiiitiitXXXXXXXXRRRRRRMMMMMMMMMMMMM" << endl;
    cout << "    +iVtXVttiiii|ii|+i+|||||i||||||||itiiitVXXVXXXRRRRRRRRMMMMMMRMMMX" << endl;
    cout << "    `+itV|++|tttt|i|+||=+i|i|iiii|iiiiiiiitiVtti+++++|itttRRRRRMVXVit" << endl;
    cout << "     +iXV+iVt+,tVit|+=i|||||iiiiitiiiiiiii|+||itttti+=++|+iVXVRV:,|t" << endl;
    cout << "     +iXtiXRXXi+Vt|i||+|++itititttttttti|iiiiitVt:.:+++|+++iXRMMXXMR" << endl;
    cout << "     :iRtiXtiV||iVVt||||++ttittttttttttttttXXVXXRXRXXXtittt|iXRMMXRM" << endl;
    cout << "      :|t|iVtXV+=+Xtti+|++itiiititittttVttXXXXXXXRRRXVtVVtttttRRMMMM|" << endl;
    cout << "        +iiiitttt||i+++||+++|iiiiiiiiitVVVXXRXXXRRRRMXVVVVttVVVXRMMMV" << endl;
    cout << "         :itti|iVttt|+|++|++|||iiiiiiiittVVXRRRMMMMMMRVtitittiVXRRMMMV" << endl;
    cout << "           `i|iitVtXt+=||++++|++++|||+++iiiVVXVRXRRRV+=|tttttttiRRRMMM|" << endl;
    cout << "             i+++|+==++++++++++++++|||||||||itVVVViitt|+,,+,,=,+|itVX'" << endl;
    cout << "              |+++++.,||+|++++=+++++++|+|||||iitt||i||ii||||||itXt|" << endl;
    cout << "              t||+++,.=i+|+||+++++++++++++|i|ittiiii|iiitttttXVXRX|" << endl;
    cout << "              :||+++++.+++++++++|++|++++++|||iii||+:,:.-+:+|iViVXV" << endl;
    cout << "              iii||+++=.,+=,=,==++++++++++|||itttt|itiittXRXXXitV'" << endl;
    cout << "             ;tttii||++,.,,,.,,,,,=++++++++++|iittti|iiiiVXXXXXXV" << endl;
    cout << "            tVtttiii||++++=,,.  . ,,,=+++++++|itiiiiiii||||itttVt" << endl;
    cout << "           tVVttiiiii||||++++==,. ..,.,+++=++iiiiiitttttVVXXRRXXV" << endl;
    cout << "        ..ttVVttitttii||i|||||+|+=,.    .,,,,==+iittVVVXRRMXRRRV" << endl;
    cout << "...'''ittitttttitVttttiiiiii|ii|++++=+=..... ,.,,||+itiVVXXVXV" << endl;
    cout << "      ,|iitiiitttttttiiiii||ii||||||||+++++,.i|itVt+,,=,==........." << endl;
    cout << "        ,|itiiiVtVtiii||iiiiii|||||||++||||tt|VXXRX|  ....  ..     ' ' '." << endl;
    cout << "          ,,i|ii||i||+|i|i|iiiiiiii||||ittRVVXRXRMX+, .  ...   .         ," << endl;
    cout << "    .       .,+|++|||||ii|i|iiiitttVVttXVVXVXRRRRXt+. .....  . .       ,. ." << endl;
    cout << "  . .          ,,++|||||||i|iiitVVVXXXXVXXVXXRRRV+=,.....  ....  ..       .." << endl;
    cout << "                  .,,++|||i|iittXXXXRMViRXXXXRVt+=, ..    ...... .        .." << endl;
    cout << "                   ,XX+.=+++iitVVXXXRXVtXXVRRV++=,..... .,, .              ." << endl;
    cout << "            ....       +XX+|i,,||tXRRRXVXti|+++,,. .,,. . . .. .      . ...." << endl;
    cout << "  . .          .      ..  ..........++,,..,...,.... ..             .. ..." << endl;
    cout << endl;
    cout << "Q was a highly powerful entity from a race of omnipotent, godlike "
         << "beings also known as the Q. Q appeared to the crews of several "
         << "Starfleet vessels and outposts during the 2360s and 2370s. "
         << "All command level officers in Starfleet are briefed on his "
         << "existence. One such briefing was attended by Benjamin Sisko "
         << "in 2367. He typically appears as a Humanoid male (though "
         << "he can take on other forms if he wishes), almost "
         << "always dressed in the uniform of a Starfleet captain." << endl;
    cout << "In every appearance, he demonstrated superior capabilities, but "
         << "also a mindset that seemed quite unlike what Federation scientists "
         << "expected for such a powerful being. He has been described, in "
         << "turn, as 'obnoxious,' 'interfering,' and a 'pest'. However, "
         << "underneath his acerbic attitude, there seemed to be a hidden "
         << "agenda to Q's visits that often had the best interests of "
         << "Humanity at their core." << endl << endl;
    cout << "\"Worf... Eat any good books lately?\" -- Q" << endl;
}
