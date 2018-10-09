# get_args

## Description:
Header for parsing command line arguments, where command line := { option_string [value]+ }+

## Usage:
```
int value;
double array[5];
if(get_flag(argv, argc "--help") || 
  (!get_args(argv, argc, "-int", &value) && 
   !get_args(argv, argc, "-array", array, 5))) { 
    printUsage();
}
```
