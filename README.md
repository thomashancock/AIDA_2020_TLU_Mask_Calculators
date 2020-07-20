# Trigger Mask Calculators
Scripts for calculating trigger masks for the AIDA 2020 TLU. They take as input a string of six characters (e.g. ```XX1110```) and return the corresponding low and high masks.

When specifying the desired trigger config, ```1``` indicates a trigger is required, ```0``` indicates an veto on the signal is required, and ```X``` means no preference.

Python and C++ implementations are provided. The C++ script can be run as a root macro or compiled into an executable.
```
python New_TLU_Mask_Calculator.py 001110
```
```
root 'New_TLU_Mask_Calculator.cxx("001100")' -b -q
```
```
g++ New_TLU_Mask_Calculator.cxx -o MaskCalculator -std=c++11
./MaskCalculator 001110
```

