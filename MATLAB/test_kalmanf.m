% % Define the system as a constant of 12 volts:
clear s
s.x = 12;
s.A = 1;
% Define a process noise (stdev) of 2 volts as the car operates:
s.Q = 2^2; % variance, hence stdev^2
% Define the voltimeter to measure the voltage itself:
s.H = 1;
% Define a measurement error (stdev) of 2 volts:
s.R = 2^2; % variance, hence stdev^2
% Do not define any system input (control) functions:
s.B = 0;
s.u = 0;
% Do not specify an initial state:
s.x = nan;
s.P = nan;
% Generate random voltages and watch the filter operate.
tru=[]; % truth voltage
for t=1:20
   tru(end+1) = randn*2+12;
   s(end).z = tru(end) + randn*2; % create a measurement
   s(end+1)=kalmanf(s(end)); % perform a Kalman filter iteration
end

