#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>


using namespace std;


/* global constants */
const int CHANGE_PARAM[8] = { 5, 10, 15, 20, 25, 30, 40, 50 };
const int PROBABILITY_OF_A_RANDOM_EVENT[4] = { 5, 12, 30, 101 };
const int MAX_DAY = 45;
const int NUMBER_OF_EXAM = 5;
const int EXAM_NUMBER[5] = { 1, 2, 3, 4, 5 };
const int EXAM_PASSED_MIN = 0;
const int EXAM_PASSED_MAX = 5;
const int PARAM_MIN_VALUE = 0;
const int PARAM_MAX_VALUE = 100;
const int ENERGY_VALUE[2] = { 80, 40 };
const int CHANCE_RANDOM_EVENT[5] = { 0, 1, 2, 3, 4 };
const int DIFFICULTY[3] = { 1, 2, 3 };
const int SUCCESS_FOR_COMPARATION = 75;
const int LEVELS = 3;
const int MONAY_ENERGY_PER_LEVELS[LEVELS] = { 100, 80, 60 };
const int PSYCHE_PER_LEVELS[LEVELS] = { 100, 80, 40 };
const int KNOWLEDGE_PER_LEVELS[LEVELS] = { 80, 50, 35 };
const int DIFICULT[3] = { 1, 2, 3 };
const int THIRD_EXAM_DAY = 26;
const int FIFTH_EXAM_DAY = 45;
const int GAME_RESULT[3] = { 1, 2, 3 };
const double PROBABILITY_OF_SUCCESS[3] = { 1.0, 0.75, 0.5 };
const double SUCCESS_RATES[4] = { 0.75, 0.2, 0.1, 0.6 };



/* limits values from 0 to 100 */
int clamp(int x, int minValue = PARAM_MIN_VALUE, int maxValue = PARAM_MAX_VALUE) {
    if (x < minValue) return minValue;
    if (x > maxValue) return maxValue;
    return x;
}

/* determines the effectiveness of actions depending on energy */
double efficiency(int energy) {
    if (energy >= ENERGY_VALUE[0]) return PROBABILITY_OF_SUCCESS[0];
    if (energy >= ENERGY_VALUE[1]) return PROBABILITY_OF_SUCCESS[1];
    return PROBABILITY_OF_SUCCESS[2];
}

/* determines the day of the exam */
int examNumberToday(int day, int exam4Day) {
    if (day == 8)  return EXAM_NUMBER[0];
    if (day == 17) return EXAM_NUMBER[1];
    if (day == 26) return EXAM_NUMBER[2];
    if (day == exam4Day) return EXAM_NUMBER[3];
    if (day == 45) return EXAM_NUMBER[4];
    return 0;
}

/* ASCII interface */
void printStatus(int money, int energy, int psyche, int knowledge,
    int examsPassed, int day) {
    cout << " ....................." << endl;
    cout << " : Day      " << setw(2) << day << " / 45  :" << endl;
    cout << " : Money:    " << setw(3) << money << " lv  :" << endl;
    cout << " : Energy:   " << setw(3) << energy << "     :" << endl;
    cout << " : Psyche:   " << setw(3) << psyche << "     :" << endl;
    cout << " : Knowledge:" << setw(3) << knowledge << "     :" << endl;
    cout << " : Exam:     " << examsPassed << "/5     :" << endl;
    cout << " ....................." << endl;
}

/* random event */
void randomDailyEvent(int& money, int& energy, int& psyche, int difficulty) {
    int chance = rand() % PROBABILITY_OF_A_RANDOM_EVENT[2];
    if (chance == CHANCE_RANDOM_EVENT[0]) {
        int random_event = rand() % PROBABILITY_OF_A_RANDOM_EVENT[0];
        cout << "Random events: ";
        if (random_event == CHANCE_RANDOM_EVENT[0]) {
            cout << "Mother and father send you money!" << endl;
            money += CHANGE_PARAM[1];
        }
        else if (random_event == CHANCE_RANDOM_EVENT[1]) {
            cout << "A friend buys you a coffee." << endl;
            psyche += CHANGE_PARAM[1];
        }
        else if (random_event == CHANCE_RANDOM_EVENT[2]) {
            cout << "You got a sick." << endl;
            energy -= CHANGE_PARAM[3];
        }
        else if (random_event == CHANCE_RANDOM_EVENT[3]) {
            cout << "The power went out." << endl;
            psyche -= CHANGE_PARAM[1];
        }
        else if (random_event == CHANCE_RANDOM_EVENT[4]) {
            cout << "Celebrate birthday!" << endl;
            if (difficulty == DIFFICULTY[0]) {
                energy -= CHANGE_PARAM[1];
                psyche += CHANGE_PARAM[1];
                money -= CHANGE_PARAM[1];
            }
            else if (difficulty == DIFFICULTY[1]) {
                energy -= CHANGE_PARAM[2];
                psyche += CHANGE_PARAM[2];
                money -= CHANGE_PARAM[2];
            }
            else {
                energy -= CHANGE_PARAM[3];
                psyche += CHANGE_PARAM[3];
                money -= CHANGE_PARAM[3];
            }
        }
    }
}

/* take exam */
void takeExam(int& energy, int& psyche, int& knowledge, int& examsPassed, int examNum) {
    cout << endl << "You are taking " << examNum << " exam." << endl;

    int K = rand() % PROBABILITY_OF_A_RANDOM_EVENT[3];
    int penalty = examsPassed * EXAM_PASSED_MAX;

    double success =
        (knowledge * SUCCESS_RATES[0]) + (psyche * SUCCESS_RATES[2]) +
        (energy * SUCCESS_RATES[2]) + (K * SUCCESS_RATES[1]) - penalty;

    cout << "Results: " << success << endl;
    energy -= CHANGE_PARAM[2];

    if (success >= SUCCESS_FOR_COMPARATION) {
        cout << "Took the exam!" << endl;
        examsPassed++;
        psyche += CHANGE_PARAM[3];
        knowledge = int(knowledge * SUCCESS_RATES[3]);
    }
    else {
        cout << "Failed the exam!" << endl;
        psyche -= CHANGE_PARAM[3];
    }
}

/* SAVE GAME */
void saveGame(int money, int energy, int psyche, int knowledge,
    int examsPassed, int day, int exam4Day, int difficulty) {
    ofstream f("save.txt");
    f << money << " " << energy << " " << psyche << " "
        << knowledge << " " << examsPassed << " "
        << day << " " << exam4Day << " " << difficulty;
    f.close();
}

/* LOAD GAME */
bool loadGame(int& money, int& energy, int& psyche, int& knowledge,
    int& examsPassed, int& day, int& exam4Day, int& difficulty) {
    ifstream f("save.txt");
    if (!f.is_open()) return false;
    f >> money >> energy >> psyche
        >> knowledge >> examsPassed
        >> day >> exam4Day >> difficulty;
    f.close();
    return true;
}



int main() {
    srand(time(0));

    int money, energy, psyche, knowledge, examsPassed;
    int day = 1;
    int exam4Day = rand() % (FIFTH_EXAM_DAY - THIRD_EXAM_DAY - 1) + (THIRD_EXAM_DAY + 1);
    int gameResult = 0;
    int difficulty = 2;

    int choice;

    /* start of game */
    cout << ".................." << endl;
    cout << ": SURVIVAL       :" << endl;
    cout << ": [1] New game   :" << endl;
    cout << ": [2] Load game  :" << endl;
    cout << ".................." << endl;

    /* input and verification data */
    while (true) {
        cout << "Choose 1 or 2: ";
        cin >> choice;
        if (choice == 1 || choice == 2) break;
        cout << "Please, input 1 or 2: ";
    }

    /* load game and choose difficulty */
    if (choice == 2 && loadGame(money, energy, psyche, knowledge,
        examsPassed, day, exam4Day, difficulty)) {
        cout << "The game is loaded." << endl;
    }
    else {
        cout << "Choose level (1 - easy, 2 - normal, 3 - difficult): ";

        /* verification data */
        while (true) {
            cin >> difficulty;
            if (difficulty >= DIFICULT[0] && difficulty <= DIFICULT[2]) break;
            cout << "Please input 1, 2 or 3: ";
        }

        /* difficulty 1 */
        if (difficulty == DIFICULT[0]) {
            money = energy = MONAY_ENERGY_PER_LEVELS[LEVELS - 3];
            psyche = PSYCHE_PER_LEVELS[LEVELS - 3];
            knowledge = KNOWLEDGE_PER_LEVELS[LEVELS - 3];
        }
        /* difficulty 2 */
        else if (difficulty == DIFICULT[1]) {
            money = energy = MONAY_ENERGY_PER_LEVELS[LEVELS - 2];
            psyche = PSYCHE_PER_LEVELS[LEVELS - 2];
            knowledge = KNOWLEDGE_PER_LEVELS[LEVELS - 2];
        }
        /* difficulty 3 */
        else {
            money = energy = MONAY_ENERGY_PER_LEVELS[LEVELS - 1];
            psyche = PSYCHE_PER_LEVELS[LEVELS - 1];
            knowledge = KNOWLEDGE_PER_LEVELS[LEVELS - 1];
        }

        examsPassed = EXAM_PASSED_MIN;
        day = 1;
    }

    /* a loop that rotates for each day */
    while (day <= MAX_DAY) {

        randomDailyEvent(money, energy, psyche, difficulty);
        printStatus(money, energy, psyche, knowledge, examsPassed, day);

        if (money <= PARAM_MIN_VALUE) { gameResult = GAME_RESULT[1]; break; }
        if (psyche <= PARAM_MIN_VALUE) { gameResult = GAME_RESULT[2]; break; }

        int examToday = examNumberToday(day, exam4Day);
        bool canTakeExam = (examToday > examsPassed);

        if (canTakeExam)
            cout << endl << "You have an " << examToday << " exam today!" << endl;

        /* choose activity */
        cout << endl << "[1] Study" << endl;
        cout << "[2] Eat" << endl;
        cout << "[3] Go out" << endl;
        cout << "[4] Rest" << endl;
        cout << "[5] Work" << endl;
        if (canTakeExam) {
            cout << "[6] You are taking an exam" << endl;
            cout << "[7] You aren't taking an exam" << endl;
        }
        cout << "[9] Save and exit" << endl;

        /*input data verification*/
        bool validChoice = false;
        while (!validChoice) {
            cin >> choice;
            if ((choice >= 1 && choice <= 5) ||
                (choice == 6 && canTakeExam) ||
                (choice == 7 && canTakeExam) ||
                choice == 9) {
                validChoice = true;
            }
            else {
                cout << "Invalid input! Try again: " << endl;
            }
        }

        double eff = efficiency(energy);

        /* chooses the type of learning */
        if (choice == 1) {
            int studyChoice;

            cout << endl << "Choose a learning method: " << endl;
            cout << "[1] You go to lectures" << endl;
            cout << "[2] Study at home" << endl;
            cout << "[3] Study with friend" << endl;

            while (true) {
                cin >> studyChoice;
                if (studyChoice >= 1 && studyChoice <= 3) break;
                cout << "Input 1, 2 or 3: ";
            }

            if (studyChoice == 1) {
                knowledge += CHANGE_PARAM[3];
                energy -= CHANGE_PARAM[3];
                psyche -= CHANGE_PARAM[1];
            }
            else if (studyChoice == 2) {
                knowledge += CHANGE_PARAM[2];
                energy -= CHANGE_PARAM[2];
                psyche -= CHANGE_PARAM[3];
            }
            else {
                knowledge += CHANGE_PARAM[0];
                energy -= CHANGE_PARAM[1];
                psyche += CHANGE_PARAM[1];
            }
        }

        /* random event: spoiled food */
        else if (choice == 2) {
            bool spoiled = (rand() % PROBABILITY_OF_A_RANDOM_EVENT[1] == 0);

            if (spoiled) {
                cout << endl << "Random event: The food is spoiled" << endl;
                energy -= CHANGE_PARAM[3];
            }
            else {
                energy += int(20 * eff);
                money -= CHANGE_PARAM[1];
                psyche += int(5 * eff);
            }
        }

        else if (choice == 3) {
            psyche += CHANGE_PARAM[6];
            money -= CHANGE_PARAM[4];
            energy -= CHANGE_PARAM[2];
        }
        else if (choice == 4) {
            energy += CHANGE_PARAM[7];
            psyche += CHANGE_PARAM[1];
        }
        else if (choice == 5) {
            money += CHANGE_PARAM[6];
            energy -= CHANGE_PARAM[3];
            psyche -= CHANGE_PARAM[1];
        }
        else if (choice == 6 && canTakeExam) {
            takeExam(energy, psyche, knowledge, examsPassed, examToday);
        }
        else if (choice == 7 && canTakeExam) {
            cout << endl << "Decide not to take the exam." << endl;

            if (difficulty == 1) psyche -= CHANGE_PARAM[1];
            else if (difficulty == 2) psyche -= CHANGE_PARAM[2];
            else psyche -= CHANGE_PARAM[3];
        }
        else if (choice == 9) {
            saveGame(money, energy, psyche, knowledge, examsPassed, day, exam4Day, difficulty);
            return 0;
        }

        energy = clamp(energy);
        psyche = clamp(psyche);
        knowledge = clamp(knowledge);

        if (energy <= PARAM_MIN_VALUE) {
            cout << "Fainted. You miss a day." << endl;
            energy = CHANGE_PARAM[6];
            psyche -= CHANGE_PARAM[1];
            day++;
        }

        if (examsPassed == EXAM_PASSED_MAX) {
            gameResult = GAME_RESULT[0];
            break;
        }

        day++;
    }

    /* final results */
    if (gameResult == GAME_RESULT[0]) {
        cout << "........................" << endl;
        cout << ": Victory!             :" << endl;
        cout << ": Passed all the exam. :" << endl;
        cout << "........................" << endl;
    }
    else if (gameResult == GAME_RESULT[1]) {
        cout << "..................................." << endl;
        cout << ": Fail!                           :" << endl;
        cout << ": Went bankrut, ran out of money. :" << endl;
        cout << "..................................." << endl;

    }
    else if (gameResult == GAME_RESULT[2]) {
        cout << "................................." << endl;
        cout << ": Fail!                         :" << endl;
        cout << ": You psyche couldn't handle it :" << endl;
        cout << ": and you left the university.  :" << endl;
        cout << "................................." << endl;
    }
    else if (day > MAX_DAY && examsPassed < NUMBER_OF_EXAM) {
        cout << "................................." << endl;
        cout << ": Fail!                         :" << endl;
        cout << ": The session ended             :" << endl;
        cout << ": without all exam taken.       :" << endl;
        cout << "................................." << endl;
    }

    return 0;
}