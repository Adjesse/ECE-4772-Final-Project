import subprocess
import pandas as pd


data = dict()
cpp_program_path_tbb = "./Final_tbb"
cpp_program_path_seq = "./Final_seq"
data["Test Number"] = list()
data["Cpp Program"] = list()
data["Cpp Arguments"] = list()
data["Elapsed time"] = list()


def function(test_number,cpp_program_path,cpp_arguments):
        data["Test Number"].append(test_number)
        data["Cpp Program"].append(cpp_program_path)

        data["Cpp Arguments"].append("/".join(cpp_arguments))
        result = subprocess.run([cpp_program_path] + cpp_arguments, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

        # Print the standard error (if any)
        if result.stderr:
            print("\nStandard Error:")
            print(result.stderr)

        elapsed_time_line = result.stdout[result.stdout.find("Elapsed time") :]
        time = elapsed_time_line[elapsed_time_line.find(":")+2:elapsed_time_line.find("us")-1]
        data["Elapsed time"].append(int(time))
        #print(data)
try:

    # function("20",cpp_program_path_seq ,["20"])
    # function("50",cpp_program_path_seq ,["50"])

    
# #Test n mult 1-10
#     for test in range(1,11):
#         cpp_arguments_tbb = [str(test) , "1"]
#         test_number = str(test)
#         function(test_number,cpp_program_path_tbb,cpp_arguments_tbb)
# #Test n mult 20 and 50
#     function("11",cpp_program_path_tbb,['20','1'])
#     function("12",cpp_program_path_tbb,['50','1'])

#Test Partial Histogram 1-10
    loop = 1
    for nmult in range(1,3):
        if nmult == 1:
             n_mult = "20"
        else:
             n_mult = "50" 
        for hp in range(1,11):
            cpp_arguments_tbb = [n_mult, str(hp)]
            test_number = str(loop)
            function(test_number,cpp_program_path_tbb,cpp_arguments_tbb)  
            loop = loop + 1 


    df = pd.DataFrame(data)

    # Specify the Excel file path
    excel_file_path = "output.xlsx"
    # Write the DataFrame to Excel
    df.to_excel(excel_file_path, index=False)

    print(f'Data has been written to {excel_file_path}')
except Exception as e:
    print(f"Error: {e}")


