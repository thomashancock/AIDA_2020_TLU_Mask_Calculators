import sys


def validateAndGetInput():
    '''
    Checks for valid input and returns it if valid.

    If input is invalid, the program is exited.
    '''
    # Ensure input is valid
    if (len(sys.argv) < 2):
        print("Input error: No trigger configuration given.")
        sys.exit(0)

    if (len(sys.argv[1]) != 6):
        print("Input error: Trigger configuration is incorrect length")
        sys.exit(0)

    for char in sys.argv[1]:
        if (char != '0') and (char != '1') and (char != 'X'):
            print("Input Error: Trigger configuration contains invalid character")
            sys.exit(0)

    return sys.argv[1]


def binaryToDecimal(inputList):
    '''
    Converts a list of 0s and 1s into an integer
    '''
    result = 0
    multiplier = 1
    for x in inputList:
        assert x is 0 or x is 1
        result += x * multiplier
        multiplier = multiplier * 2
    return result


def main():
    '''
    Main function

    Converts input trigger config to trigger mask
    '''

    # Check input is valid and print
    '''
    Trigger input is provided in the form XX1110
    - 1 means trigger signal is required to be present
    - 0 means trigger signal is required to not be present
    - X means trigger signal can be present or not
    '''
    inputStr = validateAndGetInput()
    print("Trigger Config: {0}".format(inputStr))

    # Process input trigger configuration
    '''
    Input is converted into two lists

    Positive list records all the signals which will be required (i.e. 1s)
    Other signals (X and 0) are recorded as 0.
    I.e. XX1110 => 001110

    Negative list records all signals required to be excluded (i.e. the 0s).
    Other signals are recorded as 1s.
    I.e. XX1110 => 111110
    '''
    positiveInput = [int(x) if x.isdigit() else 0 for x in list(sys.argv[1]) ]
    negativeInput = [int(x) if x.isdigit() else 1 for x in list(sys.argv[1]) ]

    # Convert lists to binary representation
    posDec = binaryToDecimal(positiveInput)
    negDec = binaryToDecimal(negativeInput)

    # Take bitwise complement of negative decimal
    '''
    Bitwise and is used to determine whether a particular combination of
        signals matches the required signals.

    In order to use the bitwise and for the negative signals, the negative
        decimal is set to its bitwise complement
    I.e. 111110 => 000001
    '''
    negDec = ~negDec;

    # Calculate Trigger Mask
    '''
    Trigger mask is calculated by selecting all integers which match the chosen
        configuration in the range 0 -> 64.

    To check if a signal matches, bitwise and is used

    For positive signals, the bitwise and returns the original decimal if a
        match is found.
    I.e. 011110 & 001110 = 001110 => Selected
         010110 & 001110 = 000110 => Not selected

    For Negative signals, want to select signals where 0 matches, therefore
        compare bitwise complement of i with bitwise complement of the nagative
        decimal (calculated earlier).
    '''
    mask = 0
    for i in range(64):
        # Check whether i contains same 1s and 0s as trigger config
        if ((i & posDec) == posDec) and ((~i & negDec) == negDec):
            # If it does, switch that digit in the mask to 1
            mask = mask | (0b1 << i)

    # Split mask into low and high and print to screen
    print("\nTrigger Masks:")
    print("Low : {0:08X}".format(mask & 0x00000000FFFFFFFF))
    print("High: {0:08X}".format(mask >> 32))


if __name__ == "__main__":
    main()
