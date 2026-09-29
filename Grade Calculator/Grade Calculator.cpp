#include <ctime>
#include <iostream>
#include <string>

using namespace std;

/// Time based greeting
void time_based_greeting() {
  time_t now = time(0);
  struct tm *current_time = localtime(&now);
  int hour = current_time->tm_hour;
  if (hour >= 6 && hour < 11) {
    cout << "Good Morning! Early Bird!" << endl;
  } else if (hour >= 12 && hour < 18) {
    cout << "Good Afternoon!" << endl;
  } else {
    cout << "Good Evening!" << endl;
  }
}

/// Calculate the percentage of points earned
double calculate_percentage(double points_earned, double total_points) {
  return (points_earned / total_points) * 100.0;
}

/// Function to determine letter grade
char get_letter_grade(double percentage) {
  if (percentage >= 90)
    return 'A';
  else if (percentage >= 80)
    return 'B';
  else if (percentage >= 70)
    return 'C';
  else if (percentage >= 60)
    return 'D';
  else
    return 'F';
}

/// Main function
int main() {
  double total_points, points_earned = 0, total_assignment_points = 0;
  int CUTOFF_A, CUTOFF_B, CUTOFF_C, CUTOFF_D;
  char letter_grade;

  /// Print title and greeting
  cout << "COSC 1436 Final Grade Calculator\n";
  time_based_greeting();

  /// Input total points and grade cutoffs
  cout << "Please enter the total points possible: "; cin >> total_points;
  cout << "Please enter the cutoff for an A Grade: ";
  cin >> CUTOFF_A;
  cout << "Please enter the cutoff for a B Grade: ";
  cin >> CUTOFF_B;
  cout << "Please enter the cutoff for a C Grade: ";
  cin >> CUTOFF_C;
  cout << "Please enter the cutoff for a D Grade: ";
  cin >> CUTOFF_D;

  /// User inputs assignment grades
  while (true) {
    cout << "\nPlease enter assignment grades\n(Enter DONE when finished): ";
    string input;
    cin >> input;
    if (input == "DONE") {
      break;
    }
    double grade;
    try {
      grade = stod(input);
    } catch (const invalid_argument &e) {
      cout << "\nInvalid input. Please try again. " << endl;
      continue;
    }
    points_earned += grade;
    total_assignment_points += grade;
  }

  /// Calculate the grade percentage
  double grade_percentage = calculate_percentage(points_earned, total_points);

  /// Letter grade
  letter_grade = get_letter_grade(grade_percentage);

  /// Output results
  cout << "\nYour Grade is: " << points_earned << "/" << total_points << "(" << calculate_percentage(points_earned, total_points) << "%)\n";
  cout << "Your Letter Grade is: " << letter_grade << endl;

  return 0;
}
