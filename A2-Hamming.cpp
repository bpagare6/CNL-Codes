/*
 * Write a program in C/C++ for error detection and correction for 7/8 bits
 * ASCII codes using Hamming Code. Demonstrate the packets captured
 * traces using Wireshark Packet Analyzer Tool.
 *
 * Author: Bhushan Pagare
 *
 * g++ A2-Hamming.cpp -o A2-Hamming --std=c++11
 */

#include <iostream>
#include <math.h>
#include <string>

using namespace std;

// Calculate the actual value of redundant bits using Hamming code
string hamming(string codeword, int number_of_redundant_bits, int parity) {
  string syndrome(number_of_redundant_bits, '0');
  int k, m = number_of_redundant_bits - 1;
  for (int i = 0; i < number_of_redundant_bits; i++) {
    k = pow(2, i);
    for (int j = codeword.size() - k; j >= 0; j -= ((k)*2)) {
      for (int l = 0; l < k && j - l >= 0; l++) {
        syndrome[m] = ((syndrome[m] - '0') + (codeword[j - l] - '0')) + '0';
      }
    }
    syndrome[m] = ((syndrome[m] - '0') % 2) + '0';
    if (syndrome[m] - '0' == parity) {
      syndrome[m] = '0';
    } else {
      syndrome[m] = '1';
    }
    m--;
  }
  return syndrome;
}

// Append the redundant bits to dataword at right positions,
// it can be all zero bits or bits calculated by hamming code
string append_redundant(string dataword, string redundant) {
  string codeword;
  codeword = dataword + redundant;
  int j = redundant.size() - 1, k = dataword.size() - 1, l = 0;
  for (int i = 0; i < codeword.size(); i++) {
    if (i + 1 == pow(2, l)) {
      codeword[i] = redundant[j];
      j--;
      l++;
    } else {
      codeword[i] = dataword[k];
      k--;
    }
  }
  // Reversing the codeword
  char temp;
  int n = codeword.size();
  for (int i = 0; i < n / 2; i++) {
    temp = codeword[i];
    codeword[i] = codeword[n - i - 1];
    codeword[n - i - 1] = temp;
  }
  return codeword;
}

// Check whether all bits of syndrome are 0, i.e. received codeword is valid
bool check_valid(string syndrome) {
  for (int i = 0; i < syndrome.size(); i++) {
    if (syndrome[i] == '1')
      return false;
  }
  return true;
}

// Get the dataword from codeword
string get_dataword(string codeword, int number_of_redundant_bits) {
  string dataword(codeword.size() - number_of_redundant_bits, '0');
  int l = 0, m = 0;
  for (int i = codeword.size() - 1; i >= 0; i--) {
    if (codeword.size() - i == pow(2, l)) {
      l++;
      continue;
    } else {
      dataword[m] = codeword[i];
      m++;
    }
  }
  // Reversing the dataword
  char temp;
  int n = dataword.size();
  for (int i = 0; i < n / 2; i++) {
    temp = dataword[i];
    dataword[i] = dataword[n - i - 1];
    dataword[n - i - 1] = temp;
  }
  return dataword;
}

// Get the error position from binary value of syndrome
int get_error_position(string syndrome) {
  int error_position = 0, base = 1;
  for (int i = syndrome.size() - 1; i >= 0; i--) {
    if (syndrome[i] == '1') {
      error_position += base;
    }
    base = base * 2;
  }
  return error_position;
}

int main() {
  string dataword = "", codeword = "", syndrome = "";
  int number_of_redundant_bits = 0;

  cout << "\n--------------   Hamming Code Demonstration   --------------\n";
  cout << "Enter the Dataword: ";
  cin >> dataword;

  // Find number of redundat bits such that it satisfies n + r + 1 > 2^r
  while (dataword.size() + number_of_redundant_bits + 1 >
         pow(2, number_of_redundant_bits)) {
    number_of_redundant_bits++;
  }
  // Append the redundant bits to dataword
  for (int i = 0; i < number_of_redundant_bits; i++) {
    syndrome = syndrome + '0';
  }
  codeword = append_redundant(dataword, syndrome);
  cout << "\nDataword: " << dataword
       << "\nNumber of redundant bits: " << number_of_redundant_bits
       << "\nInitial Codeword: " << codeword << "\n";

  syndrome = hamming(codeword, number_of_redundant_bits, 0);
  codeword = append_redundant(dataword, syndrome);

  cout << "\nCodeword to be sent to receiver side: " << codeword << "\n";

  // Take option whether to send as it is or introduce error
  char option;
  int position;
take_option:
  cout << "\nDo you want to send codeword as it is(y/n)? ";
  cin >> option;
  switch (option) {
  // if no error
  case 'y':
  case 'Y':
    cout << "\nCodeword " << codeword << " sent successfully.\n";
    cout << "\nCodeword " << codeword << " received successfully.";
    cout << "\nChecking for error using Hamming code ...";
    syndrome = hamming(codeword, number_of_redundant_bits, 0);
    cout << "\nValues of Parity bits: " << syndrome;
    if (check_valid(syndrome)) {
      cout << "\nAs all Parity bits are Zero, Codeword was received "
              "successfully!\n";
      cout << "\nFinal dataword: "
           << get_dataword(codeword, number_of_redundant_bits) << "\n";

    } else {
      cout << "\nYou have some logical error!\n";
    }
    break;
  // if error
  case 'n':
  case 'N':
  take_position:
    // Take the position to change
    cout << "\nEnter the position you want to change: ";
    cin >> position;
    if (position <= codeword.size()) {
      codeword[position - 1] = ((codeword[position - 1] - '0') ^ 1) + '0';
      cout << "\nCodeword " << codeword << " received successfully.";
      cout << "\nChecking for error using Hamming code ...";
      syndrome = hamming(codeword, number_of_redundant_bits, 0);
      cout << "\nValue of Parity bits: " << syndrome;
      if (check_valid(syndrome)) {
        cout << "\nYou have some logical error!\n";
      } else {
        cout << "\nAs Parity bits are non-zero. There was error in receiving "
                "codeword.\n";
        int error_position = codeword.size() - get_error_position(syndrome) + 1;
        codeword[error_position - 1] =
            ((codeword[error_position - 1] - '0') ^ 1) + '0';
        cout << "\nCorrected Codeword: " << codeword;
        cout << "\nError found at " << error_position << " position.";
        cout << "\nFinal Dataword: "
             << get_dataword(codeword, number_of_redundant_bits) << "\n";
      }
    } else {
      cout << "Wrong Option!\n";
      goto take_position;
    }
    break;
  default:
    cout << "Wrong Option!\n";
    goto take_option;
  }
  cout << "------------------------------------------------------------\n\n";
  return 0;

  return 0;
}