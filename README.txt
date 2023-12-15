How to use the project

Makefile
------------------------
- Makefile builds both Final_seq and Final_tbb
- Final_tbb may not work try changing the tbb flag to -ltbb over -ltbb12

Final_tbb
------------------------
- Final_tbb takes 2 arguments. 
    - Argument 1 is n multiplier. This will copy the data set n times
        - ex. [1,2,3,4] with n multiplier = 2 ------> [1,2,3,4,1,2,3,4]
    - Argument 2 is the number of partial histograms to be used
- During build, there may be an issue with the parallel pipeline, try changing filter to filter_mode or vice versa
    - This can be found in pipline.h (sorry for the spelling) in the RunPipeline function
- .h files required:
    - computation_classes.h (classes for min, max, average, and histograms)
    - pipline.h (pipeline)
    - data_structures.h (duplicate array function and structures for vehicle data)

Final_seq
------------------------
- Final_seq takes 1 argument.
    - Argument 1 is n multiplier. This will copy the data set n times
        - ex. [1,2,3,4] with n multiplier = 2 ------> [1,2,3,4,1,2,3,4]
- .h files required:
    - seq_functions.h (functions for min, max, average, and histograms)
    - data_structures.h (duplicate array function and structures for vehicle data)


Test Cases Python Script
------------------------
Requirements 
- Python
- pandas
    - pip install pandas
    - pip install openpyxl
    - pip install xlsxwriter

- This will automatically run Final_seq and Final_tbb multiple times with different variations for n multiplier 
    and partial histograms
- You can change the variations but this requires knowledge of Python programming
- Be patient! this takes a LONG time to run
- Output in output.xlsx



