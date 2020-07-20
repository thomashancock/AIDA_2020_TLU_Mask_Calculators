// STD
#include <iostream>
#include <ios>
#include <iomanip>
#include <string>
#include <cstdint>
#include <cmath>
#include <cassert>
#include <algorithm>

bool validateInput(
	const std::string& input
) {
	if (input.length() != 6) {
		std::cout << "Input Error: Trigger configuration is incorrect length" << std::endl;
		return false;
	}

	for (auto& element : input) {
		if ((element != '0')&&(element != '1')&&(element != 'X')) {
			std::cout << "Input Error: Trigger configuration contains invalid character" << std::endl;
			return false;
		}
	}

	return true;
}

unsigned int binCharToDecimal(
	const char input
) {
	assert(('0' == input)||('1' == input));
	if (input == '0') {
		return 0;
	} else {
		return 1;
	}

	// Also works, but is less safe:
	// return static_cast<int>(input) - 48;
}

unsigned int binaryStrToDecimal(
	const std::string& binaryInput
) {
	unsigned int result(0);
	unsigned int multiplier(1);
	for (auto i = 0; i < binaryInput.length(); i++) {
		result += binCharToDecimal(binaryInput[i]) * multiplier;
		multiplier *= 2;
	}
	return result;
}

void printMask(
	const uint64_t mask
) {
	// Split mask into low and high and print to screen
	std::cout << "\nTrigger Masks:" << std::endl;

	// Set printing to uppercase hexadecimal
	std::cout << std::hex << std::uppercase;

	// Print low trigger mask
	std::cout << "Low : "
		<< std::setfill('0')
		<< std::setw(8)
		<< (mask & 0x00000000FFFFFFFF)
		<< std::endl;

	// Print high trigger mask
	std::cout << "High: "
		<< std::setfill('0')
		<< std::setw(8)
		<< (mask >> 32)
		<< std::endl;

	// Set pringing back to decimal
	std::cout << std::dec;

	return;
}

void New_TLU_Mask_Calculator(
	const std::string input
) {
	/*** Check input is valid and print
	 * Trigger input is provided in the form XX1110
	 *  - 1 means trigger signal is required to be present
	 *  - 0 means trigger signal is required to not be present
	 *  - X means trigger signal can be present or not
	 */
	if (!validateInput(input)) {
		return;
	}
	std::cout << "Trigger Config: " << input << std::endl;

	/*** Process input trigger configuration
	 * Input is converted into two lists
	 *
	 * Positive list records all the signals which will be required (i.e. 1s)
	 * Other signals (X and 0) are recorded as 0.
	 * I.e. XX1110 => 001110
	 *
	 * Negative list records all signals required to be excluded (i.e. the 0s).
	 * Other signals are recorded as 1s.
	 * I.e. XX1110 => 111110
	 */
	auto positiveInput = input;
	std::replace(positiveInput.begin(), positiveInput.end(), 'X', '0');

	auto negativeInput = input;
	std::replace(negativeInput.begin(), negativeInput.end(), 'X', '1');

	// Convert string to binary representations
	const auto posDec = binaryStrToDecimal(positiveInput);
	auto negDec = binaryStrToDecimal(negativeInput);

	/*** Take bitwise complement of negative decimal
	 *
	 * Bitwise and is used to determine whether a particular combination of
	 *   signals matches the required signals.
	 *
	 * In order to use the bitwise and for the negative signals, the negative
	 *   decimal is set to its bitwise complement.
	 *   I.e. 111110 => 000001
	 */
	negDec = ~negDec;

	/*** Calculate Trigger Mask
	 *
	 * Trigger mask is calculated by selecting all integers which match the chosen
	 *   configuration in the range 0 -> 64.
	 *
	 * To check if a signal matches, bitwise and is used.
	 *
	 * For positive signals, the bitwise and returns the original decimal if a
	 *   match is found.
	 * I.e. 011110 & 001110 = 001110 => Selected
	 *   010110 & 001110 = 000110 => Not selected
	 *
	 * For Negative signals, want to select signals where 0 matches, therefore
	 *   compare bitwise complement of i with bitwise complement of the nagative
	 *   decimal (calculated earlier).
	 */
	uint64_t mask(0);
	for (unsigned int i = 0; i < 64; i++) {
		// Check whether i contains same 1s as trigger config
		if (((i & posDec) == posDec)&&((~i & negDec) == negDec)) {
			// If it does, switch that digit in the mask to 1
			mask += static_cast<uint64_t>(std::pow(2,i));
		}
	}

	// Print mask
	printMask(mask);

	return;
}

// Optional Main function for compiled version
int main(int argc, char const *argv[]) {
	if (argc < 2) {
		std::cout << "Input Error: No trigger configuration provided" << std::endl;
	} else {
		const std::string input = argv[1];
		New_TLU_Mask_Calculator(input);
	}

	return 0;
}
