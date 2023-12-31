clear all;
clc;

% Specify the file path
cd ..
file_path = 'data.txt';
data_table = readtable(file_path);

% Extract columns as arrays
time = table2array(data_table(:, 1));
id = table2array(data_table(:, 2));
value = table2array(data_table(:, 3));

%Isolate individual vectors
rpm_values = value(id == 12);
speed_values = value(id == 13);
ect_values = value(id == 5);
dist_values = value(id == 49);
fuel_values = value(id == 47);


% Find the maximum values
max_value_for_rpm = max(value(id == 12));
max_value_for_speed = max(value(id == 13));
max_value_for_ect = max(value(id == 5));
max_value_for_dist = max(value(id == 49));
max_value_for_fuel_level = max(value(id == 47));

% Average Values
disp(['Average rpm ' num2str(mean(value(id == 12)))]);
disp(['Average Vehicle Speed ' num2str(mean(value(id == 13)))]);
disp(['Average ECT ' num2str(mean(value(id == 5)))]);




% Find the minimum values
min_value_for_rpm = min(value(id == 12));
min_value_for_speed = min(value(id == 13));
min_value_for_ect = min(value(id == 5));
min_value_for_dist = min(value(id == 49));
min_value_for_fuel_level = min(value(id == 47));


disp(['Max rpm: ' num2str(max_value_for_rpm)]);
disp(['Max Vehicle Speed: ' num2str(max_value_for_speed)]);
disp(['Max Engine Coolant Temperature: ' num2str(max_value_for_ect)]);
disp(['Max Vehicle Distance: ' num2str(max_value_for_dist)]);
disp(['Max Fuel Level Percentage: ' num2str(max_value_for_fuel_level)]);


disp(['Min rpm: ' num2str(min_value_for_rpm)]);
disp(['Min Vehicle Speed: ' num2str(min_value_for_speed)]);
disp(['Min Engine Coolant Temperature: ' num2str(min_value_for_ect)]);
disp(['Min Vehicle Distance: ' num2str(min_value_for_dist)]);
disp(['Min Fuel Level Percentage: ' num2str(min_value_for_fuel_level)]);


%Find Acceleration
% Filter rows where Var2 is equal to 13
selected_rows = data_table.Var2 == 13;

% Extract values from Var1 corresponding to the selected rows
time_speed = data_table.Var1(selected_rows);

dt = diff(time_speed); % Calculate time intervals
dv = diff(speed_values); % Calculate speed differences
dt = dt*3.6;
acceleration = dv ./ dt; % Numerical differentiation to get acceleration

% Find min and max for every 100 values
group_size = 100;
num_groups = floor(length(acceleration) / group_size);

min_acceleration = zeros(1, num_groups);
max_acceleration = zeros(1, num_groups);

for i = 1:num_groups
    start_idx = (i - 1) * group_size + 1;
    end_idx = i * group_size;
    
    min_acceleration(i) = min(acceleration(start_idx:end_idx));
    max_acceleration(i) = max(acceleration(start_idx:end_idx));
end



AccHistoCounts = histcounts(acceleration,[-10:1:10]);

% Narrow down the vectors based on specified conditions
min_acceleration_filtered = min_acceleration(min_acceleration < -5.4);
max_acceleration_filtered = max_acceleration(max_acceleration > 2.7);

% Display the filtered results
disp('Min Acceleration below -5.4:');
disp(length(min_acceleration_filtered));

disp('Max Acceleration above 2.7:');
disp(length(max_acceleration_filtered));

% Create histograms
figure;
bins = 16;

subplot(2, 3, 1);
histogram(rpm_values, [0:500:8000]); 
title('RPM Values');

subplot(2, 3, 2);
histogram(speed_values, [0:10:160]);
title('Speed Values');

subplot(2, 3, 3);
histogram(ect_values, bins); 
title('ECT Values');

subplot(2, 3, 4);
histogram(dist_values, bins); 
title('Distance Values');

subplot(2, 3, 5);
histogram(fuel_values, bins);
title('Fuel Values');

subplot(2, 3, 6);
histogram(acceleration,[-10:1:10]);
title('Acceleration')
sgtitle('Histograms of Individual Vectors');

% Compare Histograms
fname_o = 'vehicle_speed_histogram.bof'; % Output binary file
fod = fopen (fname_o, 'r');
myhistVSS = fread(fod, 256, 'int32=>int32');
figure; bar ([5:10:155],myhistVSS); title ('VSS histogram generated by C++ code');

% Compare Histograms
fname_o = 'engine_speed_histogram.bof'; % Output binary file
fod = fopen (fname_o, 'r');
myhistRPM = fread(fod, 256, 'int32=>int32');
figure; bar ([250:500:7750],myhistRPM); title ('RPM histogram generated by C++ code');

% Compare Histograms
fname_o = 'acceleration_histogram.bof'; % Output binary file
fod = fopen (fname_o, 'r');
myhistACC = fread(fod, 256, 'int32=>int32');
figure; bar ([-9.5:1:9.5],myhistACC); title ('ACC histogram generated by C++ code');

cd MATLAB\

              