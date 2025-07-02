#include <iostream>
#include <string>
#include <chrono>
#include <ctime>


using namespace std;

class Question {
protected:
    string question;
    string correctAnswer;

public:
    Question(const string& question, const string& correctAnswer)
        : question(question), correctAnswer(correctAnswer) {}

    virtual ~Question() = default;

    virtual bool askQuestion() = 0;

    string getQuestion() const {
        return question;
    }

    string getCorrectAnswer() const {
        return correctAnswer;
    }
};

class MultipleChoiceQuestion : public Question {
private:
    string options[4];

public:
    MultipleChoiceQuestion(const string& question, const string& correctAnswer,
        const string& opt1, const string& opt2,
        const string& opt3, const string& opt4)
        : Question(question, correctAnswer) {
        options[0] = opt1;
        options[1] = opt2;
        options[2] = opt3;
        options[3] = opt4;
    }

    bool askQuestion() override {
        cout << question << "\n";
        for (int i = 0; i < 4; ++i) {
            cout << i + 1 << ". " << options[i] << "\n";
        }

        auto start = chrono::steady_clock::now();
        int userAnswer;
        cout << "Enter your answer (1-4): ";
        cin >> userAnswer;

        auto end = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::seconds>(end - start);

        if (userAnswer > 0 && userAnswer <= 4) {
            if (options[userAnswer - 1] == correctAnswer) {
                cout << "Correct!\n";
            }
            else {
                cout << "Incorrect. The correct answer is: " << correctAnswer << "\n";
            }
        }
        else {
            cout << "Invalid input. Please enter a number between 1 and 4.\n";
            return false;
        }

        cout << "Time taken: " << elapsed.count() << " seconds\n";
        return (options[userAnswer - 1] == correctAnswer);
    }
};

class Quiz {
private:
    Question* mathQuestions[4];
    Question* scienceQuestions[4];

public:
    Quiz() {
        // Math Questions
        mathQuestions[0] = new MultipleChoiceQuestion("What is 2 + 2?", "4", "3", "4", "5", "6");
        mathQuestions[1] = new MultipleChoiceQuestion("What is 5 * 6?", "30", "25", "30", "35", "40");
        mathQuestions[2] = new MultipleChoiceQuestion("What is the square root of 64?", "8", "6", "7", "8", "9");
        mathQuestions[3] = new MultipleChoiceQuestion("What is the value of pi (π) to two decimal places?", "3.14", "3.12", "3.14", "3.16", "3.18");

        // Science Questions
        scienceQuestions[0] = new MultipleChoiceQuestion("What is the powerhouse of the cell?", "Mitochondria", "Nucleus", "Mitochondria", "Ribosome", "Golgi apparatus");
        scienceQuestions[1] = new MultipleChoiceQuestion("What is the largest planet in our solar system?", "Jupiter", "Mars", "Jupiter", "Saturn", "Neptune");
        scienceQuestions[2] = new MultipleChoiceQuestion("Which gas do plants primarily absorb during photosynthesis?", "Carbon dioxide", "Oxygen", "Carbon monoxide", "Carbon dioxide", "Nitrogen");
        scienceQuestions[3] = new MultipleChoiceQuestion("What is the chemical symbol for water?", "H2O", "H2O", "CO2", "O2", "NaCl");
    }

    ~Quiz() {
        for (int i = 0; i < 4; ++i) {
            delete mathQuestions[i];
            delete scienceQuestions[i];
        }
    }

    void startQuiz() {
        char start;
        cout << "Welcome to the Quiz Generator!\n";
        cout << "Would you like to start the quiz? (y/n): ";
        cin >> start;

        if (!(start == 'y' || start == 'Y')) {
            cout << "Maybe next time. Goodbye!\n";
            return;
        }

        string subject;
        cout << "Choose a subject (Math/Science): ";
        cin >> subject;

        Question** selectedQuestions = nullptr; //pointer declaration and initialization
        if (subject == "Math") {
            selectedQuestions = mathQuestions;
        }
        else if (subject == "Science") {
            selectedQuestions = scienceQuestions;
        }
        else {
            cout << "Invalid subject!\n";
            return;
        }

        if (selectedQuestions == nullptr) {
            cout << "Invalid subject!\n";
            return;
        }

        int correctAnswers = 0;
        int incorrectAnswers = 0;
        auto totalTime = chrono::seconds(0);

        for (int i = 0; i < 4; ++i) {
            if (selectedQuestions[i] == nullptr) break;

            cout << "\nQuestion " << i + 1 << ":\n";
            auto start = chrono::steady_clock::now();
            if (selectedQuestions[i]->askQuestion()) {
                ++correctAnswers;
            }
            else {
                ++incorrectAnswers;
                //--i; // Ask the same question again if the answer is invalid
            }
            auto end = chrono::steady_clock::now();
            totalTime += chrono::duration_cast<chrono::seconds>(end - start);
        }

        cout << "\nQuiz Complete!\n";
        cout << "Total correct answers: " << correctAnswers << "\n";
        cout << "Total incorrect answers: " << incorrectAnswers << "\n";
        cout << "Total time taken: " << totalTime.count() << " seconds\n";
        char retry;
        cout << "Do you want to reattempt the quiz? (y/n): ";
        cin >> retry;

        if (retry == 'y' || retry == 'Y') {
            startQuiz();
        }
    }
};

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    Quiz quiz;
    quiz.startQuiz();
    return 0;
}
