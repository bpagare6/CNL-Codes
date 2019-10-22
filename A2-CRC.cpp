/*
 * Write a program in C/C++ for error detection and correction for 7/8 bits
 * ASCII codes using CRC. Demonstrate the packets captured
 * traces using Wireshark Packet Analyzer Tool.
 *
 * Author: Bhushan Pagare
 *
 * g++ A2-CRC.cpp -o A2-CRC --std=c++11
 */

#include <iostream>
#include <string>

using namespace std;

// Calculate CRC, and return the syndrome
string CRC(string codeword, string generator) {
  string syndrome = "";
  syndrome = codeword.substr(0, generator.size() - 1) + '0';
  for (int i = generator.size() - 1; i < codeword.size(); i++) {
    syndrome[generator.size() - 1] = codeword[i];
    if (syndrome[0] >= generator[0]) {
      for (int j = 1; j < generator.size(); j++) {
        syndrome[j - 1] = ((syndrome[j] - '0') ^ (generator[j] - '0')) + '0';
      }
    } else {
      for (int j = 1; j < generator.size(); j++) {
        syndrome[j - 1] = syndrome[j];
      }
    }
  }
  return syndrome.substr(0, syndrome.size() - 1);
}

// Append redundant bits to dataword, it can be all 0's or actual syndrome
string append_redundant_bits(string dataword, string redundant) {
  string codeword = "";
  codeword = dataword + redundant;
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

int main() {
  string codeword = "", dataword = "", generator = "", syndrome = "";
  int number_of_redundant_bits;

  cout << "\n-------------------   CRC Demonstration   ------------------\n";
  cout << "Enter the Dataword: ";
  cin >> dataword;
  cout << "Enter the Generator: ";
  cin >> generator;

  // Append redundant bits, number of redundant bits = length of generator - 1
  number_of_redundant_bits = generator.size() - 1;
  for (int i = 0; i < number_of_redundant_bits; i++) {
    syndrome = syndrome + '0';
  }
  codeword = append_redundant_bits(dataword, syndrome);
  cout << "\nDataword: " << dataword << "\nGenerator: " << generator
       << "\nNumber of redundant bits to be appended: "
       << number_of_redundant_bits << "\nInitial Codeword: " << codeword
       << "\n";

  // Generate the actual value of redundant bits using CRC
  syndrome = CRC(codeword, generator);
  cout << "\nValue of redundant bits: " << syndrome;
  codeword = append_redundant_bits(dataword, syndrome);
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
    cout << "\nChecking for error using CRC ...";
    syndrome = CRC(codeword, generator);
    cout << "\nSyndrome: " << syndrome;
    if (check_valid(syndrome)) {
      cout << "\nAs Syndrome is Zero, Codeword was received successfully!\n";
      cout << "\nFinal Dataword received: "
           << codeword.substr(0, codeword.size() - generator.size() + 1)
           << "\n";
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
      cout << "\nChecking for error using CRC ...";
      syndrome = CRC(codeword, generator);
      cout << "\nSyndrome: " << syndrome;
      if (check_valid(syndrome)) {
        cout << "\nYou have some logical error!\n";
      } else {
        cout << "\nAs Syndrome is non-zero. There was error in receiving "
                "codeword.\n";
        cout << "\nFinal Dataword received: "
             << codeword.substr(0, codeword.size() - generator.size() + 1)
             << "\n";
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
}