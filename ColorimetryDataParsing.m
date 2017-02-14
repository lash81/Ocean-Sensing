%% ECE 449 - Group 4 Colorimetry - 2/14/2017
% For quick testing, press enter twice to skip entering dye concentration
% and number of samples. Otherwise, follow the instructions.
%
% MAKE SURE YOU CONVERT TO PLAIN TEXT BEFORE PASTING IN SAMPLES. New-line
% characters will mess up the program. You can do this by pasting the data
% into your browser search bar and copying from there.
%
% Suggested improvement to program: 
% Embed concentration data into the copy/pasteable text, so that all the
% data can be pasted and parsed at once, instead of manually segregating by
% concentration and pasting all the data for each concentration separately.

%% Workspace Setup
clc
format shorte

%% Initializations

% Input
initVal = input('In what order of dye concentration will you paste in data? \nENTER AS SINGLE ROW MATRIX. (or press enter to use previously entered values) \n>> ');
if(~isempty(initVal))
    concentrations = initVal;
else
    disp('Your calculations will use the following ordering of concentrations (ppm):')
    disp(concentrations)
end

n = input('\nHow many samples do you want averaged into the final value? (e.g. 30,100) \nDefault is 50 if no value is entered.\n>> ');
if (isempty(n))
    n = 50;
end

% Info
numSamples = length(concentrations);
output = zeros(numSamples, 3);

for i = 1:numSamples
    rawData = input('Paste in data:\n');

    %% Parsing
    data = textscan(rawData, '%*s %*s %d %*s %*s %*s %d %*s %*s %d %*s %d %*s %d %*s %d');
    colorTemp = data{1};
    lux = data{2};
    R = data{3};
    G = data{4}; 
    B = data{5};
    C = data{6};

    %% Processing
    G = cast(G(1:n),'double');
    output(i,:) = [concentrations(i)    mean(G)     std(G)];
end

% output = matrix with numSamples rows containing: concentration, average G, stdG
fprintf('Number of samples averaged: %d\n',n)
%fprintf('Concentration: %2d ppm  Average G: %6f   Std: %5f \n', output);
disp(output)

%% Graphing

% plot trends as line plot with standard deviation as error bars 
