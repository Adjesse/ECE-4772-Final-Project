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

#Seq Test 1-10 +20 +50
    for i in range(1,11):
        function(i,cpp_program_path_seq,[str(i)])
    function(20,cpp_program_path_seq ,["20"])
    function(50,cpp_program_path_seq ,["50"])

#Test Partial Histogram 1-10
    loop = 1
    for nmult in range(1,13):
        if nmult == 11:
            n_mult = 20
        elif nmult == 12:
            n_mult = 50
        else:
            n_mult = nmult 
        for hp in range(1,11):
            cpp_arguments_tbb = [str(n_mult), str(hp)]
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


